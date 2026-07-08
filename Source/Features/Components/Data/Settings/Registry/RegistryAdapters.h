#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Data/Settings/SettingsSlice.h"

// Settings adapter registries: TJsonSettingsRegistry specializations that
// declare field-to-struct bindings for each settings type.
// These are RTK entity adapter pattern declarations — field binding data
// that the settings fold machinery traverses.

namespace ForbocAI {
namespace Game {
namespace Data {

namespace SettingsAdapters {
using SettingsReducers::ReadColorSettings;
using SettingsReducers::ReadLinearColorSettings;
} // namespace SettingsAdapters

namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FStatsOverlaySettings> {
  static const TArray<TJsonSettingsField<FStatsOverlaySettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FStatsOverlaySettings>>
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
  static const TArray<TJsonSettingsField<FDebugMessageSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FDebugMessageSettings>>
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

JSON_SETTINGS_REGISTRY(FEcsDomainRegistrationSettings, Path, Kind);

template <> struct TJsonSettingsRegistry<FEcsSettings> {
  static const TArray<TJsonSettingsField<FEcsSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FEcsSettings>> RegisteredFields = {
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FEcsSettings,
            ReadSettingsWith<FEcsDomainRegistrationSettings>(
                JSON_SETTINGS_ATOMS(Path, Kind)),
            DomainRegistry)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FStoreAutomationSettings,
                       DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                       ProjectionGateLabels, ReduxLoggerCategory,
                       ReduxLoggerActionTitlePrefix, TerrainEntity,
                       TerrainProjectionDomain);

JSON_SETTINGS_REGISTRY(FContentAssetExpectationSettings, Label, Path);

JSON_SETTINGS_REGISTRY(FContentConfigExpectationSettings, Label, Section, Key,
                       Expected);

JSON_SETTINGS_REGISTRY(FForbiddenSourcePatternSettings, Token, Message);

template <> struct TJsonSettingsRegistry<FContentAssetsAutomationSettings> {
  static const TArray<TJsonSettingsField<FContentAssetsAutomationSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FContentAssetsAutomationSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FContentAssetsAutomationSettings,
                                SkeletalMeshLoadsLabelFormat,
                                SkeletalMeshLodDataLabelFormat,
                                NativeLodAuditCountFormat,
                                NativeLodAuditEntryFormat),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FContentAssetsAutomationSettings,
                ReadSettingsWith<FContentAssetExpectationSettings>(
                    JSON_SETTINGS_ATOMS(Label, Path)),
                Packages, Classes, SpeechComponentClasses, Assets,
                SkeletalMeshLods, MissingPackages),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FContentAssetsAutomationSettings,
                ReadSettingsWith<FContentConfigExpectationSettings>(
                    JSON_SETTINGS_ATOMS(Label, Section, Key, Expected)),
                ConfigValues)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<FRTKComplianceAutomationSettings> {
  static const TArray<TJsonSettingsField<FRTKComplianceAutomationSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FRTKComplianceAutomationSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                FRTKComplianceAutomationSettings, SourceFileSuffixes,
                AllowedBoundaryFragments, SourceDirectoryName,
                SourceSearchPattern, ViolationMessageFormat,
                SourceReadFailureFormat, StoreBoundaryLabel,
                ViolationCountIncrement, CleanViolationCount),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FRTKComplianceAutomationSettings,
                ReadSettingsWith<FForbiddenSourcePatternSettings>(
                    JSON_SETTINGS_ATOMS(Token, Message)),
                ForbiddenPatterns)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(
    FConversationUITestAutomationSettings, Labels, PlayerRole, PlayerText,
    ExpectedPlayerMessageText, ExpectedPlayerMessageGreen, History,
    ExpectedHistoryCount, TaggedHistoryIndex, ExpectedTaggedHistoryText,
    UntaggedHistoryIndex, ExpectedUntaggedHistoryText, SubmittedInput,
    ExpectedSubmittedText, SpeakerName, SpeakerRole, ConversationPlayerLine,
    ConversationNpcReply, ExpectedConversationTitle,
    ExpectedConversationPlayerLine, ExpectedConversationNpcReply);

JSON_SETTINGS_REGISTRY(
    FProtocolLoopAutomationSettings, Labels, RunEnvironmentVariable,
    SkipWarning, ApiUrl, AgentPersona, ImmutablePersona, StatePersona,
    StateJson, StateNeedle, AsyncPersona, AsyncPrompt, BridgePersona,
    BridgeActionType, MinimumRuleCount);

JSON_SETTINGS_REGISTRY(FAutomationTextSettings, SpecName, TestNames,
                       GroupLabels, CaseLabels, AssertionLabels);

template <> struct TJsonSettingsRegistry<FAutomationSettings> {
  static const TArray<TJsonSettingsField<FAutomationSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FAutomationSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FStoreAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                        ProjectionGateLabels, ReduxLoggerCategory,
                        ReduxLoggerActionTitlePrefix, TerrainEntity,
                        TerrainProjectionDomain)),
                Store),
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FContentAssetsAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        Packages, Classes, SpeechComponentClasses,
                        ConfigValues, Assets, SkeletalMeshLods,
                        MissingPackages, SkeletalMeshLoadsLabelFormat,
                        SkeletalMeshLodDataLabelFormat,
                        NativeLodAuditCountFormat,
                        NativeLodAuditEntryFormat)),
                ContentAssets),
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FRTKComplianceAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        SourceFileSuffixes, AllowedBoundaryFragments,
                        SourceDirectoryName, SourceSearchPattern,
                        ForbiddenPatterns, ViolationMessageFormat,
                        SourceReadFailureFormat, StoreBoundaryLabel,
                        ViolationCountIncrement, CleanViolationCount)),
                RtkCompliance),
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FAutomationTextSettings>(
                    JSON_SETTINGS_ATOMS(SpecName, TestNames, GroupLabels,
                                        CaseLabels, AssertionLabels)),
                BotFunctionalCore, Pipeline),
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FConversationUITestAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        Labels, PlayerRole, PlayerText,
                        ExpectedPlayerMessageText,
                        ExpectedPlayerMessageGreen, History,
                        ExpectedHistoryCount, TaggedHistoryIndex,
                        ExpectedTaggedHistoryText, UntaggedHistoryIndex,
                        ExpectedUntaggedHistoryText, SubmittedInput,
                        ExpectedSubmittedText, SpeakerName, SpeakerRole,
                        ConversationPlayerLine, ConversationNpcReply,
                        ExpectedConversationTitle,
                        ExpectedConversationPlayerLine,
                        ExpectedConversationNpcReply)),
                ConversationUI),
            JSON_OBJECT_SETTING_FIELDS(
                FAutomationSettings,
                ReadSettingsWith<FProtocolLoopAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        Labels, RunEnvironmentVariable, SkipWarning, ApiUrl,
                        AgentPersona, ImmutablePersona, StatePersona,
                        StateJson, StateNeedle, AsyncPersona, AsyncPrompt,
                        BridgePersona, BridgeActionType, MinimumRuleCount)),
                ProtocolLoop)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FMarketingCaptureViewSettings, OutputName,
                       CameraOffset, TargetOffset, OrthoWidth, FieldOfView,
                       SpringArmLength, bHidePlayerMesh,
                       bUseActorRouteCenter);

template <>
struct TJsonSettingsRegistry<FMarketingCaptureSettings> {
  static const TArray<TJsonSettingsField<FMarketingCaptureSettings>>
      &Fields() {
    static const TArray<TJsonSettingsField<FMarketingCaptureSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                FMarketingCaptureSettings, CaptureCommandLineKey,
                QuitWhenDoneCommandLineKey, OutputDirectoryCommandLineKey,
                InitialDelayCommandLineKey, SettleSecondsCommandLineKey,
                BetweenSecondsCommandLineKey, DefaultOutputDirectory,
                ConsoleQuitCommand, ScreenshotLogFormat, InitialDelaySeconds,
                SettleSeconds, BetweenSeconds, MenuViewportWidth,
                MenuViewportHeight, MenuAnchorX, MenuAnchorY, MenuAlignmentX,
                MenuAlignmentY, MenuPositionX, MenuPositionY, MenuZOrder,
                MenuTitle, MenuRetakeButtonText, MenuResumeButtonText,
                MenuPanelPadding, MenuTitleSize, MenuButtonTextSize),
            JSON_OBJECT_SETTING_FIELDS(
                FMarketingCaptureSettings,
                SettingsAdapters::ReadLinearColorSettings,
                MenuPanelColor, MenuTitleColor, MenuButtonTextColor),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FMarketingCaptureSettings,
                ReadSettingsWith<FMarketingCaptureViewSettings>(
                    JSON_SETTINGS_ATOMS(OutputName, CameraOffset, TargetOffset,
                                        OrthoWidth, FieldOfView,
                                        SpringArmLength, bHidePlayerMesh,
                                        bUseActorRouteCenter)),
                CaptureViews)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FFlyModeSettings, EnabledMessage,
                       DisabledMessage, FlyingGravityScale,
                       VerticalInputScale, NeutralInputScale);

JSON_SETTINGS_REGISTRY(
    FScaleAuditCaptureSettings, CaptureCommandLineKey,
    QuitWhenDoneCommandLineKey, OutputDirectoryCommandLineKey,
    InitialDelayCommandLineKey, SettleSecondsCommandLineKey,
    BetweenSecondsCommandLineKey, WholeOrthoWidthCommandLineKey,
    TownOrthoWidthCommandLineKey, ActorsOrthoWidthCommandLineKey,
    WholeCaptureHeightCommandLineKey, TownCaptureHeightCommandLineKey,
    ActorsCaptureHeightCommandLineKey, DefaultOutputDirectory,
    WholeOutputName, TownOutputName, ActorsOutputName, InitialDelaySeconds,
    SettleSeconds, BetweenSeconds, TopDownRotation);

template <> struct TJsonSettingsRegistry<FUISettings> {
  static const TArray<TJsonSettingsField<FUISettings>> &Fields() {
    static const TArray<TJsonSettingsField<FUISettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FUISettings, PlayerRoleLabel,
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
                FUISettings,
                SettingsAdapters::ReadLinearColorSettings, PanelColor,
                TitleColor, PlayerColor, SystemColor, NpcColor, UnknownColor,
                ReplyColor),
            JSON_OBJECT_SETTING_FIELDS(
                FUISettings,
                ReadSettingsWith<FStatsOverlaySettings>(
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
                StatsOverlay),
            JSON_OBJECT_SETTING_FIELDS(
                FUISettings,
                ReadSettingsWith<FMarketingCaptureSettings>(
                    JSON_SETTINGS_ATOMS(
                        CaptureCommandLineKey, QuitWhenDoneCommandLineKey,
                        OutputDirectoryCommandLineKey,
                        InitialDelayCommandLineKey,
                        SettleSecondsCommandLineKey,
                        BetweenSecondsCommandLineKey, DefaultOutputDirectory,
                        ConsoleQuitCommand, ScreenshotLogFormat,
                        InitialDelaySeconds, SettleSeconds, BetweenSeconds,
                        MenuViewportWidth, MenuViewportHeight, MenuAnchorX,
                        MenuAnchorY, MenuAlignmentX, MenuAlignmentY,
                        MenuPositionX, MenuPositionY, MenuZOrder, MenuTitle,
                        MenuRetakeButtonText, MenuResumeButtonText,
                        MenuPanelPadding, MenuTitleSize, MenuButtonTextSize,
                        MenuPanelColor, MenuTitleColor, MenuButtonTextColor,
                        CaptureViews)),
                MarketingCapture),
            JSON_OBJECT_SETTING_FIELDS(
                FUISettings,
                ReadSettingsWith<FFlyModeSettings>(
                    JSON_SETTINGS_ATOMS(EnabledMessage, DisabledMessage,
                                        FlyingGravityScale,
                                        VerticalInputScale,
                                        NeutralInputScale)),
                FlyMode),
            JSON_OBJECT_SETTING_FIELDS(
                FUISettings,
                ReadSettingsWith<FScaleAuditCaptureSettings>(
                    JSON_SETTINGS_ATOMS(
                        CaptureCommandLineKey, QuitWhenDoneCommandLineKey,
                        OutputDirectoryCommandLineKey,
                        InitialDelayCommandLineKey,
                        SettleSecondsCommandLineKey,
                        BetweenSecondsCommandLineKey,
                        WholeOrthoWidthCommandLineKey,
                        TownOrthoWidthCommandLineKey,
                        ActorsOrthoWidthCommandLineKey,
                        WholeCaptureHeightCommandLineKey,
                        TownCaptureHeightCommandLineKey,
                        ActorsCaptureHeightCommandLineKey,
                        DefaultOutputDirectory, WholeOutputName,
                        TownOutputName, ActorsOutputName, InitialDelaySeconds,
                        SettleSeconds, BetweenSeconds, TopDownRotation)),
                ScaleAuditCapture)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FDialogueSettings, ReplyPayloadIdFormat);

JSON_SETTINGS_REGISTRY(FBotStatPresetSettings, MoveSpeed, AwarenessRange,
                       Resolve, bCanTalk);

template <> struct TJsonSettingsRegistry<FBotSettings> {
  static const TArray<TJsonSettingsField<FBotSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FBotSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FBotSettings, InitialName,
                                InitialHealth, InitialMaxHealth, InitialMana,
                                InitialMaxMana, InitialStamina,
                                InitialMaxStamina, MinimumHealth,
                                InitialPosition, InitialRotation,
                                InitialKnownPlayerPosition,
                                InitialTimeSinceSeenPlayer,
                                EnemySpottedTimeSinceSeenPlayer,
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
                FBotSettings,
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

JSON_SETTINGS_REGISTRY(FSpeechAutomationSettings, PhonemeEstimationText,
                       SilenceText, UnknownPhoneme, ActiveVisemeSampleRatio,
                       PastEndSampleRatio, ProducesPhonemesLabelFormat,
                       CorrectPhonemeCountLabel, FirstPhonemeStartLabel,
                       PositiveDurationLabelFormat, SilencePhonemeLabel,
                       VisemeMapCountLabel, VowelMappedLabelFormat,
                       ActiveVisemeLabelFormat, ActiveVisemeWeightLabelFormat,
                       PastEndSilenceLabel, KnownPhonemeLabelFormat,
                       UnknownPhonemeLabelFormat);

template <> struct TJsonSettingsRegistry<FSpeechSettings> {
  static const TArray<TJsonSettingsField<FSpeechSettings>> &Fields() {
    static const TArray<TJsonSettingsField<FSpeechSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(FSpeechSettings, RestViseme, RestWeight,
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
                FSpeechSettings,
                ReadSettingsWith<FSpeechVowelPhonemeSettings>(
                    JSON_SETTINGS_ATOMS(Character, Phoneme)),
                VowelPhonemes),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FSpeechVisemeMappingSettings>(
                    JSON_SETTINGS_ATOMS(Phoneme, MorphTargetName,
                                        BlendWeight)),
                VisemeMappings),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FSpeechPhonemeDurationRuleSettings>(
                    JSON_SETTINGS_ATOMS(Kind, Phoneme, Multiplier)),
                DurationRules),
            JSON_OBJECT_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FSpeechAutomationSettings>(
                    JSON_SETTINGS_ATOMS(
                        PhonemeEstimationText, SilenceText, UnknownPhoneme,
                        ActiveVisemeSampleRatio, PastEndSampleRatio,
                        ProducesPhonemesLabelFormat,
                        CorrectPhonemeCountLabel, FirstPhonemeStartLabel,
                        PositiveDurationLabelFormat, SilencePhonemeLabel,
                        VisemeMapCountLabel, VowelMappedLabelFormat,
                        ActiveVisemeLabelFormat,
                        ActiveVisemeWeightLabelFormat, PastEndSilenceLabel,
                        KnownPhonemeLabelFormat,
                        UnknownPhonemeLabelFormat)),
                Automation)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
