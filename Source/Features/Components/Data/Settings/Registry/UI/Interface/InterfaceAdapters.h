#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"
#include "Features/Components/Data/Settings/Registry/UI/Stats/UIStatsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <> struct TJsonSettingsRegistry<FCaptureViewSettings> {
  /** User Story: As a registry ui interface consumer, I need to invoke fields through a stable signature so the registry ui interface workflow remains explicit and composable. @fn static const TArray<TField<FCaptureViewSettings>> &Fields() */
  static const TArray<TField<FCaptureViewSettings>> &Fields() {
    static const TArray<TField<FCaptureViewSettings>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(OutputName),
                NestedFieldMembers(&FCaptureViewSettings::Output,
                                   &FCaptureOutputSettings::OutputName)),
            NestedSettingField(
                JSON_SETTING_ATOM(CameraOffset),
                NestedFieldMembers(&FCaptureViewSettings::Camera,
                                   &FCaptureCameraSettings::CameraOffset)),
            NestedSettingField(
                JSON_SETTING_ATOM(TargetOffset),
                NestedFieldMembers(&FCaptureViewSettings::Camera,
                                   &FCaptureCameraSettings::TargetOffset)),
            NestedSettingField(
                JSON_SETTING_ATOM(OrthoWidth),
                NestedFieldMembers(&FCaptureViewSettings::Camera,
                                   &FCaptureCameraSettings::OrthoWidth)),
            NestedSettingField(
                JSON_SETTING_ATOM(FieldOfView),
                NestedFieldMembers(&FCaptureViewSettings::Camera,
                                   &FCaptureCameraSettings::FieldOfView)),
            NestedSettingField(
                JSON_SETTING_ATOM(SpringArmLength),
                NestedFieldMembers(&FCaptureViewSettings::Camera,
                                   &FCaptureCameraSettings::SpringArmLength)),
            NestedSettingField(
                JSON_SETTING_ATOM(bHidePlayerMesh),
                NestedFieldMembers(
                    &FCaptureViewSettings::Visibility,
                    &FCaptureVisibilitySettings::bHidePlayerMesh)),
            NestedSettingField(
                JSON_SETTING_ATOM(bUseActorRouteCenter),
                NestedFieldMembers(
                    &FCaptureViewSettings::Visibility,
                    &FCaptureVisibilitySettings::bUseActorRouteCenter))};
    return RegisteredFields;
  }
};

#define FORBOCAI_MARKETING_SETTING_FIELD(Group, GroupType, Field)            \
  NestedSettingField(                                                       \
      JSON_SETTING_ATOM(Field),                                             \
      NestedFieldMembers(&FMarketingCaptureSettings::Group,                 \
                         &GroupType::Field))
#define FORBOCAI_MARKETING_COLOR_FIELD(Field)                               \
  NestedObjectSettingField(                                                 \
      JSON_SETTING_ATOM(Field),                                             \
      NestedFieldMembers(&FMarketingCaptureSettings::Colors,                \
                         &FMenuColorSettings::Field),                       \
      SettingsAdapters::ReadLinearColorSettings)

template <> struct TJsonSettingsRegistry<FMarketingCaptureSettings> {
  /** User Story: As a registry ui interface consumer, I need to invoke fields through a stable signature so the registry ui interface workflow remains explicit and composable. @fn static const TArray<TField<FMarketingCaptureSettings>> &Fields() */
  static const TArray<TField<FMarketingCaptureSettings>>
      &Fields() {
    static const TArray<TField<FMarketingCaptureSettings>>
        RegisteredFields = {
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, CaptureCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, QuitWhenDoneCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, OutputDirectoryCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, DefaultOutputDirectory),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, ConsoleQuitCommand),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Command, FCommandSettings, ScreenshotLogFormat),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, InitialDelayCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, SettleSecondsCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, BetweenSecondsCommandLineKey),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, InitialDelaySeconds),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, SettleSeconds),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Timing, FTiming, BetweenSeconds),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Viewport, FMenuViewportSettings, MenuViewportWidth),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Viewport, FMenuViewportSettings, MenuViewportHeight),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Viewport, FMenuViewportSettings, MenuZOrder),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuAnchorX),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuAnchorY),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuAlignmentX),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuAlignmentY),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuPositionX),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Placement, FPlacementSettings, MenuPositionY),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Content, FContentSettings, MenuTitle),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Content, FContentSettings, MenuRetakeButtonText),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Content, FContentSettings, MenuResumeButtonText),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Typography, FMenuTypographySettings, MenuPanelPadding),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Typography, FMenuTypographySettings, MenuTitleSize),
            FORBOCAI_MARKETING_SETTING_FIELD(
                Typography, FMenuTypographySettings, MenuButtonTextSize),
            FORBOCAI_MARKETING_COLOR_FIELD(MenuPanelColor),
            FORBOCAI_MARKETING_COLOR_FIELD(MenuTitleColor),
            FORBOCAI_MARKETING_COLOR_FIELD(MenuButtonTextColor),
            JSON_OBJECT_ARRAY_SETTING_FIELDS(
                FMarketingCaptureSettings,
                ReadSettingsWith<FCaptureViewSettings>(
                    JSON_SETTINGS_ATOMS(OutputName, CameraOffset, TargetOffset,
                                        OrthoWidth, FieldOfView,
                                        SpringArmLength, bHidePlayerMesh,
                                        bUseActorRouteCenter)),
                CaptureViews)};
    return RegisteredFields;
  }
};

#undef FORBOCAI_MARKETING_COLOR_FIELD
#undef FORBOCAI_MARKETING_SETTING_FIELD

JSON_SETTINGS_REGISTRY(FModeSettings, EnabledMessage,
                       DisabledMessage, FlyingGravityScale,
                       VerticalInputScale, NeutralInputScale);

JSON_SETTINGS_REGISTRY(FRegionCapture, OrthoWidthCommandLineKey,
                       CaptureHeightCommandLineKey, OutputName);

JSON_SETTINGS_REGISTRY(FTiming, InitialDelayCommandLineKey,
                       SettleSecondsCommandLineKey,
                       BetweenSecondsCommandLineKey, InitialDelaySeconds,
                       SettleSeconds, BetweenSeconds);

template <> struct TJsonSettingsRegistry<FAuditCaptureSettings> {
  /** User Story: As a registry ui interface consumer, I need to invoke fields through a stable signature so the registry ui interface workflow remains explicit and composable. @fn static const TArray<TField<FAuditCaptureSettings>> &Fields() */
  static const TArray<TField<FAuditCaptureSettings>> &Fields() {
    static const TArray<TField<FAuditCaptureSettings>> RegisteredFields = {
        JSON_SETTING_FIELDS(FAuditCaptureSettings, CaptureCommandLineKey,
                            QuitWhenDoneCommandLineKey,
                            OutputDirectoryCommandLineKey,
                            DefaultOutputDirectory, TopDownRotation),
        JSON_OBJECT_SETTING_FIELDS(
            FAuditCaptureSettings,
            ReadSettingsWith<FTiming>(JSON_SETTINGS_ATOMS(
                InitialDelayCommandLineKey, SettleSecondsCommandLineKey,
                BetweenSecondsCommandLineKey, InitialDelaySeconds,
                SettleSeconds, BetweenSeconds)),
            Timing),
        JSON_OBJECT_SETTING_FIELDS(
            FAuditCaptureSettings,
            ReadSettingsWith<FRegionCapture>(JSON_SETTINGS_ATOMS(
                OrthoWidthCommandLineKey, CaptureHeightCommandLineKey,
                OutputName)),
            Whole, Town, Actors)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
