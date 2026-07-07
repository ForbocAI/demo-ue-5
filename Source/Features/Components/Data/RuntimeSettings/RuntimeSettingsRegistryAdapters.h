#pragma once

#include "Features/Components/Data/Json/JsonSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsReducers.h"

// Runtime settings adapter registries: TJsonSettingsRegistry specializations
// that declare field-to-struct bindings for each runtime settings type.
// These are RTK entity adapter pattern declarations — field binding data
// that the settings fold machinery traverses.

namespace ForbocAI {
namespace Game {
namespace Data {

namespace RuntimeSettingsAdapters {
using RuntimeSettingsReducers::ReadColorSettings;
using RuntimeSettingsReducers::ReadLinearColorSettings;
} // namespace RuntimeSettingsAdapters

namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FRuntimeStatsOverlaySettings> {
  static const TArray<TJsonSettingsField<FRuntimeStatsOverlaySettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRuntimeStatsOverlaySettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                FRuntimeStatsOverlaySettings, FramesPerSecondLabel,
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
                FormatBufferCharacterCount, DebugMessageKey,
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
                FRuntimeStatsOverlaySettings,
                RuntimeSettingsAdapters::ReadLinearColorSettings, PanelColor,
                TextColor, LowValueColor, MediumValueColor, HighValueColor)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FRuntimeObservationIdSettings,
                       PlayerPresentationRequested,
                       PlayerMovementInputObserved,
                       TownspersonPresentationRequested,
                       HorsePresentationRequested,
                       TownspersonCandidatesObserved);

template <> struct TJsonSettingsRegistry<FRuntimeDebugMessageSettings> {
  static const TArray<TJsonSettingsField<FRuntimeDebugMessageSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRuntimeDebugMessageSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FRuntimeDebugMessageSettings, OnScreenKey,
                                DurationSeconds),
            JSON_OBJECT_SETTING_FIELD(
                FRuntimeDebugMessageSettings,
                RuntimeSettingsAdapters::ReadColorSettings, Color)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FRuntimeViewNameSettings, SceneRoot,
                       TownspersonCharacterMesh,
                       TownspersonInteractionSphere, TownspersonNameText,
                       TownspersonPromptText, TownspersonDialogueText,
                       TownspersonOverlapProfile, HorseImportedMesh,
                       HorseMountedRiderMesh, HorseNameText, PlayerCameraBoom,
                       PlayerFollowCamera, SpeechPresentationMesh,
                       SpeechComponent);

JSON_SETTINGS_REGISTRY(FRuntimeTextSettings, TownspersonNameRoleFormat,
                       NpcReplyLog, TownspersonMissingMesh,
                       TownspersonMissingAnimation, HorseMissingMesh,
                       HorseMissingWalkAnimation, RiderMissingWalkAnimation,
                       RiderMissingMesh, StartupSdkEnabled,
                       StartupSdkDisabled);

JSON_SETTINGS_REGISTRY(FRuntimeReduxLogSettings, SampleInterval,
                       SampledActionTypes);

template <> struct TJsonSettingsRegistry<FUIRuntimeSettings> {
  static const TArray<TJsonSettingsField<FUIRuntimeSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FUIRuntimeSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FUIRuntimeSettings, PlayerRoleLabel,
                                SystemRoleLabel, NpcRoleLabel,
                                UnknownRoleLabel, ChatMessageFormat,
                                HistoryRoleSeparator, HistoryTextStartOffset,
                                HistoryMinimumRoleIndex, PlaceholderTitle,
                                PlaceholderPlayerLine, PlaceholderNpcReply,
                                ConversationTitleFormat, PlayerLineFormat,
                                NpcReplyFormat, ConnectionMessageFormat,
                                UnboundDialogueError, PayloadIdFormat,
                                PanelPadding, TitleSize, BodySize),
            JSON_OBJECT_SETTING_FIELDS(
                FUIRuntimeSettings,
                RuntimeSettingsAdapters::ReadLinearColorSettings, PanelColor,
                TitleColor, PlayerColor, SystemColor, NpcColor, UnknownColor,
                RuntimeReplyColor),
            JSON_OBJECT_SETTING_FIELDS(
                FUIRuntimeSettings,
                ReadSettingsWith<FRuntimeStatsOverlaySettings>(
                    JSON_SETTINGS_ATOMS(
                        FramesPerSecondLabel, StackDepthLabel, PolyCountLabel,
                        UsedPhysicalMemoryLabel, PeakPhysicalMemoryLabel,
                        UsedVirtualMemoryLabel, GameThreadMillisecondsLabel,
                        RenderThreadMillisecondsLabel,
                        RhiThreadMillisecondsLabel, GpuMillisecondsLabel,
                        DrawCallsLabel, RhiPrimitivesLabel,
                        WallDeltaMillisecondsLabel,
                        InputDeltaMillisecondsLabel,
                        StatsSelectionMillisecondsLabel,
                        PolyCountMillisecondsLabel, MaxFpsLabel,
                        EngineIdleMillisecondsLabel,
                        EngineIdleOvershootMillisecondsLabel,
                        FrameRateLimitLabel, EffectiveMaxTickRateLabel,
                        FixedFrameRateEnabledLabel, FixedFrameRateLabel,
                        FixedTimeStepEnabledLabel,
                        FixedDeltaMillisecondsLabel,
                        VsyncEnabledLabel,
                        IdleWhenNotForegroundEnabledLabel, AppHasFocusLabel,
                        CpuThrottleEnabledLabel, AllWindowsHiddenLabel,
                        RootReducerMillisecondsLabel,
                        CombinedReducerMillisecondsLabel,
                        EcsProjectionMillisecondsLabel,
                        ProjectedEntityCountLabel,
                        ProjectedComponentTypeCountLabel, LabelValueSeparator,
                        ValueFormat, DecimalValueFormat,
                        DebugMessageFormat, FormatBufferCharacterCount,
                        DebugMessageKey, DebugMessageDurationSeconds, ViewportLeft,
                        ViewportTop, ViewportWidth, ViewportHeight,
                        PanelPadding, StatsRefreshIntervalSeconds,
                        PolyCountRefreshIntervalSeconds,
                        BudgetLogIntervalSeconds, IntervalResetElapsedSeconds,
                        BudgetScreenshotIntervalSeconds,
                        BudgetScreenshotDisabledIntervalSeconds,
                        BudgetScreenshotIntervalCommandLineKey,
                        BudgetScreenshotDirectory,
                        BudgetScreenshotFileNameFormat,
                        BudgetScreenshotInitialIndex,
                        BudgetScreenshotIndexStep, FramesPerSecondNumerator,
                        MinimumDeltaSeconds, InitialDeltaSeconds,
                        InitialFramesPerSecond, EmptyStackDepth,
                        EmptyPolyCount, EmptyTriangleCount,
                        EmptyMemoryMegabytes, MemoryBytesPerMegabyte,
                        RhiStatsGpuIndex, RhiStatsMinimumGpuIndex,
                        RhiStatsMaximumGpuIndex,
                        IdleWhenNotForegroundCVarName, MaxFpsCVarName,
                        VsyncCVarName, SecondsToMilliseconds,
                        DiagnosticFalseIntValue, DiagnosticTrueIntValue,
                        DiagnosticDefaultIntValue,
                        DiagnosticDefaultFloatValue,
                        MeshLodIndex, ForcedLodAutomaticModel,
                        LodModelIndexOffset,
                        ProcMeshFirstSectionIndex, ProcMeshSectionStep,
                        TriangleIndexDivisor, ZOrder, FontSize,
                        bRemoveDpIScale, bAutoWrapText,
                        bBudgetScreenshotCreateDirectoryTree,
                        bBudgetScreenshotShowUI,
                        bBudgetScreenshotAddFilenameSuffix,
                        bDiagnosticAllowFrameRateSmoothing,
                        FramesPerSecondMediumThreshold,
                        FramesPerSecondHighThreshold,
                        StackDepthMediumThreshold, StackDepthHighThreshold,
                        PolyCountMediumThreshold, PolyCountHighThreshold,
                        MemoryMediumThreshold, MemoryHighThreshold,
                        PanelColor, TextColor, LowValueColor,
                        MediumValueColor, HighValueColor)),
                StatsOverlay)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FDialogueRuntimeSettings, ReplyPayloadIdFormat);

JSON_SETTINGS_REGISTRY(FBotStatPresetSettings, MoveSpeed, AwarenessRange,
                       Resolve, bCanTalk);

template <> struct TJsonSettingsRegistry<FBotRuntimeSettings> {
  static const TArray<TJsonSettingsField<FBotRuntimeSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FBotRuntimeSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FBotRuntimeSettings, InitialName,
                                InitialHealth, InitialMaxHealth, InitialMana,
                                InitialMaxMana, InitialStamina,
                                InitialMaxStamina, MinimumHealth,
                                InitialPosition, InitialRotation,
                                InitialLastKnownPlayerPosition,
                                InitialTimeSinceLastSeenPlayer,
                                EnemySpottedTimeSinceLastSeenPlayer,
                                bInitialHasAggro, bDefaultHazardOverlapping,
                                bDefaultVisibilityCanSeeEnemy, InitialPhase,
                                InitialTickCount, AggroTimeoutSeconds,
                                DamageFleeHealthRatio, PhaseFleeHealthRatio,
                                MovementArrivalDistanceSquared,
                                AggroPositionToleranceSquared,
                                DefaultMovementInterpSpeed,
                                PatrolGoalPriority, PatrolGoalIdFormat,
                                bPatrolGoalInitialCompleted,
                                bActiveGoalComponentHasActiveGoal,
                                ObservationIntervalSeconds,
                                InitialObservationTimeSeconds,
                                bOrchestratorCanEverTick,
                                PatrolTickIntervalSeconds,
                                InitialPatrolPauseRemainingSeconds,
                                bRegisteredBotActive,
                                bPositionPayloadHasLocalLocation,
                                bPositionPayloadHasWorldLocation, StartLog,
                                RegisteredLogFormat, ProcessFailedLogFormat,
                                ExecuteLogFormat, NullActorLabel,
                                MoveActionType, AttackActionType,
                                MoveActionOffset, StateObservationFormat,
                                DefaultBehaviorState),
            JSON_OBJECT_SETTING_FIELDS(
                FBotRuntimeSettings,
                ReadSettingsWith<FBotStatPresetSettings>(
                    JSON_SETTINGS_ATOMS(MoveSpeed, AwarenessRange, Resolve,
                                        bCanTalk)),
                TownspersonStats, HorseStats)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FSpeechVisemeMappingSettings, Phoneme,
                       MorphTargetName, BlendWeight);

JSON_SETTINGS_REGISTRY(FSpeechVowelPhonemeSettings, Character, Phoneme);

JSON_SETTINGS_REGISTRY(FSpeechPhonemeDurationRuleSettings, Kind, Phoneme,
                       Multiplier);

template <> struct TJsonSettingsRegistry<FSpeechRuntimeSettings> {
  static const TArray<TJsonSettingsField<FSpeechRuntimeSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FSpeechRuntimeSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FSpeechRuntimeSettings, RestViseme, RestWeight,
                                SpeechRate, Volume, bEnableLipSync,
                                bCanEverTick, bStartTickEnabled,
                                InitialPlaybackTime, bInitialSpeechActive,
                                EstimatedBasePhonemeSeconds,
                                SilenceCharacters, SilencePhoneme,
                                VisemeChangeTolerance, TtsEndpoint, TtsVerb,
                                TtsContentTypeHeader, TtsContentType,
                                TtsRequestFormat, TtsSuccessResponseCode,
                                MinimumAudioBytes, SpeechStartLogFormat,
                                SpeechAudioReceivedLogFormat,
                                ResetMorphTargets),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechRuntimeSettings,
                ReadSettingsWith<FSpeechVowelPhonemeSettings>(
                    JSON_SETTINGS_ATOMS(Character, Phoneme)),
                VowelPhonemes),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechRuntimeSettings,
                ReadSettingsWith<FSpeechVisemeMappingSettings>(
                    JSON_SETTINGS_ATOMS(Phoneme, MorphTargetName,
                                        BlendWeight)),
                VisemeMappings),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechRuntimeSettings,
                ReadSettingsWith<FSpeechPhonemeDurationRuleSettings>(
                    JSON_SETTINGS_ATOMS(Kind, Phoneme, Multiplier)),
                DurationRules)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
