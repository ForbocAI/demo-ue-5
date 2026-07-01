#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace BotSettingsAdapters {
namespace Json = JsonAdapters;

FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTownspersonDefaultsSettings>(
      Object, JSON_SETTINGS_FIELDS(FTownspersonDefaultsSettings, Id, Name,
                                   Role, Persona, InteractionPrompt,
                                   DefaultPlayerLine));
}

FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTownspersonPresentationSettings>(
      Object,
      JSON_SETTINGS_FIELDS(FTownspersonPresentationSettings,
                           CharacterHeightFeet, CharacterShoulderWidthFeet,
                           PatrolPauseSeconds, PromptAboveHeadFeet,
                           NameAbovePromptFeet, DialogueAboveNameHeightRatio,
                           InteractionRadiusLots, WalkSpeedHeightRatio,
                           MannequinScale, MannequinOffsetFeet,
                           MannequinRotation, PromptTextScale,
                           DialogueTextScale, MeshPath,
                           AnimationBlueprintClassPath));
}

FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FHorsePresentationSettings>(
      Object, JSON_SETTINGS_FIELDS(FHorsePresentationSettings, DefaultName,
                                   HorseLengthFeet, BodyHeightFeet,
                                   LegHeightFeet, NeckHeightFeet,
                                   HeadHeightFeet, SaddleHeightFeet,
                                   PatrolPauseSeconds,
                                   WalkSpeedHorseLengthRatio,
                                   RouteArrivalLegRatio, ImportedHorseScale,
                                   MountedRiderScale, NameTextWorldSizeFeet,
                                   HorseMeshPath, HorseWalkAnimationPath,
                                   RiderMeshPath, RiderWalkAnimationPath));
}

} // namespace BotSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
