// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Level/Game/Mode/ModeView.h"

#include "Core/fp.hpp"
#include "EngineUtils.h"
#include "Views/Player/Character/CharacterView.h"
#include "Features/Systems/SystemsActions.h"
#include "Views/Player/Controller/ControllerView.h"
#include "Views/Level/LevelView.h"

namespace FG = ForbocAI::Game::Level;

namespace {
/** User Story: As a level game mode consumer, I need to invoke first runtime level view through a stable signature so the level game mode workflow remains explicit and composable. @fn ARuntimeLevelView *FirstRuntimeLevelView( TActorIterator<ARuntimeLevelView> &Iterator) */
ARuntimeLevelView *FirstRuntimeLevelView(
    TActorIterator<ARuntimeLevelView> &Iterator) {
  return Iterator ? *Iterator : nullptr;
}

/** User Story: As a level game mode consumer, I need to invoke find runtime level view through a stable signature so the level game mode workflow remains explicit and composable. @fn ARuntimeLevelView *FindRuntimeLevelView(UWorld *World) */
ARuntimeLevelView *FindRuntimeLevelView(UWorld *World) {
  TActorIterator<ARuntimeLevelView> Iterator(World);
  return FirstRuntimeLevelView(Iterator);
}

/** User Story: As a level game mode consumer, I need to invoke load player spawn transform through a stable signature so the level game mode workflow remains explicit and composable. @fn FTransform LoadPlayerSpawnTransform() */
FTransform LoadPlayerSpawnTransform() {
  auto Spawn = FG::FPointPayload();
  const auto Result = FG::RuntimeActions::DispatchRequestPlayerSpawn();
  func::thenAsync(Result, [&Spawn](auto Resolved) { Spawn = Resolved; });
  func::executeAsync(Result);
  return FTransform(Spawn.Rotation, Spawn.Location);
}
} // namespace

/** User Story: As a level game mode consumer, I need to invoke alevel game mode view through a stable signature so the level game mode workflow remains explicit and composable. @fn ALevelGameModeView::ALevelGameModeView() */
ALevelGameModeView::ALevelGameModeView() : RuntimeLevelView(nullptr) {
  DefaultPawnClass = APlayerCharacterView::StaticClass();
  PlayerControllerClass = APlayerRuntimeControllerView::StaticClass();
}

/** User Story: As a level game mode consumer, I need to invoke init game through a stable signature so the level game mode workflow remains explicit and composable. @fn void ALevelGameModeView::InitGame(const FString &MapName, const FString &Options, FString &ErrorMessage) */
void ALevelGameModeView::InitGame(const FString &MapName,
                                  const FString &Options,
                                  FString &ErrorMessage) {
  DefaultPawnClass = APlayerCharacterView::StaticClass();
  PlayerControllerClass = APlayerRuntimeControllerView::StaticClass();
  Super::InitGame(MapName, Options, ErrorMessage);
}

/** User Story: As a level game mode consumer, I need to invoke start play through a stable signature so the level game mode workflow remains explicit and composable. @fn void ALevelGameModeView::StartPlay() */
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

/** User Story: As a level game mode consumer, I need to invoke restart player through a stable signature so the level game mode workflow remains explicit and composable. @fn void ALevelGameModeView::RestartPlayer(AController *NewPlayer) */
void ALevelGameModeView::RestartPlayer(AController *NewPlayer) {
  func::match(
      func::from_nullable_value(NewPlayer, NewPlayer != nullptr),
      [this](AController *Player) {
        RestartPlayerAtTransform(Player, LoadPlayerSpawnTransform());
      },
      []() {});
}
