#include "Features/Components/Data/Settings/ForbocAI/Protocol/SettingsForbocAIProtocolAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Data/Settings/Registry/ForbocAI/Protocol/RegistryForbocAIProtocolAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace ForbocAIProtocolAdapters {
namespace Json = JsonAdapters;
namespace {

FForbocAICommandSettings
ReadCommand(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsWith<FForbocAICommandSettings>(
      JSON_SETTINGS_ATOMS(Family, KeyPrefix, Tokens))(Object);
}

FForbocAIFlowPresentationSettings
ReadFlowPresentation(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsWith<FForbocAIFlowPresentationSettings>(
      JSON_SETTINGS_ATOMS(Pending, Succeeded, FailurePrefix))(Object);
}

FLogSettings
ReadProtocolLogging(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadStringField(Object, "StateFormat"),
      Json::ReadStringField(Object, "Present"),
      Json::ReadStringField(Object, "Absent"),
      Json::ReadSettingsWith<FForbocAIFlowStatusTextSettings>(
          JSON_SETTINGS_ATOMS(Idle, Pending, Succeeded, Failed))(
          Json::ReadObjectField(Object, "Statuses"))};
}

FForbocAINpcCommandSettings
ReadNpcCommands(const TSharedPtr<FJsonObject> &Object) {
  return {ReadCommand(Json::ReadObjectField(Object, "Create")),
          ReadCommand(Json::ReadObjectField(Object, "Process"))};
}

FForbocAINpcPresentationSettings
ReadNpcPresentation(const TSharedPtr<FJsonObject> &Object) {
  const FForbocAIFlowPresentationSettings Common =
      ReadFlowPresentation(Object);
  FForbocAINpcPresentationSettings Settings;
  Settings.Pending = Common.Pending;
  Settings.Succeeded = Common.Succeeded;
  Settings.FailurePrefix = Common.FailurePrefix;
  Settings.ActionLinePrefix = Json::ReadStringField(Object, "ActionLinePrefix");
  return Settings;
}

FForbocAINpcSettings
ReadNpc(const TSharedPtr<FJsonObject> &Object) {
  return {ReadNpcCommands(Json::ReadObjectField(Object, "Commands")),
          ReadNpcPresentation(Json::ReadObjectField(Object, "Presentation"))};
}

FForbocAIGhostCommandSettings
ReadGhostCommands(const TSharedPtr<FJsonObject> &Object) {
  return {ReadCommand(Json::ReadObjectField(Object, "Run")),
          ReadCommand(Json::ReadObjectField(Object, "Create")),
          ReadCommand(Json::ReadObjectField(Object, "Decide")),
          ReadCommand(Json::ReadObjectField(Object, "Results")),
          ReadCommand(Json::ReadObjectField(Object, "Stop"))};
}

FForbocAIGhostRunRequestSettings
ReadGhostRunRequest(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsWith<FForbocAIGhostRunRequestSettings>(
      JSON_SETTINGS_ATOMS(Suite, DurationSeconds, DurationOption))(Object);
}

FForbocAIGhostActorRequestSettings
ReadGhostActorRequest(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsWith<FForbocAIGhostActorRequestSettings>(
      JSON_SETTINGS_ATOMS(Persona))(Object);
}

FForbocAIGhostDecisionRequestSettings
ReadGhostDecisionRequest(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsWith<FForbocAIGhostDecisionRequestSettings>(
      JSON_SETTINGS_ATOMS(ObservationFormat, LegalActionsOption,
                          LegalActionsSeparator, LegalActions))(Object);
}

FForbocAIGhostRequestSettings
ReadGhostRequest(const TSharedPtr<FJsonObject> &Object) {
  return {ReadGhostRunRequest(Json::ReadObjectField(Object, "Run")),
          ReadGhostActorRequest(Json::ReadObjectField(Object, "Actor")),
          ReadGhostDecisionRequest(
              Json::ReadObjectField(Object, "Decision"))};
}

FForbocAIGhostSettings
ReadGhost(const TSharedPtr<FJsonObject> &Object) {
  return {ReadGhostCommands(Json::ReadObjectField(Object, "Commands")),
          ReadGhostRequest(Json::ReadObjectField(Object, "Request")),
          ReadFlowPresentation(Json::ReadObjectField(Object, "Presentation"))};
}

} // namespace

FProtocolSettings
ReadProtocolSettings(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadSettingsWith<FForbocAIErrorSettings>(
          JSON_SETTINGS_ATOMS(CommandUnmatched, RouteFamilyMismatchFormat,
                              ActionMissing))(
          Json::ReadObjectField(Object, "Errors")),
      ReadProtocolLogging(Json::ReadObjectField(Object, "Logging")),
      ReadNpc(Json::ReadObjectField(Object, "Npc")),
      ReadGhost(Json::ReadObjectField(Object, "Ghost"))};
}

} // namespace ForbocAIProtocolAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
