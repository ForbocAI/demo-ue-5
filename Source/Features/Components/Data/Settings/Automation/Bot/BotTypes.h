#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Automation/Bot/Assertions/AssertionsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Bot {

struct FGroups {
  FString StateCreation;
  FString Reducers;
  FString Movement;
  FString Combat;
  FString Awareness;
  FString TickUpdate;
};

struct FStateCaseLabels {
  FString CreateInitialState;
};

struct FMovementCaseLabels {
  FString UpdatePosition;
};

struct FCombatCaseLabels {
  FString ReduceHealth;
  FString TransitionToCombat;
  FString TransitionToFlee;
};

struct FAwarenessCaseLabels {
  FString UpdateMemory;
};

struct FTickCaseLabels {
  FString IncrementTick;
  FString DecayAggro;
};

struct FCaseLabels {
  FStateCaseLabels State;
  FMovementCaseLabels Movement;
  FCombatCaseLabels Combat;
  FAwarenessCaseLabels Awareness;
  FTickCaseLabels Tick;
};

struct FOrchestratorGroups {
  FString Registration;
  FString Cycle;
  FString RuntimeStore;
};

struct FOrchestratorCaseLabels {
  FString RegisterBot;
  FString RespectObservationInterval;
  FString RegisterBots;
  FString DispatchMovement;
};

struct FOrchestratorAssertions {
  FString ThreeBotsInRootState;
  FString BotSelectable;
  FString HorseSelectable;
  FString PositionSelectable;
  FString WorldPositionUpdated;
};

struct FOrchestratorSettings {
  FString Spec;
  FString MultiBotSpec;
  FString Persona;
  int32 WorldContextIndex;
  FOrchestratorGroups Groups;
  FOrchestratorCaseLabels Cases;
  FOrchestratorAssertions Assertions;
};

struct FSettings {
  FString Spec;
  TArray<FString> Tests;
  FGroups Groups;
  FCaseLabels Cases;
  FAssertions Assertions;
  FOrchestratorSettings Orchestrator;
};

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FGroups &Left, const FGroups &Right) */
inline bool operator==(const FGroups &Left, const FGroups &Right) {
  return Left.StateCreation == Right.StateCreation &&
         Left.Reducers == Right.Reducers && Left.Movement == Right.Movement &&
         Left.Combat == Right.Combat && Left.Awareness == Right.Awareness &&
         Left.TickUpdate == Right.TickUpdate;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FGroups &Left, const FGroups &Right) */
inline bool operator!=(const FGroups &Left, const FGroups &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FStateCaseLabels &Left, const FStateCaseLabels &Right) */
inline bool operator==(const FStateCaseLabels &Left,
                       const FStateCaseLabels &Right) {
  return Left.CreateInitialState == Right.CreateInitialState;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FStateCaseLabels &Left, const FStateCaseLabels &Right) */
inline bool operator!=(const FStateCaseLabels &Left,
                       const FStateCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FMovementCaseLabels &Left, const FMovementCaseLabels &Right) */
inline bool operator==(const FMovementCaseLabels &Left,
                       const FMovementCaseLabels &Right) {
  return Left.UpdatePosition == Right.UpdatePosition;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FMovementCaseLabels &Left, const FMovementCaseLabels &Right) */
inline bool operator!=(const FMovementCaseLabels &Left,
                       const FMovementCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FCombatCaseLabels &Left, const FCombatCaseLabels &Right) */
inline bool operator==(const FCombatCaseLabels &Left,
                       const FCombatCaseLabels &Right) {
  return Left.ReduceHealth == Right.ReduceHealth &&
         Left.TransitionToCombat == Right.TransitionToCombat &&
         Left.TransitionToFlee == Right.TransitionToFlee;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FCombatCaseLabels &Left, const FCombatCaseLabels &Right) */
inline bool operator!=(const FCombatCaseLabels &Left,
                       const FCombatCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FAwarenessCaseLabels &Left, const FAwarenessCaseLabels &Right) */
inline bool operator==(const FAwarenessCaseLabels &Left,
                       const FAwarenessCaseLabels &Right) {
  return Left.UpdateMemory == Right.UpdateMemory;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FAwarenessCaseLabels &Left, const FAwarenessCaseLabels &Right) */
inline bool operator!=(const FAwarenessCaseLabels &Left,
                       const FAwarenessCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FTickCaseLabels &Left, const FTickCaseLabels &Right) */
inline bool operator==(const FTickCaseLabels &Left,
                       const FTickCaseLabels &Right) {
  return Left.IncrementTick == Right.IncrementTick &&
         Left.DecayAggro == Right.DecayAggro;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FTickCaseLabels &Left, const FTickCaseLabels &Right) */
inline bool operator!=(const FTickCaseLabels &Left,
                       const FTickCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) */
inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) {
  return Left.State == Right.State &&
         Left.Movement == Right.Movement &&
         Left.Combat == Right.Combat &&
         Left.Awareness == Right.Awareness &&
         Left.Tick == Right.Tick;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) */
inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FOrchestratorGroups &Left, const FOrchestratorGroups &Right) */
inline bool operator==(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return Left.Registration == Right.Registration &&
         Left.Cycle == Right.Cycle &&
         Left.RuntimeStore == Right.RuntimeStore;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FOrchestratorGroups &Left, const FOrchestratorGroups &Right) */
inline bool operator!=(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FOrchestratorCaseLabels &Left, const FOrchestratorCaseLabels &Right) */
inline bool operator==(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return Left.RegisterBot == Right.RegisterBot &&
         Left.RespectObservationInterval == Right.RespectObservationInterval &&
         Left.RegisterBots == Right.RegisterBots &&
         Left.DispatchMovement == Right.DispatchMovement;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FOrchestratorCaseLabels &Left, const FOrchestratorCaseLabels &Right) */
inline bool operator!=(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FOrchestratorAssertions &Left, const FOrchestratorAssertions &Right) */
inline bool operator==(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return Left.ThreeBotsInRootState == Right.ThreeBotsInRootState &&
         Left.BotSelectable == Right.BotSelectable &&
         Left.HorseSelectable == Right.HorseSelectable &&
         Left.PositionSelectable == Right.PositionSelectable &&
         Left.WorldPositionUpdated == Right.WorldPositionUpdated;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FOrchestratorAssertions &Left, const FOrchestratorAssertions &Right) */
inline bool operator!=(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FOrchestratorSettings &Left, const FOrchestratorSettings &Right) */
inline bool operator==(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return Left.Spec == Right.Spec && Left.MultiBotSpec == Right.MultiBotSpec &&
         Left.Persona == Right.Persona &&
         Left.WorldContextIndex == Right.WorldContextIndex &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FOrchestratorSettings &Left, const FOrchestratorSettings &Right) */
inline bool operator!=(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return !(Left == Right);
}

/** User Story: As a settings automation bot consumer, I need to compare values for equality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Tests == Right.Tests &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions &&
         Left.Orchestrator == Right.Orchestrator;
}

/** User Story: As a settings automation bot consumer, I need to compare values for inequality through a stable signature so the settings automation bot workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
