// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Level/Game/Mode/ModeView.h"

#include "Core/ue_fp.hpp"
#include "EngineUtils.h"
#include "Views/Player/Character/CharacterView.h"
#include "Features/Systems/SystemsActions.h"
#include "Views/Player/Controller/ControllerView.h"
#include "Views/Level/LevelView.h"

namespace FG = ForbocAI::Game::Level;

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
  const auto Result = FG::RuntimeActions::DispatchRequestPlayerSpawn();
  func::thenAsync(Result, [&Spawn](auto Resolved) { Spawn = Resolved; });
  func::executeAsync(Result);
  return FTransform(Spawn.Rotation, Spawn.Location);
}
} // namespace

ALevelGameModeView::ALevelGameModeView() : RuntimeLevelView(nullptr) {
  DefaultPawnClass = APlayerCharacterView::StaticClass();
  PlayerControllerClass = APlayerRuntimeControllerView::StaticClass();
}

void ALevelGameModeView::InitGame(const FString &MapName,
                                  const FString &Options,
                                  FString &ErrorMessage) {
  DefaultPawnClass = APlayerCharacterView::StaticClass();
  PlayerControllerClass = APlayerRuntimeControllerView::StaticClass();
  Super::InitGame(MapName, Options, ErrorMessage);
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
