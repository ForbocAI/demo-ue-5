#include "FrenchGulch/FrenchGulchPrototypeGameMode.h"

#include "FrenchGulch/Components/FrenchGulchLayout.h"
#include "FrenchGulch/FrenchGulchPlayerController.h"
#include "FrenchGulch/FrenchGulchRuntimeLevel.h"
#include "FrenchGulch/Components/FrenchGulchTerrainData.h"
#include "EngineUtils.h"
#include "GameFramework/DefaultPawn.h"

AFrenchGulchPrototypeGameMode::AFrenchGulchPrototypeGameMode()
    : RuntimeLevel(nullptr) {
  DefaultPawnClass = ADefaultPawn::StaticClass();
  PlayerControllerClass = AFrenchGulchPlayerController::StaticClass();
}

void AFrenchGulchPrototypeGameMode::StartPlay() {
  Super::StartPlay();

  UWorld *World = GetWorld();
  if (!World) {
    RuntimeLevel = nullptr;
    return;
  }

  for (TActorIterator<AFrenchGulchRuntimeLevel> It(World); It; ++It) {
    RuntimeLevel = *It;
    break;
  }

  if (!RuntimeLevel) {
    RuntimeLevel =
        World->SpawnActor<AFrenchGulchRuntimeLevel>(FVector::ZeroVector,
                                                    FRotator::ZeroRotator);
  }
}

void AFrenchGulchPrototypeGameMode::RestartPlayer(AController *NewPlayer) {
  if (!NewPlayer) {
    return;
  }

  FFrenchGulchTerrainData SpawnTerrain;
  SpawnTerrain.LoadFromContent();
  const FVector PostOfficeSpawn =
      ForbocAI::Demo::FrenchGulch::FrenchGulchLayout::ToWorld(
          SpawnTerrain,
          ForbocAI::Demo::FrenchGulch::FrenchGulchLayout::PlayerSpawnPoint());
  RestartPlayerAtTransform(
      NewPlayer,
      FTransform(
          ForbocAI::Demo::FrenchGulch::FrenchGulchLayout::PlayerSpawnRotation(),
          PostOfficeSpawn));
}
