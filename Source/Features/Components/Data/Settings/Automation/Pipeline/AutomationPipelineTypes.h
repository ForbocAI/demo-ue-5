#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Pipeline {

struct FTests {
  FString IdleTickAdvancesState;
  FString HazardCausesDamage;
  FString AwarenessTriggersAggro;
  FString FleeTransitionOnLowHealth;
  FString MultiBotIndependent;
  FString DeterministicOrder;
};

struct FAssertions {
  FString TickCountAdvanced;
  FString ActionDispatched;
  FString HealthUnchanged;
  FString HealthReducedByHazard;
  FString HealthAfterHazard;
  FString BotHasAggro;
  FString PhaseIsCombat;
  FString RemembersEnemyPosition;
  FString PhaseTransitionedToFlee;
  FString BotsProcessed;
  FString IdleBotFullHealth;
  FString HazardBotTookDamage;
  FString AwareBotHasAggro;
  FString HealthDeterministic;
  FString PositionDeterministic;
  FString PhaseDeterministic;
  FString AggroDeterministic;
  FString ActionCountDeterministic;
};

struct FSettings {
  FString Spec;
  FTests Tests;
  TArray<FString> Groups;
  TArray<FString> Cases;
  FAssertions Assertions;
};

inline bool operator==(const FTests &Left, const FTests &Right) {
  return Left.IdleTickAdvancesState == Right.IdleTickAdvancesState &&
         Left.HazardCausesDamage == Right.HazardCausesDamage &&
         Left.AwarenessTriggersAggro == Right.AwarenessTriggersAggro &&
         Left.FleeTransitionOnLowHealth == Right.FleeTransitionOnLowHealth &&
         Left.MultiBotIndependent == Right.MultiBotIndependent &&
         Left.DeterministicOrder == Right.DeterministicOrder;
}

inline bool operator!=(const FTests &Left, const FTests &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAssertions &Left, const FAssertions &Right) {
  return Left.TickCountAdvanced == Right.TickCountAdvanced &&
         Left.ActionDispatched == Right.ActionDispatched &&
         Left.HealthUnchanged == Right.HealthUnchanged &&
         Left.HealthReducedByHazard == Right.HealthReducedByHazard &&
         Left.HealthAfterHazard == Right.HealthAfterHazard &&
         Left.BotHasAggro == Right.BotHasAggro &&
         Left.PhaseIsCombat == Right.PhaseIsCombat &&
         Left.RemembersEnemyPosition == Right.RemembersEnemyPosition &&
         Left.PhaseTransitionedToFlee == Right.PhaseTransitionedToFlee &&
         Left.BotsProcessed == Right.BotsProcessed &&
         Left.IdleBotFullHealth == Right.IdleBotFullHealth &&
         Left.HazardBotTookDamage == Right.HazardBotTookDamage &&
         Left.AwareBotHasAggro == Right.AwareBotHasAggro &&
         Left.HealthDeterministic == Right.HealthDeterministic &&
         Left.PositionDeterministic == Right.PositionDeterministic &&
         Left.PhaseDeterministic == Right.PhaseDeterministic &&
         Left.AggroDeterministic == Right.AggroDeterministic &&
         Left.ActionCountDeterministic == Right.ActionCountDeterministic;
}

inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Tests == Right.Tests &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Pipeline
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
