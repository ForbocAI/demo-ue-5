#include "Features/World/PrototypeGameMode.h"

#include "Features/Components/MapLayout.h"
#include "Features/Player/RuntimePlayerController.h"
#include "Features/World/RuntimeLevel.h"
#include "Features/Player/ThirdPersonCharacter.h"
#include "Features/Components/TerrainData.h"
#include "EngineUtils.h"

APrototypeGameMode::APrototypeGameMode()
    : RuntimeLevel(nullptr) {
  DefaultPawnClass = AThirdPersonCharacter::StaticClass();
  PlayerControllerClass = ARuntimePlayerController::StaticClass();
}

void APrototypeGameMode::StartPlay() {
  Super::StartPlay();

  UWorld *World = GetWorld();
  if (!World) {
    RuntimeLevel = nullptr;
    return;
  }

  for (TActorIterator<ARuntimeLevel> It(World); It; ++It) {
    RuntimeLevel = *It;
    break;
  }

  if (!RuntimeLevel) {
    RuntimeLevel =
        World->SpawnActor<ARuntimeLevel>(FVector::ZeroVector,
                                                    FRotator::ZeroRotator);
  }
}

void APrototypeGameMode::RestartPlayer(AController *NewPlayer) {
  if (!NewPlayer) {
    return;
  }

  FMapTerrainData SpawnTerrain;
  SpawnTerrain.LoadFromContent();
  const FVector PostOfficeSpawn =
      ForbocAI::Demo::Map::MapLayout::ToWorld(
          SpawnTerrain,
          ForbocAI::Demo::Map::MapLayout::PlayerSpawnPoint());
  RestartPlayerAtTransform(
      NewPlayer,
      FTransform(
          ForbocAI::Demo::Map::MapLayout::PlayerSpawnRotation(),
          PostOfficeSpawn));
}
