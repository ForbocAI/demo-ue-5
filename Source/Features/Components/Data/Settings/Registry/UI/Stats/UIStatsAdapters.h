#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FStatsOverlaySettings> {
  static const TArray<TField<FStatsOverlaySettings>>
      &Fields() {
    static const TArray<TField<FStatsOverlaySettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                FStatsOverlaySettings, FramesPerSecondLabel,
                StackDepthLabel, PolyCountLabel,
                UsedPhysicalMemoryLabel, PeakPhysicalMemoryLabel,
                UsedVirtualMemoryLabel, GameThreadMillisecondsLabel,
                RenderThreadMillisecondsLabel,
                RhiThreadMillisecondsLabel, GpuMillisecondsLabel,
                DrawCallsLabel, RhiPrimitivesLabel,
                WallDeltaMillisecondsLabel, InputDeltaMillisecondsLabel,
                StatsSelectionMillisecondsLabel, PolyCountMillisecondsLabel,
                EngineIdleMillisecondsLabel,
                EngineIdleOvershootMillisecondsLabel, MaxFpsLabel,
                FrameRateLimitLabel, EffectiveMaxTickRateLabel,
                FixedFrameRateEnabledLabel, FixedFrameRateLabel,
                FixedTimeStepEnabledLabel, FixedDeltaMillisecondsLabel,
                VsyncEnabledLabel, IdleWhenNotForegroundEnabledLabel,
                AppHasFocusLabel, CpuThrottleEnabledLabel,
                AllWindowsHiddenLabel, RootReducerMillisecondsLabel,
                CombinedReducerMillisecondsLabel,
                EcsProjectionMillisecondsLabel, ProjectedEntityCountLabel,
                ProjectedComponentTypeCountLabel, LabelValueSeparator,
                ValueFormat, DecimalValueFormat, DebugMessageFormat,
                BudgetLogFormat, FormatBufferCharacterCount, DebugMessageKey,
                DebugMessageDurationSeconds,
                ViewportLeft, ViewportTop, ViewportWidth, ViewportHeight,
                PanelPadding, StatsRefreshIntervalSeconds,
                PolyCountRefreshIntervalSeconds, BudgetLogIntervalSeconds,
                IntervalResetElapsedSeconds, BudgetScreenshotIntervalSeconds,
                BudgetScreenshotDisabledIntervalSeconds,
                BudgetScreenshotIntervalCommandLineKey,
                BudgetScreenshotDirectory, BudgetScreenshotFileNameFormat,
                BudgetScreenshotInitialIndex, BudgetScreenshotIndexStep,
                FramesPerSecondNumerator, MinimumDeltaSeconds,
                InitialDeltaSeconds, InitialFramesPerSecond, EmptyStackDepth,
                EmptyPolyCount, EmptyTriangleCount, EmptyMemoryMegabytes,
                MemoryBytesPerMegabyte, RhiStatsGpuIndex,
                RhiStatsMinimumGpuIndex, RhiStatsMaximumGpuIndex,
                IdleWhenNotForegroundCVarName, MaxFpsCVarName, VsyncCVarName,
                SecondsToMilliseconds, DiagnosticFalseIntValue,
                DiagnosticTrueIntValue, DiagnosticDefaultIntValue,
                DiagnosticDefaultFloatValue,
                MeshLodIndex, ForcedLodAutomaticModel, LodModelIndexOffset,
                ProcMeshFirstSectionIndex, ProcMeshSectionStep,
                TriangleIndexDivisor, ZOrder, FontSize, bRemoveDpIScale,
                bAutoWrapText, bBudgetScreenshotCreateDirectoryTree,
                bBudgetScreenshotShowUI,
                bBudgetScreenshotAddFilenameSuffix,
                bDiagnosticAllowFrameRateSmoothing,
                FramesPerSecondMediumThreshold, FramesPerSecondHighThreshold,
                StackDepthMediumThreshold, StackDepthHighThreshold,
                PolyCountMediumThreshold, PolyCountHighThreshold,
                MemoryMediumThreshold, MemoryHighThreshold),
            JSON_OBJECT_SETTING_FIELDS(
                FStatsOverlaySettings,
                SettingsAdapters::ReadLinearColorSettings, PanelColor,
                TextColor, LowValueColor, MediumValueColor, HighValueColor)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FObservationIdSettings,
                       PlayerPresentationRequested,
                       PlayerMovementInputObserved,
                       TownspersonPresentationRequested,
                       HorsePresentationRequested,
                       TownspersonCandidatesObserved);

template <> struct TJsonSettingsRegistry<FDebugMessageSettings> {
  static const TArray<TField<FDebugMessageSettings>>
      &Fields() {
    static const TArray<TField<FDebugMessageSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FDebugMessageSettings, OnScreenKey,
                                DurationSeconds),
            JSON_OBJECT_SETTING_FIELD(
                FDebugMessageSettings,
                SettingsAdapters::ReadColorSettings, Color)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FViewNameSettings, SceneRoot,
                       TownspersonCharacterMesh,
                       TownspersonInteractionSphere, TownspersonNameText,
                       TownspersonPromptText, TownspersonDialogueText,
                       TownspersonOverlapProfile, HorseImportedMesh,
                       HorseMountedRiderMesh, HorseNameText, PlayerCameraBoom,
                       PlayerFollowCamera, SpeechPresentationMesh,
                       SpeechComponent);

JSON_SETTINGS_REGISTRY(FTextSettings, TownspersonNameRoleFormat,
                       NpcReplyLog, TownspersonMissingMesh,
                       TownspersonMissingAnimation, HorseMissingMesh,
                       HorseMissingWalkAnimation, RiderMissingWalkAnimation,
                       RiderMissingMesh, StartupSdkEnabled,
                       StartupSdkDisabled);

JSON_SETTINGS_REGISTRY(FReduxLogSettings, SampleInterval,
                       SampledActionTypes);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
