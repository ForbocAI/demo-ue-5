#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

enum class ETownspersonInteractionIntent : uint8 {
  General,
  Dialogue,
  Memory,
  CombatValidation
};

struct FTownspersonSeed {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
  ETownspersonInteractionIntent InteractionIntent;
  TArray<FLevelLocalPoint> PatrolRoute;
};

struct FTownspersonState {
  rtk::EntityState<FTownspersonSeed> Items;
};

inline bool operator==(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DefaultPlayerLine == Right.DefaultPlayerLine &&
         Left.PinnedResponse == Right.PinnedResponse &&
         Left.InteractionIntent == Right.InteractionIntent &&
         Left.PatrolRoute == Right.PatrolRoute;
}

inline bool operator!=(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FTownspersonState &Left,
                       const FTownspersonState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
