#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Rendering/Profile/Output/ProfileOutputTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(RenderingProfile::FAntiAliasing, AntiAliasingMethod,
                       PostProcessAAQuality, ScreenPercentage,
                       MinimumScreenPercentage);
JSON_SETTINGS_REGISTRY(RenderingProfile::FResolution, InternalRenderWidth,
                       InternalRenderHeight, OutputScaleMultiplier,
                       bFullscreenOutput);
JSON_SETTINGS_REGISTRY(RenderingProfile::FCommand,
                       RuntimeResolutionCommandFormat, FullscreenOutputSuffix,
                       WindowedOutputSuffix, ScreenPercentageCVarName,
                       RuntimeOutputLogFormat,
                       RuntimeOutputFormatBufferCharacterCount);
JSON_SETTINGS_CONCERN_REGISTRY(RenderingProfile::FOutput, AntiAliasing,
                               Resolution, Command);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
