#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FPerformanceLabelSettings, FramesPerSecondLabel,
                       StackDepthLabel, PolyCountLabel);
JSON_SETTINGS_REGISTRY(FMemoryLabelSettings, UsedPhysicalMemoryLabel,
                       PeakPhysicalMemoryLabel, UsedVirtualMemoryLabel);
JSON_SETTINGS_REGISTRY(FTimingLabelSettings, GameThreadMillisecondsLabel,
                       RenderThreadMillisecondsLabel, RhiThreadMillisecondsLabel,
                       GpuMillisecondsLabel, WallDeltaMillisecondsLabel,
                       InputDeltaMillisecondsLabel);
JSON_SETTINGS_REGISTRY(FRenderingLabelSettings, DrawCallsLabel,
                       RhiPrimitivesLabel, StatsSelectionMillisecondsLabel,
                       PolyCountMillisecondsLabel, EngineIdleMillisecondsLabel,
                       EngineIdleOvershootMillisecondsLabel);
JSON_SETTINGS_REGISTRY(FRateLabelSettings, MaxFpsLabel, FrameRateLimitLabel,
                       EffectiveMaxTickRateLabel, FixedFrameRateEnabledLabel,
                       FixedFrameRateLabel, VsyncEnabledLabel);
JSON_SETTINGS_REGISTRY(FStateLabelSettings, FixedTimeStepEnabledLabel,
                       FixedDeltaMillisecondsLabel,
                       IdleWhenNotForegroundEnabledLabel, AppHasFocusLabel,
                       CpuThrottleEnabledLabel, AllWindowsHiddenLabel);
JSON_SETTINGS_REGISTRY(FProjectionLabelSettings, RootReducerMillisecondsLabel,
                       CombinedReducerMillisecondsLabel,
                       EcsProjectionMillisecondsLabel,
                       ProjectedEntityCountLabel,
                       ProjectedComponentTypeCountLabel);
JSON_SETTINGS_CONCERN_REGISTRY(FLabelSettings, Performance, Memory, Timing,
                               Rendering, Rate, State, Projection);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
