#include "Views/PlayerRuntimeControllerView.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Features/Systems/Interaction/InteractionSlice.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Store.h"
#include "Views/RuntimeChatWidget.h"
#include "Views/RuntimeStatsWidget.h"
#include "Views/TownspersonView.h"

namespace FG = ForbocAI::Demo::Level;

namespace {

FG::FInteractionCandidatesObserved ObserveTownspersonCandidates(
    const APlayerRuntimeControllerView &Controller, float InteractionDistance,
    TArray<ATownspersonView *> &ObservedTownspeople) {
  FG::FInteractionCandidatesObserved Observation;
  const ForbocAI::Demo::Data::FRuntimeObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectRuntimeObservationIds(
          FG::Store::GetStore().getState());
  Observation.Id = Ids.TownspersonCandidatesObserved;
  Observation.MaxDistance = InteractionDistance;

  const APawn *ControlledPawn = Controller.GetPawn();
  UWorld *World = Controller.GetWorld();
  check(ControlledPawn);
  check(World);

  Observation.Origin = ControlledPawn->GetActorLocation();
  TArray<AActor *> Actors;
  UGameplayStatics::GetAllActorsOfClass(
      World, ATownspersonView::StaticClass(), Actors);
  func::for_each_indexed(
      Actors, static_cast<size_t>(Actors.Num()),
      [&ObservedTownspeople, &Observation](AActor *const &Actor) {
        ATownspersonView *Candidate = Cast<ATownspersonView>(Actor);
        check(Candidate);
        const int32 Index = ObservedTownspeople.Add(Candidate);
        Observation.Candidates.Add({Index, Candidate->GetTownspersonId(),
                                    Candidate->GetActorLocation(),
                                    Candidate->IsPlayerNearby()});
      });
  return Observation;
}

FG::FRuntimeTownspersonInteractionSource ObserveTownspersonInteractionSource(
    const ATownspersonView &Townsperson) {
  return {Townsperson.GetTownspersonName(), Townsperson.GetRole(),
          Townsperson.GetPersona(), Townsperson.GetDefaultPlayerLine(),
          Townsperson.GetPinnedResponse()};
}

void PresentMissingInteraction(const FString &Message) {
  check(GEngine);
  const ForbocAI::Demo::Data::FRuntimeDebugMessageSettings &Debug =
      FG::RuntimeSelectors::SelectRuntimeDebugMessages(
          FG::Store::GetStore().getState());
  GEngine->AddOnScreenDebugMessage(Debug.OnScreenKey, Debug.DurationSeconds,
                                   Debug.Color, Message);
}

} // namespace

APlayerRuntimeControllerView::APlayerRuntimeControllerView()
    : InteractionDistance(
          FG::RuntimeSelectors::SelectTownspersonInteractionDistance(
              FG::Store::GetStore().getState())),
      RuntimeConversationWidgetClass(URuntimeChatWidget::StaticClass()),
      RuntimeConversationWidget(nullptr),
      RuntimeStatsWidgetClass(URuntimeStatsWidget::StaticClass()),
      RuntimeStatsWidget(nullptr) {}

void APlayerRuntimeControllerView::BeginPlay() {
  Super::BeginPlay();
  PresentRuntimeStatsWidget();
}

void APlayerRuntimeControllerView::SetupInputComponent() {
  Super::SetupInputComponent();
  check(InputComponent);
  InputComponent->BindKey(
      EKeys::E, IE_Pressed, this,
      &APlayerRuntimeControllerView::InteractWithNearestTownsperson);
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

  const bool bCanInteract =
      Selection.bFound && Townspeople.IsValidIndex(Selection.CandidateIndex);
  bCanInteract
      ? ([this, &Townspeople, &Selection]() {
          ATownspersonView *Townsperson =
              Townspeople[Selection.CandidateIndex];
          FG::Store::GetStore().dispatch(
              FG::RuntimeActions::TownspersonInteractionSourceObserved()(
                  ObserveTownspersonInteractionSource(*Townsperson)));
          const ForbocAI::Demo::UI::FRuntimeConversationViewModel
              Conversation = FG::RuntimeSelectors::SelectRuntimeConversation(
                  FG::Store::GetStore().getState());
          const FString Reply = Conversation.NpcReply;
          Townsperson->ShowDialogueReply(Reply);
          PresentConversationViewModel(Conversation);
          PresentMissingInteraction(Reply);
        }(), void())
      : (PresentMissingInteraction(Selection.MissingMessage), void());
}

void APlayerRuntimeControllerView::PresentConversationViewModel(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation) {
  RuntimeConversationWidget
      ? void()
      : ([this]() {
          RuntimeConversationWidget = CreateWidget<URuntimeChatWidget>(
              this, RuntimeConversationWidgetClass);
          check(RuntimeConversationWidget);
          RuntimeConversationWidget->AddToViewport();
        }(), void());
  RuntimeConversationWidget->ShowConversationViewModel(Conversation);
}

void APlayerRuntimeControllerView::PresentRuntimeStatsWidget() {
  RuntimeStatsWidget
      ? void()
      : ([this]() {
          RuntimeStatsWidget = CreateWidget<URuntimeStatsWidget>(
              this, RuntimeStatsWidgetClass);
          check(RuntimeStatsWidget);
          RuntimeStatsWidget->AddToViewport(
              FG::RuntimeSelectors::SelectUIRuntimeSettings(
                  FG::Store::GetStore().getState())
                  .StatsOverlay.ZOrder);
        }(), void());
}
