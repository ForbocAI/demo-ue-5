#pragma once

#include "Features/Components/Data/Settings/ForbocAI/Protocol/SettingsForbocAIProtocolTypes.h"
#include "Dom/JsonObject.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace ForbocAIProtocolAdapters {

/** User Story: As demo initialization, I need JSON-authored NPC and Ghost CLI workflows composed into one typed protocol setting. @fn FProtocolSettings ReadProtocolSettings(const TSharedPtr<FJsonObject> &Object) */
FProtocolSettings
ReadProtocolSettings(const TSharedPtr<FJsonObject> &Object);

} // namespace ForbocAIProtocolAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
