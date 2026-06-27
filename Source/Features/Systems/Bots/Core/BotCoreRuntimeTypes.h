#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FBotCoreStats {
  float Health = 100.0f;
  float MaxHealth = 100.0f;
  float Mana = 100.0f;
  float MaxMana = 100.0f;
  float Stamina = 100.0f;
  float MaxStamina = 100.0f;
};

struct FBotCoreMemory {
  FVector LastKnownPlayerPos = FVector::ZeroVector;
  float TimeSinceLastSeenPlayer = 9999.0f;
  bool bHasAggro = false;
};

enum class EBotCorePhase { Idle, Patrol, Combat, Flee, Search };

struct FBotCoreRuntimeState {
  FGuid Id;
  FString Name;

  FVector Position;
  FRotator Rotation;

  FBotCoreStats Stats;
  FBotCoreMemory Memory;

  EBotCorePhase Phase = EBotCorePhase::Idle;

  uint64 TickCount = 0;
};

inline FBotCoreRuntimeState CreateBotCoreRuntimeInitialState(FString InName) {
  FBotCoreRuntimeState State;
  State.Id = FGuid::NewGuid();
  State.Name = InName;
  State.Position = FVector::ZeroVector;
  State.Rotation = FRotator::ZeroRotator;
  return State;
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
