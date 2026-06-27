#include "Features/Entities/Characters/Player/RuntimePlayerController.h"

#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Entities/Characters/Bots/TalkableTownsperson.h"
#include "GameFramework/Pawn.h"
#include "InputCoreTypes.h"
#include "Views/RuntimeChatWidget.h"

ARuntimePlayerController::ARuntimePlayerController()
    : InteractionDistance(
          ForbocAI::Demo::Level::LevelLayoutSlice::TownLotWorldUnits() *
          2.1f),
      ChatWidget(nullptr) {}

void ARuntimePlayerController::SetupInputComponent() {
  Super::SetupInputComponent();
  if (InputComponent) {
    InputComponent->BindKey(EKeys::E, IE_Pressed, this,
                            &ARuntimePlayerController::
                                InteractWithNearestTownsperson);
  }
}

void ARuntimePlayerController::InteractWithNearestTownsperson() {
  ATalkableTownsperson *Townsperson = FindNearestTownsperson();
  const FString PlayerLine =
      Townsperson ? Townsperson->GetDefaultPlayerLine()
                  : FString(TEXT("What should I know here?"));
  const FString Reply =
      Townsperson ? Townsperson->Interact(PlayerLine)
                  : TEXT("No talkable townsperson nearby.");

  if (Townsperson) {
    ShowConversation(Townsperson->GetTownspersonName(),
                     Townsperson->GetRole(), PlayerLine, Reply);
  }

  if (GEngine) {
    GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Cyan, Reply);
  }
}

void ARuntimePlayerController::ShowConversation(
    const FString &Name, const FString &SpeakerRole, const FString &PlayerLine,
    const FString &Reply) {
  if (!ChatWidget) {
    ChatWidget = CreateWidget<URuntimeChatWidget>(
        this, URuntimeChatWidget::StaticClass());
    if (ChatWidget) {
      ChatWidget->AddToViewport();
    }
  }

  if (ChatWidget) {
    ChatWidget->ShowConversation(Name, SpeakerRole, PlayerLine, Reply);
  }
}

ATalkableTownsperson *
ARuntimePlayerController::FindNearestTownsperson() const {
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
