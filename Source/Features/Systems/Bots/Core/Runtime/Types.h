#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

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

inline FBotCoreStats
CreateBotCoreStats(const ForbocAI::Game::Data::FBotSettings &Settings) {
  return {Settings.InitialHealth, Settings.InitialMaxHealth,
          Settings.InitialMana, Settings.InitialMaxMana,
          Settings.InitialStamina, Settings.InitialMaxStamina};
}

inline FBotCoreMemory
CreateBotCoreMemory(const ForbocAI::Game::Data::FBotSettings &Settings) {
  return {Settings.InitialKnownPlayerPosition,
          Settings.InitialTimeSinceSeenPlayer, Settings.bInitialHasAggro};
}

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
  State.Phase =
      static_cast<EBotCorePhase>(Request.Settings.InitialPhase);
  State.TickCount =
      static_cast<uint64>(Request.Settings.InitialTickCount);
  return State;
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
