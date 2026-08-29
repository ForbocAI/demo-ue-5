#pragma once

#include "Features/Components/Data/Settings/Registry/UI/Interface/InterfaceAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

#define FORBOCAI_UI_SETTING_FIELD(Group, GroupType, Field)                   \
  NestedSettingField(JSON_SETTING_ATOM(Field),                              \
                     NestedFieldMembers(&FUISettings::Group,                \
                                        &GroupType::Field))
#define FORBOCAI_UI_COLOR_FIELD(Group, GroupType, Field)                     \
  NestedObjectSettingField(                                                 \
      NestedFieldMembers(&FUISettings::Group, &GroupType::Field),           \
      SettingsAdapters::ReadLinearColorSettings)(JSON_SETTING_ATOM(Field))

template <> struct TJsonSettingsRegistry<FUISettings> {
  /** User Story: As a UI root registry consumer, I need to enumerate authored fields through a stable signature so UI concerns remain explicit and composable. @fn static const TArray<TField<FUISettings>> &Fields() */
  static const TArray<TField<FUISettings>> &Fields() {
    static const TArray<TField<FUISettings>> RegisteredFields = {
        FORBOCAI_UI_SETTING_FIELD(Roles, FRoleSettings, PlayerRoleLabel),
        FORBOCAI_UI_SETTING_FIELD(Roles, FRoleSettings, SystemRoleLabel),
        FORBOCAI_UI_SETTING_FIELD(Roles, FRoleSettings, NpcRoleLabel),
        FORBOCAI_UI_SETTING_FIELD(Roles, FRoleSettings, UnknownRoleLabel),
        FORBOCAI_UI_SETTING_FIELD(History, FHistorySettings,
                                  ChatMessageFormat),
        FORBOCAI_UI_SETTING_FIELD(History, FHistorySettings,
                                  HistoryRoleSeparator),
        FORBOCAI_UI_SETTING_FIELD(History, FHistorySettings,
                                  HistoryTextStartOffset),
        FORBOCAI_UI_SETTING_FIELD(History, FHistorySettings,
                                  HistoryMinimumRoleIndex),
        FORBOCAI_UI_SETTING_FIELD(Placeholder, FPlaceholderSettings,
                                  PlaceholderTitle),
        FORBOCAI_UI_SETTING_FIELD(Placeholder, FPlaceholderSettings,
                                  PlaceholderPlayerLine),
        FORBOCAI_UI_SETTING_FIELD(Placeholder, FPlaceholderSettings,
                                  PlaceholderNpcReply),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  ConversationTitleFormat),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  PlayerLineFormat),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  NpcReplyFormat),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  ConnectionMessageFormat),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  UnboundDialogueError),
        FORBOCAI_UI_SETTING_FIELD(Format, FConversationFormatSettings,
                                  PayloadIdFormat),
        FORBOCAI_UI_COLOR_FIELD(SurfaceColors, FSurfaceColorSettings,
                                PanelColor),
        FORBOCAI_UI_COLOR_FIELD(SurfaceColors, FSurfaceColorSettings,
                                TitleColor),
        FORBOCAI_UI_COLOR_FIELD(SurfaceColors, FSurfaceColorSettings,
                                ReplyColor),
        FORBOCAI_UI_COLOR_FIELD(RoleColors, FRoleColorSettings, PlayerColor),
        FORBOCAI_UI_COLOR_FIELD(RoleColors, FRoleColorSettings, SystemColor),
        FORBOCAI_UI_COLOR_FIELD(RoleColors, FRoleColorSettings, NpcColor),
        FORBOCAI_UI_COLOR_FIELD(RoleColors, FRoleColorSettings, UnknownColor),
        FORBOCAI_UI_SETTING_FIELD(ConversationLayout,
                                  FConversationLayoutSettings, PanelPadding),
        FORBOCAI_UI_SETTING_FIELD(ConversationLayout,
                                  FConversationLayoutSettings, TitleSize),
        FORBOCAI_UI_SETTING_FIELD(ConversationLayout,
                                  FConversationLayoutSettings, BodySize),
        JSON_OBJECT_SETTING_FIELDS(
            FUISettings,
            ReadSettingsWith<FOverlaySettings>(JSON_SETTINGS_ATOMS(
                FramesPerSecondLabel, StackDepthLabel, PolyCountLabel,
                UsedPhysicalMemoryLabel, PeakPhysicalMemoryLabel,
                UsedVirtualMemoryLabel, GameThreadMillisecondsLabel,
                RenderThreadMillisecondsLabel, RhiThreadMillisecondsLabel,
                GpuMillisecondsLabel, DrawCallsLabel, RhiPrimitivesLabel,
                WallDeltaMillisecondsLabel, InputDeltaMillisecondsLabel,
                StatsSelectionMillisecondsLabel, PolyCountMillisecondsLabel,
                MaxFpsLabel, EngineIdleMillisecondsLabel,
                EngineIdleOvershootMillisecondsLabel, FrameRateLimitLabel,
                EffectiveMaxTickRateLabel, FixedFrameRateEnabledLabel,
                FixedFrameRateLabel, FixedTimeStepEnabledLabel,
                FixedDeltaMillisecondsLabel, VsyncEnabledLabel,
                IdleWhenNotForegroundEnabledLabel, AppHasFocusLabel,
                CpuThrottleEnabledLabel, AllWindowsHiddenLabel,
                RootReducerMillisecondsLabel, CombinedReducerMillisecondsLabel,
                EcsProjectionMillisecondsLabel, ProjectedEntityCountLabel,
                ProjectedComponentTypeCountLabel, LabelValueSeparator,
                ValueFormat, DecimalValueFormat, DebugMessageFormat,
                BudgetLogFormat, FormatBufferCharacterCount, DebugMessageKey,
                DebugMessageDurationSeconds, ViewportLeft, ViewportTop,
                ViewportWidth, ViewportHeight, PanelPadding,
                StatsRefreshIntervalSeconds, PolyCountRefreshIntervalSeconds,
                BudgetLogIntervalSeconds, IntervalResetElapsedSeconds,
                BudgetScreenshotIntervalSeconds,
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
                DiagnosticDefaultFloatValue, MeshLodIndex,
                ForcedLodAutomaticModel, LodModelIndexOffset,
                ProcMeshFirstSectionIndex, ProcMeshSectionStep,
                TriangleIndexDivisor, ZOrder, FontSize, bRemoveDpIScale,
                bAutoWrapText, bBudgetScreenshotCreateDirectoryTree,
                bBudgetScreenshotShowUI, bBudgetScreenshotAddFilenameSuffix,
                bDiagnosticAllowFrameRateSmoothing,
                FramesPerSecondMediumThreshold, FramesPerSecondHighThreshold,
                StackDepthMediumThreshold, StackDepthHighThreshold,
                PolyCountMediumThreshold, PolyCountHighThreshold,
                MemoryMediumThreshold, MemoryHighThreshold, PanelColor,
                TextColor, LowValueColor, MediumValueColor, HighValueColor)),
            StatsOverlay),
        JSON_OBJECT_SETTING_FIELDS(
            FUISettings,
            ReadSettingsWith<FMarketingCaptureSettings>(JSON_SETTINGS_ATOMS(
                CaptureCommandLineKey, QuitWhenDoneCommandLineKey,
                OutputDirectoryCommandLineKey, InitialDelayCommandLineKey,
                SettleSecondsCommandLineKey, BetweenSecondsCommandLineKey,
                DefaultOutputDirectory, ConsoleQuitCommand,
                ScreenshotLogFormat, InitialDelaySeconds, SettleSeconds,
                BetweenSeconds, MenuViewportWidth, MenuViewportHeight,
                MenuAnchorX, MenuAnchorY, MenuAlignmentX, MenuAlignmentY,
                MenuPositionX, MenuPositionY, MenuZOrder, MenuTitle,
                MenuRetakeButtonText, MenuResumeButtonText, MenuPanelPadding,
                MenuTitleSize, MenuButtonTextSize, MenuPanelColor,
                MenuTitleColor, MenuButtonTextColor, CaptureViews)),
            MarketingCapture),
        JSON_OBJECT_SETTING_FIELDS(
            FUISettings,
            ReadSettingsWith<FModeSettings>(JSON_SETTINGS_ATOMS(
                EnabledMessage, DisabledMessage, FlyingGravityScale,
                VerticalInputScale, NeutralInputScale)),
            FlyMode),
        JSON_OBJECT_SETTING_FIELDS(
            FUISettings,
            ReadSettingsWith<FAuditCaptureSettings>(JSON_SETTINGS_ATOMS(
                CaptureCommandLineKey, QuitWhenDoneCommandLineKey,
                OutputDirectoryCommandLineKey, DefaultOutputDirectory,
                TopDownRotation, Timing, Whole, Town, Actors)),
            ScaleAuditCapture)};
    return RegisteredFields;
  }
};

#undef FORBOCAI_UI_COLOR_FIELD
#undef FORBOCAI_UI_SETTING_FIELD

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
