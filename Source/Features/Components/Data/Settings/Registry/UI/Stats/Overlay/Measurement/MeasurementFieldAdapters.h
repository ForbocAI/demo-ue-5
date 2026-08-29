#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FFrameMetricSettings, FramesPerSecondNumerator,
                       MinimumDeltaSeconds, InitialDeltaSeconds,
                       InitialFramesPerSecond);
JSON_SETTINGS_REGISTRY(FEmptyMetricSettings, EmptyStackDepth, EmptyPolyCount,
                       EmptyTriangleCount);
JSON_SETTINGS_REGISTRY(FMemoryMetricSettings, EmptyMemoryMegabytes,
                       MemoryBytesPerMegabyte);
JSON_SETTINGS_REGISTRY(FGpuMetricSettings, RhiStatsGpuIndex,
                       RhiStatsMinimumGpuIndex, RhiStatsMaximumGpuIndex);
JSON_SETTINGS_REGISTRY(FCVarSettings, IdleWhenNotForegroundCVarName,
                       MaxFpsCVarName, VsyncCVarName);
JSON_SETTINGS_REGISTRY(FValueSettings, SecondsToMilliseconds,
                       DiagnosticFalseIntValue, DiagnosticTrueIntValue,
                       DiagnosticDefaultIntValue, DiagnosticDefaultFloatValue,
                       bDiagnosticAllowFrameRateSmoothing);
JSON_SETTINGS_REGISTRY(FMeshMetricSettings, MeshLodIndex,
                       ForcedLodAutomaticModel, LodModelIndexOffset,
                       ProcMeshFirstSectionIndex, ProcMeshSectionStep,
                       TriangleIndexDivisor);
JSON_SETTINGS_CONCERN_REGISTRY(FMeasurementSettings, Frame, Empty, Memory, Gpu,
                               CVars, Diagnostics, Mesh);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
