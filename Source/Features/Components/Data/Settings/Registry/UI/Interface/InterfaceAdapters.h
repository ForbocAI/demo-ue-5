#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/UIStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

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
                        DebugMessageFormat, BudgetLogFormat,
                        FormatBufferCharacterCount,
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

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
