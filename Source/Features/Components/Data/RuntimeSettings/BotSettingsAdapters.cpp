#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FTownspersonDefaultsSettings, Id, Name, Role, Persona,
                       InteractionPrompt, DefaultPlayerLine);

JSON_SETTINGS_REGISTRY(FTownspersonPresentationSettings, CharacterHeightFeet,
                       CharacterShoulderWidthFeet, PatrolPauseSeconds,
                       PromptAboveHeadFeet, NameAbovePromptFeet,
                       DialogueAboveNameHeightRatio, InteractionRadiusLots,
                       WalkSpeedHeightRatio, MannequinScale,
                       MannequinOffsetFeet, MannequinRotation,
                       PromptTextScale, DialogueTextScale, MeshPath,
                       AnimationBlueprintClassPath);

JSON_SETTINGS_REGISTRY(FHorsePresentationSettings, DefaultName, HorseLengthFeet,
                       BodyHeightFeet, LegHeightFeet, NeckHeightFeet,
                       HeadHeightFeet, SaddleHeightFeet, PatrolPauseSeconds,
                       WalkSpeedHorseLengthRatio, RouteArrivalLegRatio,
                       ImportedHorseScale, MountedRiderScale,
                       NameTextWorldSizeFeet, HorseMeshPath,
                       HorseWalkAnimationPath, RiderMeshPath,
                       RiderWalkAnimationPath);

} // namespace JsonAdapters
namespace BotSettingsAdapters {
namespace Json = JsonAdapters;

FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTownspersonDefaultsSettings>(
      Object, JSON_SETTINGS_ATOMS(Id, Name, Role, Persona, InteractionPrompt,
                                  DefaultPlayerLine));
}

FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FTownspersonPresentationSettings>(
      Object, JSON_SETTINGS_ATOMS(CharacterHeightFeet,
                                  CharacterShoulderWidthFeet,
                                  PatrolPauseSeconds, PromptAboveHeadFeet,
                                  NameAbovePromptFeet,
                                  DialogueAboveNameHeightRatio,
                                  InteractionRadiusLots, WalkSpeedHeightRatio,
                                  MannequinScale, MannequinOffsetFeet,
                                  MannequinRotation, PromptTextScale,
                                  DialogueTextScale, MeshPath,
                                  AnimationBlueprintClassPath));
}

FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FHorsePresentationSettings>(
      Object, JSON_SETTINGS_ATOMS(DefaultName, HorseLengthFeet, BodyHeightFeet,
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
