#include "Views/LevelGameModeView.h"

#include "Core/ue_fp.hpp"
#include "EngineUtils.h"
#include "Views/PlayerCharacterView.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Store.h"
#include "Views/PlayerRuntimeControllerView.h"
#include "Views/RuntimeLevelView.h"

namespace FG = ForbocAI::Demo::Level;

namespace {
ARuntimeLevelView *FirstRuntimeLevelView(
    TActorIterator<ARuntimeLevelView> &Iterator) {
  return Iterator ? *Iterator : nullptr;
}

ARuntimeLevelView *FindRuntimeLevelView(UWorld *World) {
  TActorIterator<ARuntimeLevelView> Iterator(World);
  return FirstRuntimeLevelView(Iterator);
}

FTransform LoadPlayerSpawnTransform() {
  auto Spawn = FG::FSpawnPointPayload();
  const auto Result =
      FG::Store::GetStore().dispatch(FG::RuntimeSlice::RequestPlayerSpawn());
  func::thenAsync(Result, [&Spawn](auto Resolved) { Spawn = Resolved; });
  func::executeAsync(Result);
  return FTransform(Spawn.Rotation, Spawn.Location);
}
} // namespace

ALevelGameModeView::ALevelGameModeView() : RuntimeLevelView(nullptr) {
  DefaultPawnClass = APlayerCharacterView::StaticClass();
  PlayerControllerClass = APlayerRuntimeControllerView::StaticClass();
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
        RestartPlayerAtTransform(Player, LoadPlayerSpawnTransform());
      },
      []() {});
}
