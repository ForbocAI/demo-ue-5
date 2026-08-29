#pragma once

#include "Features/Components/Data/Json/Settings/Registry/SettingsRegistryAdapters.h"
#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"
#include "Features/Components/Data/Settings/UI/Stats/UIStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FPresentationFormatSettings, LabelValueSeparator,
                       ValueFormat, DecimalValueFormat, DebugMessageFormat,
                       BudgetLogFormat, FormatBufferCharacterCount);
JSON_SETTINGS_REGISTRY(FPresentationMessageSettings, DebugMessageKey,
                       DebugMessageDurationSeconds);
JSON_SETTINGS_REGISTRY(FPresentationViewportSettings, ViewportLeft,
                       ViewportTop, ViewportWidth, ViewportHeight, PanelPadding,
                       ZOrder);
JSON_SETTINGS_REGISTRY(FPresentationTypographySettings, FontSize,
                       bRemoveDpIScale, bAutoWrapText);
JSON_OBJECT_SETTINGS_REGISTRY(FPresentationColorSettings,
                              SettingsAdapters::ReadLinearColorSettings,
                              PanelColor, TextColor, LowValueColor,
                              MediumValueColor, HighValueColor);
JSON_SETTINGS_CONCERN_REGISTRY(FStatsPresentationSettings, Format, Message,
                               Viewport, Typography, Colors);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
