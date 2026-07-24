// Typed readers for bot-authored settings.
#include "Features/Components/Data/Settings/Bot/SettingsBotAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FTownspersonDefaultsSettings, Id, Name, Role, Persona,
                       InteractionPrompt, DefaultPlayerLine);

JSON_SETTINGS_REGISTRY(FCharacterGeometrySettings, CharacterHeightFeet,
                       CharacterShoulderWidthFeet);
JSON_SETTINGS_REGISTRY(FTownspersonMotionSettings, PatrolPauseSeconds,
                       InteractionRadiusLots, WalkSpeedHeightRatio);
JSON_SETTINGS_REGISTRY(FTextPlacementSettings,
                       PromptAboveHeadFeet, NameAbovePromptFeet,
                       DialogueAboveNameHeightRatio, PromptTextScale,
                       DialogueTextScale);
JSON_SETTINGS_REGISTRY(FMannequinSettings, MannequinScale,
                       MannequinOffsetFeet, MannequinRotation, MeshPath,
                       AnimationBlueprintClassPath);

JSON_SETTINGS_REGISTRY(FBodySettings, HorseLengthFeet, BodyHeightFeet,
                       LegHeightFeet, NeckHeightFeet, HeadHeightFeet,
                       SaddleHeightFeet);
JSON_SETTINGS_REGISTRY(FHorseMotionSettings, PatrolPauseSeconds,
                       WalkSpeedHorseLengthRatio, RouteArrivalLegRatio);
JSON_SETTINGS_REGISTRY(FRiderSettings, ImportedHorseScale,
                       MountedRiderScale, MountedRiderOffsetFeet);
JSON_SETTINGS_REGISTRY(FNameSettings, DefaultName,
                       NameTextWorldSizeFeet);
JSON_SETTINGS_REGISTRY(FAssetsSettings, HorseMeshPath,
                       HorseWalkAnimationPath, RiderMeshPath,
                       RiderWalkAnimationPath);

} // namespace JsonAdapters
namespace BotAdapters {
namespace Json = JsonAdapters;

/** User Story: As a data settings bot consumer, I need to invoke read townsperson defaults settings through a stable signature so the data settings bot workflow remains explicit and composable. @fn FTownspersonDefaultsSettings ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) */
FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTownspersonDefaultsSettings>(
      Object, JSON_SETTINGS_ATOMS(Id, Name, Role, Persona, InteractionPrompt,
                                  DefaultPlayerLine));
}

/** User Story: As a data settings bot consumer, I need to invoke read townsperson presentation settings through a stable signature so the data settings bot workflow remains explicit and composable. @fn FTownspersonPresentationSettings ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) */
FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadSettingsFields<FCharacterGeometrySettings>(
          Object, JSON_SETTINGS_ATOMS(CharacterHeightFeet,
                                      CharacterShoulderWidthFeet)),
      Json::ReadSettingsFields<FTownspersonMotionSettings>(
          Object, JSON_SETTINGS_ATOMS(PatrolPauseSeconds,
                                      InteractionRadiusLots,
                                      WalkSpeedHeightRatio)),
      Json::ReadSettingsFields<FTextPlacementSettings>(
          Object, JSON_SETTINGS_ATOMS(PromptAboveHeadFeet,
                                      NameAbovePromptFeet,
                                      DialogueAboveNameHeightRatio,
                                      PromptTextScale, DialogueTextScale)),
      Json::ReadSettingsFields<FMannequinSettings>(
          Object, JSON_SETTINGS_ATOMS(MannequinScale,
                                      MannequinOffsetFeet,
                                      MannequinRotation, MeshPath,
                                      AnimationBlueprintClassPath))};
}

/** User Story: As a data settings bot consumer, I need to invoke read horse presentation settings through a stable signature so the data settings bot workflow remains explicit and composable. @fn FHorsePresentationSettings ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) */
FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadSettingsFields<FBodySettings>(
          Object, JSON_SETTINGS_ATOMS(HorseLengthFeet, BodyHeightFeet,
                                      LegHeightFeet, NeckHeightFeet,
                                      HeadHeightFeet, SaddleHeightFeet)),
      Json::ReadSettingsFields<FHorseMotionSettings>(
          Object, JSON_SETTINGS_ATOMS(PatrolPauseSeconds,
                                      WalkSpeedHorseLengthRatio,
                                      RouteArrivalLegRatio)),
      Json::ReadSettingsFields<FRiderSettings>(
          Object, JSON_SETTINGS_ATOMS(ImportedHorseScale,
                                      MountedRiderScale,
                                      MountedRiderOffsetFeet)),
      Json::ReadSettingsFields<FNameSettings>(
          Object, JSON_SETTINGS_ATOMS(DefaultName, NameTextWorldSizeFeet)),
      Json::ReadSettingsFields<FAssetsSettings>(
          Object, JSON_SETTINGS_ATOMS(HorseMeshPath,
                                      HorseWalkAnimationPath, RiderMeshPath,
                                      RiderWalkAnimationPath))};
}

} // namespace BotAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
