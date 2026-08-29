#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Speech/Phoneme/Viseme/Mapping/VisemeMappingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLipSyncSettings {
  FString RestViseme;
  float RestWeight;
  bool bEnableLipSync;
  float VisemeChangeTolerance;
  TArray<FString> ResetMorphTargets;
  TArray<FMappingSettings> VisemeMappings;
};

/** User Story: As a speech lip-sync consumer, I need to compare lip-sync settings so viseme projection remains deterministic. @fn inline bool operator==(const FLipSyncSettings &Left, const FLipSyncSettings &Right) */
inline bool operator==(const FLipSyncSettings &Left,
                       const FLipSyncSettings &Right) {
  return Left.RestViseme == Right.RestViseme &&
         FMath::IsNearlyEqual(Left.RestWeight, Right.RestWeight) &&
         Left.bEnableLipSync == Right.bEnableLipSync &&
         FMath::IsNearlyEqual(Left.VisemeChangeTolerance,
                              Right.VisemeChangeTolerance) &&
         Left.ResetMorphTargets == Right.ResetMorphTargets &&
         Left.VisemeMappings == Right.VisemeMappings;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
