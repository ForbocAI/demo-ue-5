#pragma once

#include "Core/rtk.hpp"

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

struct FCaseLabels {
  FString CreateInitialState;
  FString UpdatePosition;
  FString ReduceHealth;
  FString TransitionToCombat;
  FString TransitionToFlee;
  FString UpdateMemory;
  FString IncrementTick;
  FString DecayAggro;
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
  TArray<FString> Assertions;
  FOrchestratorSettings Orchestrator;
};

inline bool operator==(const FGroups &Left, const FGroups &Right) {
  return Left.StateCreation == Right.StateCreation &&
         Left.Reducers == Right.Reducers && Left.Movement == Right.Movement &&
         Left.Combat == Right.Combat && Left.Awareness == Right.Awareness &&
         Left.TickUpdate == Right.TickUpdate;
}

inline bool operator!=(const FGroups &Left, const FGroups &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) {
  return Left.CreateInitialState == Right.CreateInitialState &&
         Left.UpdatePosition == Right.UpdatePosition &&
         Left.ReduceHealth == Right.ReduceHealth &&
         Left.TransitionToCombat == Right.TransitionToCombat &&
         Left.TransitionToFlee == Right.TransitionToFlee &&
         Left.UpdateMemory == Right.UpdateMemory &&
         Left.IncrementTick == Right.IncrementTick &&
         Left.DecayAggro == Right.DecayAggro;
}

inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) {
  return !(Left == Right);
}

inline bool operator==(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return Left.Registration == Right.Registration &&
         Left.Cycle == Right.Cycle &&
         Left.RuntimeStore == Right.RuntimeStore;
}

inline bool operator!=(const FOrchestratorGroups &Left,
                       const FOrchestratorGroups &Right) {
  return !(Left == Right);
}

inline bool operator==(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return Left.RegisterBot == Right.RegisterBot &&
         Left.RespectObservationInterval == Right.RespectObservationInterval &&
         Left.RegisterBots == Right.RegisterBots &&
         Left.DispatchMovement == Right.DispatchMovement;
}

inline bool operator!=(const FOrchestratorCaseLabels &Left,
                       const FOrchestratorCaseLabels &Right) {
  return !(Left == Right);
}

inline bool operator==(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return Left.ThreeBotsInRootState == Right.ThreeBotsInRootState &&
         Left.BotSelectable == Right.BotSelectable &&
         Left.HorseSelectable == Right.HorseSelectable &&
         Left.PositionSelectable == Right.PositionSelectable &&
         Left.WorldPositionUpdated == Right.WorldPositionUpdated;
}

inline bool operator!=(const FOrchestratorAssertions &Left,
                       const FOrchestratorAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return Left.Spec == Right.Spec && Left.MultiBotSpec == Right.MultiBotSpec &&
         Left.Persona == Right.Persona &&
         Left.WorldContextIndex == Right.WorldContextIndex &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions;
}

inline bool operator!=(const FOrchestratorSettings &Left,
                       const FOrchestratorSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Tests == Right.Tests &&
         Left.Groups == Right.Groups && Left.Cases == Right.Cases &&
         Left.Assertions == Right.Assertions &&
         Left.Orchestrator == Right.Orchestrator;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
