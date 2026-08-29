#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLifecycleSettings {
  bool bSpawnOnBeginPlay;
  int32 FirstRenderIndex;
  int32 IndexStep;
};

/** User Story: As a level geometry consumer, I need to compare lifecycle settings so level spawning and rendering remain deterministic. @fn inline bool operator==(const FLifecycleSettings &Left, const FLifecycleSettings &Right) */
inline bool operator==(const FLifecycleSettings &Left,
                       const FLifecycleSettings &Right) {
  return Left.bSpawnOnBeginPlay == Right.bSpawnOnBeginPlay &&
         Left.FirstRenderIndex == Right.FirstRenderIndex &&
         Left.IndexStep == Right.IndexStep;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
