#include "Views/LevelGameModeView.h"

#include "Core/functional_core.hpp"
#include "EngineUtils.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Entities/Characters/Player/RuntimePlayerController.h"
#include "Features/Entities/Characters/Player/ThirdPersonCharacter.h"
#include "Views/RuntimeLevelView.h"

namespace {
ARuntimeLevelView *FirstRuntimeLevelView(
    TActorIterator<ARuntimeLevelView> &Iterator) {
  return Iterator ? *Iterator : nullptr;
}

ARuntimeLevelView *FindRuntimeLevelView(UWorld *World) {
  TActorIterator<ARuntimeLevelView> Iterator(World);
  return FirstRuntimeLevelView(Iterator);
}
} // namespace

ALevelGameModeView::ALevelGameModeView() : RuntimeLevelView(nullptr) {
  DefaultPawnClass = AThirdPersonCharacter::StaticClass();
  PlayerControllerClass = ARuntimePlayerController::StaticClass();
}

void ALevelGameModeView::StartPlay() {
  Super::StartPlay();

  UWorld *World = GetWorld();
  func::match(
      func::from_nullable_value(World, World != nullptr),
      [this](UWorld *WorldValue) {
        RuntimeLevelView = FindRuntimeLevelView(WorldValue);
        RuntimeLevelView =
            RuntimeLevelView
                ? RuntimeLevelView
                : WorldValue->SpawnActor<ARuntimeLevelView>(
                      FVector::ZeroVector, FRotator::ZeroRotator);
      },
      [this]() { RuntimeLevelView = nullptr; });
}

void ALevelGameModeView::RestartPlayer(AController *NewPlayer) {
  func::match(
      func::from_nullable_value(NewPlayer, NewPlayer != nullptr),
      [this](AController *Player) {
        FLevelTerrainData SpawnTerrain;
        SpawnTerrain.LoadFromContent();
        const FVector PostOfficeSpawn =
            ForbocAI::Demo::Level::LevelLayoutSlice::ToWorld(
                SpawnTerrain,
                ForbocAI::Demo::Level::LevelLayoutSlice::PlayerSpawnPoint());
        RestartPlayerAtTransform(
            Player,
            FTransform(
                ForbocAI::Demo::Level::LevelLayoutSlice::PlayerSpawnRotation(),
                PostOfficeSpawn));
      },
      []() {});
}
