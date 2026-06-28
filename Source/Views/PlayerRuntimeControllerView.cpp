#include "Views/PlayerRuntimeControllerView.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"
#include "Store.h"
#include "Views/RuntimeChatWidget.h"
#include "Views/TownspersonView.h"

namespace FG = ForbocAI::Demo::Level;

namespace {

FG::FInteractionCandidatesObserved ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance,
    TArray<ATownspersonView *> &ObservedTownspeople) {
  FG::FInteractionCandidatesObserved Observation;
  Observation.Id = TEXT("systems/interaction/townspersonCandidatesObserved");
  Observation.MaxDistance = InteractionDistance;

  const APawn *ControlledPawn = Controller.GetPawn();
  UWorld *World = Controller.GetWorld();
  if (!ControlledPawn || !World) {
    return Observation;
  }

  Observation.Origin = ControlledPawn->GetActorLocation();
  for (TActorIterator<ATownspersonView> It(World); It; ++It) {
    ATownspersonView *Candidate = *It;
    const int32 Index = ObservedTownspeople.Add(Candidate);
    Observation.Candidates.Add({Index, Candidate->GetTownspersonId(),
                                Candidate->GetActorLocation(),
                                Candidate->IsPlayerNearby()});
  }
  return Observation;
}

FG::FRuntimeTownspersonInteractionSource ObserveTownspersonInteractionSource(
    const ATownspersonView &Townsperson) {
  return {Townsperson.GetTownspersonName(), Townsperson.GetRole(),
          Townsperson.GetPersona(), Townsperson.GetDefaultPlayerLine(),
          Townsperson.GetPinnedResponse()};
}

void PresentMissingInteraction(const FString &Message) {
  GEngine ? GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, Message)
          : void();
}

} // namespace

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          FG::RuntimeSelectors::SelectTownspersonInteractionDistance(
              FG::Store::GetStore().getState())),
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
  TArray<ATownspersonView *> Townspeople;
  const FG::FInteractionCandidatesObserved Observation =
      ObserveTownspersonCandidates(*this, InteractionDistance, Townspeople);
  FG::Store::GetStore().dispatch(
      FG::InteractionSlice::TownspersonCandidatesObserved(Observation));
  const FG::FInteractionSelection Selection =
      FG::RuntimeSelectors::SelectInteractionSelection(
          FG::Store::GetStore().getState());

  if (!Selection.bFound ||
      !Townspeople.IsValidIndex(Selection.CandidateIndex)) {
    PresentMissingInteraction(Selection.MissingMessage);
    return;
  }

  ATownspersonView *Townsperson = Townspeople[Selection.CandidateIndex];
  FG::Store::GetStore().dispatch(
      FG::RuntimeActions::TownspersonInteractionSourceObserved()(
          ObserveTownspersonInteractionSource(*Townsperson)));
  const FG::FRuntimeTownspersonInteractionRequest Request =
      FG::RuntimeSelectors::SelectTownspersonInteractionRequest(
          FG::Store::GetStore().getState());
  auto InteractionPayload = FG::FRuntimeTownspersonInteractionPayload();
  const auto Result = FG::Store::GetStore().dispatch(
      FG::RuntimeSlice::RequestTownspersonInteraction(Request));
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
