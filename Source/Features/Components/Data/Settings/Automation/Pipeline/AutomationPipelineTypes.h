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

struct FTickAssertions {
  FString TickCountAdvanced;
  FString ActionDispatched;
  FString HealthUnchanged;
};

struct FHazardAssertions {
  FString HealthReducedByHazard;
  FString HealthAfterHazard;
  FString HazardBotTookDamage;
};

struct FAggroAssertions {
  FString BotHasAggro;
  FString PhaseIsCombat;
  FString RemembersEnemyPosition;
  FString AwareBotHasAggro;
};

struct FFleeAssertions {
  FString PhaseTransitionedToFlee;
};

struct FBotAssertions {
  FString BotsProcessed;
  FString IdleBotFullHealth;
};

struct FDeterminismAssertions {
  FString HealthDeterministic;
  FString PositionDeterministic;
  FString PhaseDeterministic;
  FString AggroDeterministic;
  FString ActionCountDeterministic;
};

struct FAssertions {
  FTickAssertions Tick;
  FHazardAssertions Hazard;
  FAggroAssertions Aggro;
  FFleeAssertions Flee;
  FBotAssertions Bots;
  FDeterminismAssertions Determinism;
};

struct FSettings {
  FString Spec;
  FTests Tests;
  TArray<FString> Groups;
  TArray<FString> Cases;
  FAssertions Assertions;
};

/** User Story: As a settings automation pipeline consumer, I need to compare values for equality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator==(const FTests &Left, const FTests &Right) */
inline bool operator==(const FTests &Left, const FTests &Right) {
  return Left.IdleTickAdvancesState == Right.IdleTickAdvancesState &&
         Left.HazardCausesDamage == Right.HazardCausesDamage &&
         Left.AwarenessTriggersAggro == Right.AwarenessTriggersAggro &&
         Left.FleeTransitionOnLowHealth == Right.FleeTransitionOnLowHealth &&
         Left.MultiBotIndependent == Right.MultiBotIndependent &&
         Left.DeterministicOrder == Right.DeterministicOrder;
}

/** User Story: As a settings automation pipeline consumer, I need to compare values for inequality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator!=(const FTests &Left, const FTests &Right) */
inline bool operator!=(const FTests &Left, const FTests &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation pipeline consumer, I need to compare values for equality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator==(const FAssertions &Left, const FAssertions &Right) */
inline bool operator==(const FAssertions &Left, const FAssertions &Right) {
  return Left.Tick.TickCountAdvanced == Right.Tick.TickCountAdvanced &&
         Left.Tick.ActionDispatched == Right.Tick.ActionDispatched &&
         Left.Tick.HealthUnchanged == Right.Tick.HealthUnchanged &&
         Left.Hazard.HealthReducedByHazard ==
             Right.Hazard.HealthReducedByHazard &&
         Left.Hazard.HealthAfterHazard == Right.Hazard.HealthAfterHazard &&
         Left.Hazard.HazardBotTookDamage ==
             Right.Hazard.HazardBotTookDamage &&
         Left.Aggro.BotHasAggro == Right.Aggro.BotHasAggro &&
         Left.Aggro.PhaseIsCombat == Right.Aggro.PhaseIsCombat &&
         Left.Aggro.RemembersEnemyPosition ==
             Right.Aggro.RemembersEnemyPosition &&
         Left.Aggro.AwareBotHasAggro == Right.Aggro.AwareBotHasAggro &&
         Left.Flee.PhaseTransitionedToFlee ==
             Right.Flee.PhaseTransitionedToFlee &&
         Left.Bots.BotsProcessed == Right.Bots.BotsProcessed &&
         Left.Bots.IdleBotFullHealth == Right.Bots.IdleBotFullHealth &&
         Left.Determinism.HealthDeterministic ==
             Right.Determinism.HealthDeterministic &&
         Left.Determinism.PositionDeterministic ==
             Right.Determinism.PositionDeterministic &&
         Left.Determinism.PhaseDeterministic ==
             Right.Determinism.PhaseDeterministic &&
         Left.Determinism.AggroDeterministic ==
             Right.Determinism.AggroDeterministic &&
         Left.Determinism.ActionCountDeterministic ==
             Right.Determinism.ActionCountDeterministic;
}

/** User Story: As a settings automation pipeline consumer, I need to compare values for inequality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator!=(const FAssertions &Left, const FAssertions &Right) */
inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation pipeline consumer, I need to compare values for equality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Tests == Right.Tests &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions;
}

/** User Story: As a settings automation pipeline consumer, I need to compare values for inequality through a stable signature so the settings automation pipeline workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Pipeline
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
