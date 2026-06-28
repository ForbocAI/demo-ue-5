#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace BotSettingsAdapters {
namespace Json = JsonAdapters;

FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FTownspersonDefaultsSettings Settings;
  Settings.Id = String(TEXT("id"));
  Settings.Name = String(TEXT("name"));
  Settings.Role = String(TEXT("role"));
  Settings.Persona = String(TEXT("persona"));
  Settings.InteractionPrompt = String(TEXT("interaction_prompt"));
  Settings.DefaultPlayerLine = String(TEXT("default_player_line"));
  return Settings;
}

FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonVectorReader Vector = Json::VectorIn(Object);
  const Json::FJsonRotatorReader Rotator = Json::RotatorIn(Object);
  FTownspersonPresentationSettings Settings;
  Settings.CharacterHeightFeet = Float(TEXT("character_height_feet"));
  Settings.CharacterShoulderWidthFeet =
      Float(TEXT("character_shoulder_width_feet"));
  Settings.PatrolPauseSeconds = Float(TEXT("patrol_pause_seconds"));
  Settings.PromptAboveHeadFeet = Float(TEXT("prompt_above_head_feet"));
  Settings.NameAbovePromptFeet = Float(TEXT("name_above_prompt_feet"));
  Settings.DialogueAboveNameHeightRatio =
      Float(TEXT("dialogue_above_name_height_ratio"));
  Settings.InteractionRadiusLots = Float(TEXT("interaction_radius_lots"));
  Settings.WalkSpeedHeightRatio = Float(TEXT("walk_speed_height_ratio"));
  Settings.MannequinScale = Float(TEXT("mannequin_scale"));
  Settings.MannequinOffsetFeet = Vector(TEXT("mannequin_offset_feet"));
  Settings.MannequinRotation = Rotator(TEXT("mannequin_rotation"));
  Settings.PromptTextScale = Float(TEXT("prompt_text_scale"));
  Settings.DialogueTextScale = Float(TEXT("dialogue_text_scale"));
  Settings.MeshPath = String(TEXT("mesh_path"));
  Settings.AnimationBlueprintClassPath =
      String(TEXT("animation_blueprint_class_path"));
  return Settings;
}

FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FHorsePresentationSettings Settings;
  Settings.DefaultName = String(TEXT("default_name"));
  Settings.HorseLengthFeet = Float(TEXT("horse_length_feet"));
  Settings.BodyHeightFeet = Float(TEXT("body_height_feet"));
  Settings.LegHeightFeet = Float(TEXT("leg_height_feet"));
  Settings.NeckHeightFeet = Float(TEXT("neck_height_feet"));
  Settings.HeadHeightFeet = Float(TEXT("head_height_feet"));
  Settings.SaddleHeightFeet = Float(TEXT("saddle_height_feet"));
  Settings.PatrolPauseSeconds = Float(TEXT("patrol_pause_seconds"));
  Settings.WalkSpeedHorseLengthRatio =
      Float(TEXT("walk_speed_horse_length_ratio"));
  Settings.RouteArrivalLegRatio = Float(TEXT("route_arrival_leg_ratio"));
  Settings.ImportedHorseScale = Float(TEXT("imported_horse_scale"));
  Settings.MountedRiderScale = Float(TEXT("mounted_rider_scale"));
  Settings.NameTextWorldSizeFeet = Float(TEXT("name_text_world_size_feet"));
  Settings.HorseMeshPath = String(TEXT("horse_mesh_path"));
  Settings.HorseWalkAnimationPath = String(TEXT("horse_walk_animation_path"));
  Settings.RiderMeshPath = String(TEXT("rider_mesh_path"));
  Settings.RiderWalkAnimationPath = String(TEXT("rider_walk_animation_path"));
  return Settings;
}

} // namespace BotSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
