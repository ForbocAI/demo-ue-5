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

struct FCases {
  FString CreateInitialState;
  FString UpdatePosition;
  FString ReduceHealth;
  FString TransitionToCombat;
  FString TransitionToFlee;
  FString UpdateMemory;
  FString IncrementTick;
  FString DecayAggro;
};

struct FSettings {
  FString Spec;
  TArray<FString> Tests;
  FGroups Groups;
  FCases Cases;
  TArray<FString> Assertions;
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

inline bool operator==(const FCases &Left, const FCases &Right) {
  return Left.CreateInitialState == Right.CreateInitialState &&
         Left.UpdatePosition == Right.UpdatePosition &&
         Left.ReduceHealth == Right.ReduceHealth &&
         Left.TransitionToCombat == Right.TransitionToCombat &&
         Left.TransitionToFlee == Right.TransitionToFlee &&
         Left.UpdateMemory == Right.UpdateMemory &&
         Left.IncrementTick == Right.IncrementTick &&
         Left.DecayAggro == Right.DecayAggro;
}

inline bool operator!=(const FCases &Left, const FCases &Right) {
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

} // namespace Bot
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
