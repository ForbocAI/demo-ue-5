#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FTownspersonSeed {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  TArray<FMapLocalPoint> PatrolRoute;
};

struct FTownspersonState {
  rtk::EntityState<FTownspersonSeed> Items;
};

inline bool operator==(const FTownspersonSeed &Left,
                       const FTownspersonSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Role == Right.Role && Left.Persona == Right.Persona &&
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

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
