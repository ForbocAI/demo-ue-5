#include "FrenchGulch/FrenchGulchPlayerController.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"
#include "FrenchGulch/UI/FrenchGulchRuntimeChatWidget.h"
#include "FrenchGulch/Entities/TalkableTownsperson.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"

AFrenchGulchPlayerController::AFrenchGulchPlayerController()
    : InteractionDistance(
          ForbocAI::Demo::FrenchGulch::FrenchGulchLayout::TownLotWorldUnits() *
          2.1f),
      ChatWidget(nullptr) {}

void AFrenchGulchPlayerController::SetupInputComponent() {
  Super::SetupInputComponent();
  if (InputComponent) {
    InputComponent->BindKey(EKeys::E, IE_Pressed, this,
                            &AFrenchGulchPlayerController::
                                InteractWithNearestTownsperson);
  }
}

void AFrenchGulchPlayerController::InteractWithNearestTownsperson() {
  ATalkableTownsperson *Townsperson = FindNearestTownsperson();
  const FString PlayerLine = TEXT("What should I know here?");
  const FString Reply =
      Townsperson ? Townsperson->Interact(PlayerLine)
                  : TEXT("No talkable townsperson nearby.");

  if (!ChatWidget) {
    ChatWidget = CreateWidget<UFrenchGulchRuntimeChatWidget>(
        this, UFrenchGulchRuntimeChatWidget::StaticClass());
    if (ChatWidget) {
      ChatWidget->AddToViewport();
    }
  }

  if (ChatWidget && Townsperson) {
    ChatWidget->ShowConversation(Townsperson->GetTownspersonName(),
                                 Townsperson->GetRole(), PlayerLine, Reply);
  }

  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, Reply);
  }
}

ATalkableTownsperson *
AFrenchGulchPlayerController::FindNearestTownsperson() const {
  const APawn *ControlledPawn = GetPawn();
  UWorld *World = GetWorld();
  if (!ControlledPawn || !World) {
    return nullptr;
  }

  ATalkableTownsperson *Best = nullptr;
  float BestDistSq = FMath::Square(InteractionDistance);
  const FVector Origin = ControlledPawn->GetActorLocation();

  for (TActorIterator<ATalkableTownsperson> It(World); It; ++It) {
    ATalkableTownsperson *Candidate = *It;
    const float DistSq =
        FVector::DistSquared(Origin, Candidate->GetActorLocation());
    if (DistSq <= BestDistSq) {
      Best = Candidate;
      BestDistSq = DistSq;
    }
  }

  return Best;
}
