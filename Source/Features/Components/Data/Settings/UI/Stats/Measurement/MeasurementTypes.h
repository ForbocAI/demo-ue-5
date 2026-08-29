#pragma once

#include "Features/Components/Data/Settings/UI/Stats/Measurement/CVar/CVarTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Diagnostic/DiagnosticTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Empty/EmptyTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Frame/FrameTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Gpu/GpuTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Memory/MeasurementMemoryTypes.h"
#include "Features/Components/Data/Settings/UI/Stats/Measurement/Mesh/MeasurementMeshTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FMeasurementSettings {
  FFrameMetricSettings Frame;
  FEmptyMetricSettings Empty;
  FMemoryMetricSettings Memory;
  FGpuMetricSettings Gpu;
  FCVarSettings CVars;
  FValueSettings Diagnostics;
  FMeshMetricSettings Mesh;
};

/** User Story: As a stats overlay consumer, I need one composed measurement concern so metric constants share an immutable boundary. @fn inline bool operator==(const FMeasurementSettings &Left, const FMeasurementSettings &Right) */
inline bool operator==(const FMeasurementSettings &Left,
                       const FMeasurementSettings &Right) {
  return Left.Frame == Right.Frame && Left.Empty == Right.Empty &&
         Left.Memory == Right.Memory && Left.Gpu == Right.Gpu &&
         Left.CVars == Right.CVars &&
         Left.Diagnostics == Right.Diagnostics && Left.Mesh == Right.Mesh;
}

/** User Story: As a stats overlay consumer, I need to compare composed measurement settings for inequality so immutable transitions remain explicit. @fn inline bool operator!=(const FMeasurementSettings &Left, const FMeasurementSettings &Right) */
inline bool operator!=(const FMeasurementSettings &Left,
                       const FMeasurementSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
