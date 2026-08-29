#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Labels/Memory/LabelsMemoryTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/Performance/LabelsPerformanceTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/Projection/LabelsProjectionTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/Rate/RateTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/Rendering/LabelsRenderingTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/State/LabelsStateTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Labels/Timing/LabelsTimingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FLabelSettings {
  FPerformanceLabelSettings Performance;
  FMemoryLabelSettings Memory;
  FTimingLabelSettings Timing;
  FRenderingLabelSettings Rendering;
  FRateLabelSettings Rate;
  FStateLabelSettings State;
  FProjectionLabelSettings Projection;
};

/** User Story: As a stats overlay consumer, I need one composed label concern so all metric captions share one immutable boundary. @fn inline bool operator==(const FLabelSettings &Left, const FLabelSettings &Right) */
inline bool operator==(const FLabelSettings &Left,
                       const FLabelSettings &Right) {
  return Left.Performance == Right.Performance && Left.Memory == Right.Memory &&
         Left.Timing == Right.Timing && Left.Rendering == Right.Rendering &&
         Left.Rate == Right.Rate && Left.State == Right.State &&
         Left.Projection == Right.Projection;
}

/** User Story: As a stats overlay consumer, I need to compare composed labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FLabelSettings &Left, const FLabelSettings &Right) */
inline bool operator!=(const FLabelSettings &Left,
                       const FLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
