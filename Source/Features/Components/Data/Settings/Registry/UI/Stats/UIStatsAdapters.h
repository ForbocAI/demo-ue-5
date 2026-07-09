#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FOverlaySettings> {
  static const TArray<TField<FOverlaySettings>>
      &Fields() {
    static const TArray<TField<FOverlaySettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                FOverlaySettings, FramesPerSecondLabel,
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
                FOverlaySettings,
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

template <> struct TJsonSettingsRegistry<FViewNameSettings> {
  static const TArray<TField<FViewNameSettings>> &Fields() {
    static const TArray<TField<FViewNameSettings>> RegisteredFields = {
        NestedSettingField(JSON_SETTING_ATOM(SceneRoot),
                           NestedFieldMembers(&FViewNameSettings::Scene,
                                              &FSceneViewNameSettings::Root)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonCharacterMesh),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::CharacterMesh)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonInteractionSphere),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::InteractionSphere)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonNameText),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::NameText)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonPromptText),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::PromptText)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonDialogueText),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::DialogueText)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonOverlapProfile),
            NestedFieldMembers(&FViewNameSettings::Townsperson,
                               &FTownspersonViewNameSettings::OverlapProfile)),
        NestedSettingField(JSON_SETTING_ATOM(HorseImportedMesh),
                           NestedFieldMembers(&FViewNameSettings::Horse,
                                              &FHorseViewNameSettings::ImportedMesh)),
        NestedSettingField(
            JSON_SETTING_ATOM(HorseMountedRiderMesh),
            NestedFieldMembers(&FViewNameSettings::Horse,
                               &FHorseViewNameSettings::MountedRiderMesh)),
        NestedSettingField(JSON_SETTING_ATOM(HorseNameText),
                           NestedFieldMembers(&FViewNameSettings::Horse,
                                              &FHorseViewNameSettings::NameText)),
        NestedSettingField(JSON_SETTING_ATOM(PlayerCameraBoom),
                           NestedFieldMembers(&FViewNameSettings::Player,
                                              &FPlayerViewNameSettings::CameraBoom)),
        NestedSettingField(
            JSON_SETTING_ATOM(PlayerFollowCamera),
            NestedFieldMembers(&FViewNameSettings::Player,
                               &FPlayerViewNameSettings::FollowCamera)),
        NestedSettingField(
            JSON_SETTING_ATOM(SpeechPresentationMesh),
            NestedFieldMembers(&FViewNameSettings::Speech,
                               &FSpeechViewNameSettings::PresentationMesh)),
        NestedSettingField(JSON_SETTING_ATOM(SpeechComponent),
                           NestedFieldMembers(&FViewNameSettings::Speech,
                                              &FSpeechViewNameSettings::Component))};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FTextSettings> {
  static const TArray<TField<FTextSettings>> &Fields() {
    static const TArray<TField<FTextSettings>> RegisteredFields = {
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonNameRoleFormat),
            NestedFieldMembers(&FTextSettings::Townsperson,
                               &FTownspersonTextSettings::NameRoleFormat)),
        NestedSettingField(JSON_SETTING_ATOM(NpcReplyLog),
                           NestedFieldMembers(&FTextSettings::Townsperson,
                                              &FTownspersonTextSettings::NpcReplyLog)),
        NestedSettingField(JSON_SETTING_ATOM(TownspersonMissingMesh),
                           NestedFieldMembers(&FTextSettings::Townsperson,
                                              &FTownspersonTextSettings::MissingMesh)),
        NestedSettingField(
            JSON_SETTING_ATOM(TownspersonMissingAnimation),
            NestedFieldMembers(&FTextSettings::Townsperson,
                               &FTownspersonTextSettings::MissingAnimation)),
        NestedSettingField(JSON_SETTING_ATOM(HorseMissingMesh),
                           NestedFieldMembers(&FTextSettings::Horse,
                                              &FHorseTextSettings::MissingMesh)),
        NestedSettingField(
            JSON_SETTING_ATOM(HorseMissingWalkAnimation),
            NestedFieldMembers(&FTextSettings::Horse,
                               &FHorseTextSettings::MissingWalkAnimation)),
        NestedSettingField(
            JSON_SETTING_ATOM(RiderMissingWalkAnimation),
            NestedFieldMembers(&FTextSettings::Rider,
                               &FRiderTextSettings::MissingWalkAnimation)),
        NestedSettingField(JSON_SETTING_ATOM(RiderMissingMesh),
                           NestedFieldMembers(&FTextSettings::Rider,
                                              &FRiderTextSettings::MissingMesh)),
        NestedSettingField(JSON_SETTING_ATOM(StartupSdkEnabled),
                           NestedFieldMembers(&FTextSettings::Startup,
                                              &FStartupTextSettings::SdkEnabled)),
        NestedSettingField(
            JSON_SETTING_ATOM(StartupSdkDisabled),
            NestedFieldMembers(&FTextSettings::Startup,
                               &FStartupTextSettings::SdkDisabled))};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FReduxLogSettings, SampleInterval,
                       SampledActionTypes);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
