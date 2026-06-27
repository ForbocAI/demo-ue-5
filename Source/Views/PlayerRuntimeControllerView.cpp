#include "Views/PlayerRuntimeControllerView.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Features/Components/Spatial/SpatialSelectors.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/UI/UISelectors.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"
#include "Store.h"
#include "Views/RuntimeChatWidget.h"
#include "Views/TownspersonView.h"

namespace FG = ForbocAI::Demo::Level;

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          ForbocAI::Demo::Level::SpatialSelectors::SelectTownLotWorldUnits() *
          2.1f),
      RuntimeConversationWidget(nullptr) {}

void APlayerRuntimeControllerView::SetupInputComponent() {
  Super::SetupInputComponent();
  if (InputComponent) {
    InputComponent->BindKey(EKeys::E, IE_Pressed, this,
                            &APlayerRuntimeControllerView::
                                InteractWithNearestTownsperson);
  }
}

void APlayerRuntimeControllerView::InteractWithNearestTownsperson() {
  ATownspersonView *Townsperson = FindNearestTownsperson();
  if (!Townsperson) {
    const FString Reply =
        FG::UISelectors::SelectNearestTownsperson({}).MissingMessage;
    GEngine ? GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, Reply)
            : void();
    return;
  }

  auto InteractionPayload = FG::FRuntimeTownspersonInteractionPayload();
  const auto Result = FG::Store::GetStore().dispatch(
      FG::RuntimeSlice::RequestTownspersonInteraction(
          {Townsperson->GetTownspersonName(), Townsperson->GetRole(),
           Townsperson->GetPersona(), Townsperson->GetDefaultPlayerLine(),
           Townsperson->GetPinnedResponse()}));
  func::thenAsync(Result, [&InteractionPayload](auto Resolved) {
    InteractionPayload = Resolved;
  });
  func::executeAsync(Result);
  const FString Reply = InteractionPayload.DialogueReply.Reply;
  Townsperson->ShowDialogueReply(Reply);
  PresentConversationViewModel(InteractionPayload.UI.Conversation);

  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, Reply);
  }
}

void APlayerRuntimeControllerView::PresentConversationViewModel(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation) {
  if (!RuntimeConversationWidget) {
    RuntimeConversationWidget = CreateWidget<URuntimeChatWidget>(
        this, URuntimeChatWidget::StaticClass());
    if (RuntimeConversationWidget) {
      RuntimeConversationWidget->AddToViewport();
    }
  }

  if (RuntimeConversationWidget) {
    RuntimeConversationWidget->ShowConversationViewModel(Conversation);
  }
}

ATownspersonView *
APlayerRuntimeControllerView::FindNearestTownsperson() const {
  const APawn *ControlledPawn = GetPawn();
  UWorld *World = GetWorld();
  if (!ControlledPawn || !World) {
    return nullptr;
  }

  const FVector Origin = ControlledPawn->GetActorLocation();
  TArray<ATownspersonView *> Townspeople;
  TArray<FG::FUITownspersonInteractionCandidate> Candidates;

  for (TActorIterator<ATownspersonView> It(World); It; ++It) {
    ATownspersonView *Candidate = *It;
    const int32 Index = Townspeople.Add(Candidate);
    Candidates.Add({Index, Candidate->GetActorLocation(),
                    Candidate->IsPlayerNearby()});
  }

  const FG::FUISelectedTownspersonViewModel Selection =
      FG::UISelectors::SelectNearestTownsperson(
          {Origin, InteractionDistance, Candidates});
  return Selection.bFound && Townspeople.IsValidIndex(Selection.CandidateIndex)
             ? Townspeople[Selection.CandidateIndex]
             : nullptr;
}
