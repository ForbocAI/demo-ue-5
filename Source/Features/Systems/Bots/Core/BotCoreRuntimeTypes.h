#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
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
  FVector LastKnownPlayerPos;
  float TimeSinceLastSeenPlayer;
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
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;

  EBotCorePhase Phase;

  uint64 TickCount;
};

struct FBotCoreRuntimeInitialStateRequest {
  FString Name;
  ForbocAI::Demo::Data::FBotRuntimeSettings RuntimeSettings;
};

inline FBotCoreStats
CreateBotCoreStats(const ForbocAI::Demo::Data::FBotRuntimeSettings &Settings) {
  return {Settings.InitialHealth, Settings.InitialMaxHealth,
          Settings.InitialMana, Settings.InitialMaxMana,
          Settings.InitialStamina, Settings.InitialMaxStamina};
}

inline FBotCoreMemory
CreateBotCoreMemory(const ForbocAI::Demo::Data::FBotRuntimeSettings &Settings) {
  return {Settings.InitialLastKnownPlayerPosition,
          Settings.InitialTimeSinceLastSeenPlayer, Settings.bInitialHasAggro};
}

inline FBotCoreRuntimeState
CreateBotCoreRuntimeInitialState(
    const FBotCoreRuntimeInitialStateRequest &Request) {
  FBotCoreRuntimeState State;
  State.Id = FGuid::NewGuid();
  State.Name = Request.Name;
  State.Position = Request.RuntimeSettings.InitialPosition;
  State.Rotation = Request.RuntimeSettings.InitialRotation;
  State.Stats = CreateBotCoreStats(Request.RuntimeSettings);
  State.Memory = CreateBotCoreMemory(Request.RuntimeSettings);
  State.RuntimeSettings = Request.RuntimeSettings;
  State.Phase =
      static_cast<EBotCorePhase>(Request.RuntimeSettings.InitialPhase);
  State.TickCount =
      static_cast<uint64>(Request.RuntimeSettings.InitialTickCount);
  return State;
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
