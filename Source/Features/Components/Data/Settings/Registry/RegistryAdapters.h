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

JSON_SETTINGS_REGISTRY(FEcsDomainRegistrationSettings, Path, Kind);

template <> struct TJsonSettingsRegistry<FEcsSettings> {
  static const TArray<TField<FEcsSettings>> &Fields() {
    static const TArray<TField<FEcsSettings>> RegisteredFields = {
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FEcsSettings,
            ReadSettingsWith<FEcsDomainRegistrationSettings>(
                JSON_SETTINGS_ATOMS(Path, Kind)),
            DomainRegistry)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Store::FSettings,
                       DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                       ProjectionGateLabels, ReduxLoggerCategory,
                       ReduxLoggerActionTitlePrefix, TerrainEntity,
                       TerrainProjectionDomain);

JSON_SETTINGS_REGISTRY(Automation::Content::Assets::FPackage, Label, Path);

JSON_SETTINGS_REGISTRY(Automation::Content::Assets::FConfig, Label, Section,
                       Key, Expected);

JSON_SETTINGS_REGISTRY(Automation::Rtk::Compliance::FPattern, Token, Message);

template <>
struct TJsonSettingsRegistry<Automation::Content::Assets::FSettings> {
  static const TArray<TField<Automation::Content::Assets::FSettings>>
      &Fields() {
    static const TArray<
        TField<Automation::Content::Assets::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Content::Assets::FSettings,
                                SkeletalMeshLoadsLabelFormat,
                                SkeletalMeshLodDataLabelFormat,
                                NativeLodAuditCountFormat,
                                NativeLodAuditEntryFormat),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Content::Assets::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FPackage>(
                    JSON_SETTINGS_ATOMS(Label, Path)),
                Packages, Classes, SpeechComponentClasses, Assets,
                SkeletalMeshLods, MissingPackages),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Content::Assets::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FConfig>(
                    JSON_SETTINGS_ATOMS(Label, Section, Key, Expected)),
                ConfigValues)};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Rtk::Compliance::FSettings> {
  static const TArray<TField<Automation::Rtk::Compliance::FSettings>>
      &Fields() {
    static const TArray<
        TField<Automation::Rtk::Compliance::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(
                Automation::Rtk::Compliance::FSettings, SourceFileSuffixes,
                AllowedBoundaryFragments, SourceDirectoryName,
                SourceSearchPattern, ViolationMessageFormat,
                SourceReadFailureFormat, StoreBoundaryLabel,
                ViolationCountIncrement, CleanViolationCount),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                Automation::Rtk::Compliance::FSettings,
                ReadSettingsWith<Automation::Rtk::Compliance::FPattern>(
                    JSON_SETTINGS_ATOMS(Token, Message)),
                ForbiddenPatterns)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Bot::FGroups,
                       StateCreation, Reducers, Movement, Combat, Awareness,
                       TickUpdate);

JSON_SETTINGS_REGISTRY(Automation::Bot::FCases,
                       CreateInitialState, UpdatePosition, ReduceHealth,
                       TransitionToCombat, TransitionToFlee, UpdateMemory,
                       IncrementTick, DecayAggro);

template <>
struct TJsonSettingsRegistry<Automation::Bot::FSettings> {
  static const TArray<TField<Automation::Bot::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Bot::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Bot::FSettings,
                                Spec, Tests, Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FGroups>(
                    JSON_SETTINGS_ATOMS(StateCreation, Reducers, Movement,
                                        Combat, Awareness, TickUpdate)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Bot::FSettings,
                ReadSettingsWith<Automation::Bot::FCases>(
                    JSON_SETTINGS_ATOMS(CreateInitialState, UpdatePosition,
                                        ReduceHealth, TransitionToCombat,
                                        TransitionToFlee, UpdateMemory,
                                        IncrementTick, DecayAggro)),
                Cases)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FTests, IdleTickAdvancesState,
                       HazardCausesDamage, AwarenessTriggersAggro,
                       FleeTransitionOnLowHealth, MultiBotIndependent,
                       DeterministicOrder);

JSON_SETTINGS_REGISTRY(Automation::Pipeline::FAssertions, TickCountAdvanced,
                       ActionDispatched, HealthUnchanged,
                       HealthReducedByHazard, HealthAfterHazard, BotHasAggro,
                       PhaseIsCombat, RemembersEnemyPosition,
                       PhaseTransitionedToFlee, BotsProcessed,
                       IdleBotFullHealth, HazardBotTookDamage,
                       AwareBotHasAggro, HealthDeterministic,
                       PositionDeterministic, PhaseDeterministic,
                       AggroDeterministic, ActionCountDeterministic);

template <> struct TJsonSettingsRegistry<Automation::Pipeline::FSettings> {
  static const TArray<TField<Automation::Pipeline::FSettings>> &Fields() {
    static const TArray<TField<Automation::Pipeline::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Pipeline::FSettings, Spec, Groups,
                                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Pipeline::FSettings,
                ReadSettingsWith<Automation::Pipeline::FTests>(
                    JSON_SETTINGS_ATOMS(
                        IdleTickAdvancesState, HazardCausesDamage,
                        AwarenessTriggersAggro, FleeTransitionOnLowHealth,
                        MultiBotIndependent, DeterministicOrder)),
                Tests),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Pipeline::FSettings,
                ReadSettingsWith<Automation::Pipeline::FAssertions>(
                    JSON_SETTINGS_ATOMS(
                        TickCountAdvanced, ActionDispatched, HealthUnchanged,
                        HealthReducedByHazard, HealthAfterHazard, BotHasAggro,
                        PhaseIsCombat, RemembersEnemyPosition,
                        PhaseTransitionedToFlee, BotsProcessed,
                        IdleBotFullHealth, HazardBotTookDamage,
                        AwareBotHasAggro, HealthDeterministic,
                        PositionDeterministic, PhaseDeterministic,
                        AggroDeterministic, ActionCountDeterministic)),
                Assertions)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FMessage, Role, Text,
                       ExpectedText, ExpectedGreen);

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FLine, Index,
                       ExpectedText);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FHistory> {
  static const TArray<TField<Automation::Conversation::UI::FHistory>>
      &Fields() {
    static const TArray<TField<
        Automation::Conversation::UI::FHistory>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FHistory, Lines,
                                ExpectedCount),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FHistory,
                ReadSettingsWith<Automation::Conversation::UI::FLine>(
                    JSON_SETTINGS_ATOMS(Index, ExpectedText)),
                Tagged, Untagged)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FSubmitted, Input,
                       ExpectedText);

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FDialogue, SpeakerName,
                       SpeakerRole, PlayerLine, NpcReply, ExpectedTitle,
                       ExpectedPlayerLine, ExpectedNpcReply);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FSettings> {
  static const TArray<TField<Automation::Conversation::UI::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Conversation::UI::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FSettings, Test,
                                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FMessage>(
                    JSON_SETTINGS_ATOMS(Role, Text, ExpectedText,
                                        ExpectedGreen)),
                PlayerMessage),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FHistory>(
                    JSON_SETTINGS_ATOMS(Lines, ExpectedCount, Tagged,
                                        Untagged)),
                History),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FSubmitted>(
                    JSON_SETTINGS_ATOMS(Input, ExpectedText)),
                Submitted),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FDialogue>(
                    JSON_SETTINGS_ATOMS(
                        SpeakerName, SpeakerRole, PlayerLine, NpcReply,
                        ExpectedTitle, ExpectedPlayerLine, ExpectedNpcReply)),
                Dialogue)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FGate, Variable, Warning);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FConnection, Url);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FPersonas, Agent, Immutable,
                       State, Async, Bridge);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FState, Json, Needle);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FAsync, Prompt);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FBridge, Action,
                       MinimumRules);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FGroups, AgentCreation,
                       StateUpdates, AsyncProcessPipeline, BridgeValidation);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FCases, CreateAgent,
                       CreateImmutableAgent, WithStateUpdate, InvokeProcess,
                       CreateRpgRules, ValidateRpgAction);

JSON_SETTINGS_REGISTRY(Automation::Protocol::Loop::FAssertions,
                       AgentIdNotEmpty, PersonaMatches, AgentPointerValid,
                       PersonaPreserved, OriginalIdPreserved,
                       UpdatedStateContainsMood, RpgRulesNotEmpty,
                       MoveActionValid);

template <> struct TJsonSettingsRegistry<Automation::Protocol::Loop::FSettings> {
  static const TArray<TField<Automation::Protocol::Loop::FSettings>>
      &Fields() {
    static const TArray<TField<
        Automation::Protocol::Loop::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Protocol::Loop::FSettings, Spec),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FGroups>(
                    JSON_SETTINGS_ATOMS(AgentCreation, StateUpdates,
                                        AsyncProcessPipeline,
                                        BridgeValidation)),
                Groups),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FCases>(
                    JSON_SETTINGS_ATOMS(CreateAgent, CreateImmutableAgent,
                                        WithStateUpdate, InvokeProcess,
                                        CreateRpgRules, ValidateRpgAction)),
                Cases),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAssertions>(
                    JSON_SETTINGS_ATOMS(
                        AgentIdNotEmpty, PersonaMatches, AgentPointerValid,
                        PersonaPreserved, OriginalIdPreserved,
                        UpdatedStateContainsMood, RpgRulesNotEmpty,
                        MoveActionValid)),
                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FGate>(
                    JSON_SETTINGS_ATOMS(Variable, Warning)),
                Gate),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FConnection>(
                    JSON_SETTINGS_ATOMS(Url)),
                Connection),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FPersonas>(
                    JSON_SETTINGS_ATOMS(Agent, Immutable, State, Async,
                                        Bridge)),
                Personas),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FState>(
                    JSON_SETTINGS_ATOMS(Json, Needle)),
                State),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FAsync>(
                    JSON_SETTINGS_ATOMS(Prompt)),
                Async),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Protocol::Loop::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FBridge>(
                    JSON_SETTINGS_ATOMS(Action, MinimumRules)),
                Bridge)};
    return RegisteredFields;
  }
};

template <> struct TJsonSettingsRegistry<Automation::FSettings> {
  static const TArray<TField<Automation::FSettings>> &Fields() {
    static const TArray<TField<Automation::FSettings>>
        RegisteredFields = {
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Store::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        DataBackedMapLabels, ReduxLoggerMiddlewareLabels,
                        ProjectionGateLabels, ReduxLoggerCategory,
                        ReduxLoggerActionTitlePrefix, TerrainEntity,
                        TerrainProjectionDomain)),
                Store),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Content::Assets::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        Packages, Classes, SpeechComponentClasses,
                        ConfigValues, Assets, SkeletalMeshLods,
                        MissingPackages, SkeletalMeshLoadsLabelFormat,
                        SkeletalMeshLodDataLabelFormat,
                        NativeLodAuditCountFormat,
                        NativeLodAuditEntryFormat)),
                ContentAssets),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Rtk::Compliance::FSettings>(
                    JSON_SETTINGS_ATOMS(
                        SourceFileSuffixes, AllowedBoundaryFragments,
                        SourceDirectoryName, SourceSearchPattern,
                        ForbiddenPatterns, ViolationMessageFormat,
                        SourceReadFailureFormat, StoreBoundaryLabel,
                        ViolationCountIncrement, CleanViolationCount)),
                RtkCompliance),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Bot::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Tests, Groups, Cases,
                                        Assertions)),
                Bot),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Pipeline::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Tests, Groups, Cases,
                                        Assertions)),
                Pipeline),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FSettings>(
                    JSON_SETTINGS_ATOMS(Test, Assertions, PlayerMessage,
                                        History, Submitted, Dialogue)),
                ConversationUI),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::FSettings,
                ReadSettingsWith<Automation::Protocol::Loop::FSettings>(
                    JSON_SETTINGS_ATOMS(Spec, Groups, Cases, Assertions, Gate,
                                        Connection, Personas, State, Async,
                                        Bridge)),
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
  static const TArray<TField<FMarketingCaptureSettings>>
      &Fields() {
    static const TArray<TField<FMarketingCaptureSettings>>
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
  static const TArray<TField<FUISettings>> &Fields() {
    static const TArray<TField<FUISettings>>
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

JSON_SETTINGS_REGISTRY(FStatPresetSettings, MoveSpeed, AwarenessRange,
                       Resolve, bCanTalk);

template <> struct TJsonSettingsRegistry<FBotSettings> {
  static const TArray<TField<FBotSettings>> &Fields() {
    static const TArray<TField<FBotSettings>>
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
                ReadSettingsWith<FStatPresetSettings>(
                    JSON_SETTINGS_ATOMS(MoveSpeed, AwarenessRange, Resolve,
                                        bCanTalk)),
                TownspersonStats, HorseStats)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(FVisemeMappingSettings, Phoneme,
                       MorphTargetName, BlendWeight);

JSON_SETTINGS_REGISTRY(FVowelPhonemeSettings, Character, Phoneme);

JSON_SETTINGS_REGISTRY(FPhonemeDurationRuleSettings, Kind, Phoneme,
                       Multiplier);

JSON_SETTINGS_REGISTRY(FAutomationSettings, PhonemeEstimationText,
                       SilenceText, UnknownPhoneme, ActiveVisemeSampleRatio,
                       PastEndSampleRatio, ProducesPhonemesLabelFormat,
                       CorrectPhonemeCountLabel, FirstPhonemeStartLabel,
                       PositiveDurationLabelFormat, SilencePhonemeLabel,
                       VisemeMapCountLabel, VowelMappedLabelFormat,
                       ActiveVisemeLabelFormat, ActiveVisemeWeightLabelFormat,
                       PastEndSilenceLabel, KnownPhonemeLabelFormat,
                       UnknownPhonemeLabelFormat);

template <> struct TJsonSettingsRegistry<FSpeechSettings> {
  static const TArray<TField<FSpeechSettings>> &Fields() {
    static const TArray<TField<FSpeechSettings>>
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
                ReadSettingsWith<FVowelPhonemeSettings>(
                    JSON_SETTINGS_ATOMS(Character, Phoneme)),
                VowelPhonemes),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FVisemeMappingSettings>(
                    JSON_SETTINGS_ATOMS(Phoneme, MorphTargetName,
                                        BlendWeight)),
                VisemeMappings),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FPhonemeDurationRuleSettings>(
                    JSON_SETTINGS_ATOMS(Kind, Phoneme, Multiplier)),
                DurationRules),
            JSON_OBJECT_SETTING_FIELDS(
                FSpeechSettings,
                ReadSettingsWith<FAutomationSettings>(
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
