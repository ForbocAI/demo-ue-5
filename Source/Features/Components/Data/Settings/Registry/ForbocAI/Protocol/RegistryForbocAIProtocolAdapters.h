#pragma once

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/SettingsForbocAIProtocolTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FForbocAICommandSettings, Family, KeyPrefix, Tokens);
JSON_SETTINGS_REGISTRY(FForbocAIErrorSettings, CommandUnmatched,
                       RouteFamilyMismatchFormat, ActionMissing);
JSON_SETTINGS_REGISTRY(FForbocAIFlowStatusTextSettings, Idle, Pending,
                       Succeeded, Failed);
JSON_SETTINGS_REGISTRY(FLogSettings, StateFormat, Present,
                       Absent, Statuses);
JSON_SETTINGS_REGISTRY(FForbocAIFlowPresentationSettings, Pending, Succeeded,
                       FailurePrefix);
JSON_SETTINGS_REGISTRY(FForbocAIGhostRunRequestSettings, Suite,
                       DurationSeconds, DurationOption);
JSON_SETTINGS_REGISTRY(FForbocAIGhostActorRequestSettings, Persona);
JSON_SETTINGS_REGISTRY(FForbocAIGhostDecisionRequestSettings,
                       ObservationFormat, LegalActionsOption,
                       LegalActionsSeparator, LegalActions);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
