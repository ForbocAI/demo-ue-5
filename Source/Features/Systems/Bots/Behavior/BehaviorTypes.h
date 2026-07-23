#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FBotCorePayload {
  FString Id;
};

struct FBotCoreStats {
  float Health;
  float MaxHealth;
  float Mana;
  float MaxMana;
  float Stamina;
  float MaxStamina;
};

struct FBotCoreMemory {
  FVector KnownPlayerPos;
  float TimeSinceSeenPlayer;
  bool bHasAggro;
};

enum class EBotCorePhase { Idle, Patrol, Combat, Flee, Search };

struct FBotCoreRuntimeState {
  FGuid Id;
  FString Name;

  FVector Position;
  FRotator Rotation;

  FBotCoreStats Stats;
  FBotCoreMemory Memory;
  ForbocAI::Game::Data::FBotSettings Settings;

  EBotCorePhase Phase;

  uint64 TickCount;
};

struct FBotCoreRuntimeInitialStateRequest {
  FString Name;
  ForbocAI::Game::Data::FBotSettings Settings;
};

struct FBotCoreState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  bool bReady;
};

/** User Story: As a systems bots behavior consumer, I need to compare values for equality through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline bool operator==(const FBotCoreState &Left, const FBotCoreState &Right) */
inline bool operator==(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return Left.bReady == Right.bReady &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

/** User Story: As a systems bots behavior consumer, I need to compare values for inequality through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline bool operator!=(const FBotCoreState &Left, const FBotCoreState &Right) */
inline bool operator!=(const FBotCoreState &Left,
                       const FBotCoreState &Right) {
  return !(Left == Right);
}

/** User Story: As a systems bots behavior consumer, I need to invoke create bot core stats through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreStats CreateBotCoreStats(const ForbocAI::Game::Data::FBotSettings &Settings) */
inline FBotCoreStats
CreateBotCoreStats(const ForbocAI::Game::Data::FBotSettings &Settings) {
  return {Settings.InitialHealth, Settings.InitialMaxHealth,
          Settings.InitialMana, Settings.InitialMaxMana,
          Settings.InitialStamina, Settings.InitialMaxStamina};
}

/** User Story: As a systems bots behavior consumer, I need to invoke create bot core memory through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreMemory CreateBotCoreMemory(const ForbocAI::Game::Data::FBotSettings &Settings) */
inline FBotCoreMemory
CreateBotCoreMemory(const ForbocAI::Game::Data::FBotSettings &Settings) {
  return {Settings.InitialKnownPlayerPosition,
          Settings.InitialTimeSinceSeenPlayer, Settings.bInitialHasAggro};
}

/** User Story: As a systems bots behavior consumer, I need to invoke create bot core runtime initial state through a stable signature so the systems bots behavior workflow remains explicit and composable. @fn inline FBotCoreRuntimeState CreateBotCoreRuntimeInitialState( const FBotCoreRuntimeInitialStateRequest &Request) */
inline FBotCoreRuntimeState
CreateBotCoreRuntimeInitialState(
    const FBotCoreRuntimeInitialStateRequest &Request) {
  FBotCoreRuntimeState State;
  State.Id = FGuid::NewGuid();
  State.Name = Request.Name;
  State.Position = Request.Settings.InitialPosition;
  State.Rotation = Request.Settings.InitialRotation;
  State.Stats = CreateBotCoreStats(Request.Settings);
  State.Memory = CreateBotCoreMemory(Request.Settings);
  State.Settings = Request.Settings;
  State.Phase = static_cast<EBotCorePhase>(Request.Settings.InitialPhase);
  State.TickCount = static_cast<uint64>(Request.Settings.InitialTickCount);
  return State;
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
