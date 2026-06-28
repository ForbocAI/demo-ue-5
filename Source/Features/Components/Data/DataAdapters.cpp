#include "Features/Components/Data/DataAdapters.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataAdapters {
namespace {

TSharedPtr<FJsonObject> ReadObjectValue(const FJsonFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value = ReadObject(Request);
  return Value.hasValue ? Value.value : TSharedPtr<FJsonObject>();
}

FJsonFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name) {
  return {Object, FString(Name)};
}

FPlayerPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  FPlayerPresentationSettings Settings;
  Settings.CapsuleRadius = ReadFloat(Field(Object, TEXT("capsule_radius")));
  Settings.CapsuleHalfHeight =
      ReadFloat(Field(Object, TEXT("capsule_half_height")));
  Settings.FollowCameraArmLength =
      ReadFloat(Field(Object, TEXT("follow_camera_arm_length")));
  Settings.RotationRateYaw =
      ReadFloat(Field(Object, TEXT("rotation_rate_yaw")));
  Settings.JumpZVelocity = ReadFloat(Field(Object, TEXT("jump_z_velocity")));
  Settings.AirControl = ReadFloat(Field(Object, TEXT("air_control")));
  Settings.MaxWalkSpeed = ReadFloat(Field(Object, TEXT("max_walk_speed")));
  Settings.MinAnalogWalkSpeed =
      ReadFloat(Field(Object, TEXT("min_analog_walk_speed")));
  Settings.BrakingDecelerationWalking =
      ReadFloat(Field(Object, TEXT("braking_deceleration_walking")));
  Settings.BrakingDecelerationFalling =
      ReadFloat(Field(Object, TEXT("braking_deceleration_falling")));
  Settings.MeshRelativeLocation =
      ReadVector(Field(Object, TEXT("mesh_relative_location")));
  Settings.MeshRelativeRotation =
      ReadRotator(Field(Object, TEXT("mesh_relative_rotation")));
  Settings.MeshPath = ReadString(Field(Object, TEXT("mesh_path")));
  Settings.AnimationBlueprintClassPath =
      ReadString(Field(Object, TEXT("animation_blueprint_class_path")));
  Settings.MoveActionPath =
      ReadString(Field(Object, TEXT("move_action_path")));
  Settings.LookActionPath =
      ReadString(Field(Object, TEXT("look_action_path")));
  Settings.MouseLookActionPath =
      ReadString(Field(Object, TEXT("mouse_look_action_path")));
  Settings.JumpActionPath =
      ReadString(Field(Object, TEXT("jump_action_path")));
  Settings.DefaultMappingContextPath =
      ReadString(Field(Object, TEXT("default_mapping_context_path")));
  Settings.MouseMappingContextPath =
      ReadString(Field(Object, TEXT("mouse_mapping_context_path")));
  return Settings;
}

FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object) {
  FInteractionSettings Settings;
  Settings.TownspersonMaxDistanceLots =
      ReadFloat(Field(Object, TEXT("townsperson_max_distance_lots")));
  Settings.NoTownspersonMessage =
      ReadString(Field(Object, TEXT("no_townsperson_message")));
  return Settings;
}

FTownspersonDefaultsSettings
ReadTownspersonDefaultsSettings(const TSharedPtr<FJsonObject> &Object) {
  FTownspersonDefaultsSettings Settings;
  Settings.Id = ReadString(Field(Object, TEXT("id")));
  Settings.Name = ReadString(Field(Object, TEXT("name")));
  Settings.Role = ReadString(Field(Object, TEXT("role")));
  Settings.Persona = ReadString(Field(Object, TEXT("persona")));
  Settings.InteractionPrompt =
      ReadString(Field(Object, TEXT("interaction_prompt")));
  Settings.DefaultPlayerLine =
      ReadString(Field(Object, TEXT("default_player_line")));
  return Settings;
}

FLevelTerrainSourceSettings
ReadLevelTerrainSourceSettings(const TSharedPtr<FJsonObject> &Object) {
  FLevelTerrainSourceSettings Settings;
  Settings.TerrainCsvPath =
      ReadString(Field(Object, TEXT("terrain_csv_path")));
  Settings.OrthoCsvPath = ReadString(Field(Object, TEXT("ortho_csv_path")));
  return Settings;
}

FLevelGeometrySettings
ReadLevelGeometrySettings(const TSharedPtr<FJsonObject> &Object) {
  FLevelGeometrySettings Settings;
  Settings.TerrainWorldSize =
      ReadFloat(Field(Object, TEXT("terrain_world_size")));
  Settings.TerrainElevationScale =
      ReadFloat(Field(Object, TEXT("terrain_elevation_scale")));
  Settings.TerrainLotsAcross =
      ReadFloat(Field(Object, TEXT("terrain_lots_across")));
  Settings.PostOfficeEastLots =
      ReadFloat(Field(Object, TEXT("post_office_east_lots")));
  Settings.PostOfficeNorthLots =
      ReadFloat(Field(Object, TEXT("post_office_north_lots")));
  Settings.CubeMeshSize = ReadFloat(Field(Object, TEXT("cube_mesh_size")));
  Settings.BlockScalePerFoot =
      ReadFloat(Field(Object, TEXT("block_scale_per_foot")));
  Settings.HeightScalePerStory =
      ReadFloat(Field(Object, TEXT("height_scale_per_story")));
  Settings.FoundationHeightRatio =
      ReadFloat(Field(Object, TEXT("foundation_height_ratio")));
  Settings.RoadClearanceRatio =
      ReadFloat(Field(Object, TEXT("road_clearance_ratio")));
  Settings.CharacterHeightRatio =
      ReadFloat(Field(Object, TEXT("character_height_ratio")));
  Settings.LabelClearanceRatio =
      ReadFloat(Field(Object, TEXT("label_clearance_ratio")));
  Settings.ActorFootToTerrainRatio =
      ReadFloat(Field(Object, TEXT("actor_foot_to_terrain_ratio")));
  Settings.ActorReferenceFeetAcross =
      ReadFloat(Field(Object, TEXT("actor_reference_feet_across")));
  Settings.PlayerSpawnNorthLots =
      ReadFloat(Field(Object, TEXT("player_spawn_north_lots")));
  Settings.PlayerSpawnExtraHeightRatio =
      ReadFloat(Field(Object, TEXT("player_spawn_extra_height_ratio")));
  Settings.MainStreetFacingYawDegrees =
      ReadFloat(Field(Object, TEXT("main_street_facing_yaw_degrees")));
  Settings.PlayerSpawnAnchorLabel =
      ReadString(Field(Object, TEXT("player_spawn_anchor_label")));
  Settings.LandmarkLabelWorldSizeScale =
      ReadFloat(Field(Object, TEXT("landmark_label_world_size_scale")));
  Settings.NatureLabelWorldSizeScale =
      ReadFloat(Field(Object, TEXT("nature_label_world_size_scale")));
  return Settings;
}

FRenderingAssetPathSettings
ReadRenderingAssetPathSettings(const TSharedPtr<FJsonObject> &Object) {
  FRenderingAssetPathSettings Settings;
  Settings.LevelCubeMeshPath =
      ReadString(Field(Object, TEXT("level_cube_mesh_path")));
  Settings.BlockoutMaterialPath =
      ReadString(Field(Object, TEXT("blockout_material_path")));
  return Settings;
}

FRenderingProfileSettings
ReadRenderingProfileSettings(const TSharedPtr<FJsonObject> &Object) {
  FRenderingProfileSettings Settings;
  Settings.TimeOfDayHour =
      ReadFloat(Field(Object, TEXT("time_of_day_hour")));
  Settings.AntiAliasingMethod =
      ReadInt(Field(Object, TEXT("anti_aliasing_method")));
  Settings.PostProcessAAQuality =
      ReadInt(Field(Object, TEXT("post_process_aa_quality")));
  Settings.ScreenPercentage =
      ReadFloat(Field(Object, TEXT("screen_percentage")));
  Settings.ViewDistanceScale =
      ReadFloat(Field(Object, TEXT("view_distance_scale")));
  Settings.FoliageDensityScale =
      ReadFloat(Field(Object, TEXT("foliage_density_scale")));
  Settings.GrassDensityScale =
      ReadFloat(Field(Object, TEXT("grass_density_scale")));
  Settings.SunPitchDegrees =
      ReadFloat(Field(Object, TEXT("sun_pitch_degrees")));
  Settings.SunYawDegrees =
      ReadFloat(Field(Object, TEXT("sun_yaw_degrees")));
  Settings.DirectionalLightIntensity =
      ReadFloat(Field(Object, TEXT("directional_light_intensity")));
  Settings.DirectionalLightSourceAngle =
      ReadFloat(Field(Object, TEXT("directional_light_source_angle")));
  Settings.ShadowCascades =
      ReadInt(Field(Object, TEXT("shadow_cascades")));
  Settings.ShadowMaxResolution =
      ReadInt(Field(Object, TEXT("shadow_max_resolution")));
  return Settings;
}

FRenderingTextureSpecSettings
ReadRenderingTextureSpecSettings(const TSharedPtr<FJsonObject> &Object) {
  FRenderingTextureSpecSettings Settings;
  Settings.Texture = ReadString(Field(Object, TEXT("texture")));
  Settings.Name = ReadString(Field(Object, TEXT("name")));
  Settings.Use = ReadString(Field(Object, TEXT("use")));
  Settings.Size = ReadInt(Field(Object, TEXT("size")));
  return Settings;
}

TArray<FRenderingTextureSpecSettings>
ReadTextureCatalogSettings(const TSharedPtr<FJsonObject> &Object) {
  return MapJsonValues<FRenderingTextureSpecSettings>(
      {ReadArray(Field(Object, TEXT("texture_catalog"))),
       ReadRenderingTextureSpecSettings});
}

FTownspersonPresentationSettings
ReadTownspersonPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  FTownspersonPresentationSettings Settings;
  Settings.CharacterHeightFeet =
      ReadFloat(Field(Object, TEXT("character_height_feet")));
  Settings.CharacterShoulderWidthFeet =
      ReadFloat(Field(Object, TEXT("character_shoulder_width_feet")));
  Settings.PatrolPauseSeconds =
      ReadFloat(Field(Object, TEXT("patrol_pause_seconds")));
  Settings.PromptAboveHeadFeet =
      ReadFloat(Field(Object, TEXT("prompt_above_head_feet")));
  Settings.NameAbovePromptFeet =
      ReadFloat(Field(Object, TEXT("name_above_prompt_feet")));
  Settings.DialogueAboveNameHeightRatio =
      ReadFloat(Field(Object, TEXT("dialogue_above_name_height_ratio")));
  Settings.InteractionRadiusLots =
      ReadFloat(Field(Object, TEXT("interaction_radius_lots")));
  Settings.WalkSpeedHeightRatio =
      ReadFloat(Field(Object, TEXT("walk_speed_height_ratio")));
  Settings.MannequinScale =
      ReadFloat(Field(Object, TEXT("mannequin_scale")));
  Settings.MannequinOffsetFeet =
      ReadVector(Field(Object, TEXT("mannequin_offset_feet")));
  Settings.MannequinRotation =
      ReadRotator(Field(Object, TEXT("mannequin_rotation")));
  Settings.PromptTextScale =
      ReadFloat(Field(Object, TEXT("prompt_text_scale")));
  Settings.DialogueTextScale =
      ReadFloat(Field(Object, TEXT("dialogue_text_scale")));
  Settings.MeshPath = ReadString(Field(Object, TEXT("mesh_path")));
  Settings.AnimationBlueprintClassPath =
      ReadString(Field(Object, TEXT("animation_blueprint_class_path")));
  return Settings;
}

FHorsePresentationSettings
ReadHorsePresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  FHorsePresentationSettings Settings;
  Settings.DefaultName = ReadString(Field(Object, TEXT("default_name")));
  Settings.HorseLengthFeet =
      ReadFloat(Field(Object, TEXT("horse_length_feet")));
  Settings.BodyHeightFeet =
      ReadFloat(Field(Object, TEXT("body_height_feet")));
  Settings.LegHeightFeet =
      ReadFloat(Field(Object, TEXT("leg_height_feet")));
  Settings.NeckHeightFeet =
      ReadFloat(Field(Object, TEXT("neck_height_feet")));
  Settings.HeadHeightFeet =
      ReadFloat(Field(Object, TEXT("head_height_feet")));
  Settings.SaddleHeightFeet =
      ReadFloat(Field(Object, TEXT("saddle_height_feet")));
  Settings.PatrolPauseSeconds =
      ReadFloat(Field(Object, TEXT("patrol_pause_seconds")));
  Settings.WalkSpeedHorseLengthRatio =
      ReadFloat(Field(Object, TEXT("walk_speed_horse_length_ratio")));
  Settings.RouteArrivalLegRatio =
      ReadFloat(Field(Object, TEXT("route_arrival_leg_ratio")));
  Settings.ImportedHorseScale =
      ReadFloat(Field(Object, TEXT("imported_horse_scale")));
  Settings.MountedRiderScale =
      ReadFloat(Field(Object, TEXT("mounted_rider_scale")));
  Settings.NameTextWorldSizeFeet =
      ReadFloat(Field(Object, TEXT("name_text_world_size_feet")));
  Settings.HorseMeshPath =
      ReadString(Field(Object, TEXT("horse_mesh_path")));
  Settings.HorseWalkAnimationPath =
      ReadString(Field(Object, TEXT("horse_walk_animation_path")));
  Settings.RiderMeshPath =
      ReadString(Field(Object, TEXT("rider_mesh_path")));
  Settings.RiderWalkAnimationPath =
      ReadString(Field(Object, TEXT("rider_walk_animation_path")));
  return Settings;
}

FDemoRuntimeSettings
ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  FDemoRuntimeSettings Settings;
  Settings.PlayerPresentation = ReadPlayerPresentationSettings(
      ReadObjectValue(Field(Object, TEXT("player_presentation"))));
  Settings.Interaction =
      ReadInteractionSettings(ReadObjectValue(Field(Object, TEXT("interaction"))));
  Settings.TownspersonDefaults = ReadTownspersonDefaultsSettings(
      ReadObjectValue(Field(Object, TEXT("townsperson_defaults"))));
  Settings.LevelTerrainSources = ReadLevelTerrainSourceSettings(
      ReadObjectValue(Field(Object, TEXT("level_terrain_sources"))));
  Settings.LevelGeometry = ReadLevelGeometrySettings(
      ReadObjectValue(Field(Object, TEXT("level_geometry"))));
  Settings.RenderingAssets = ReadRenderingAssetPathSettings(
      ReadObjectValue(Field(Object, TEXT("rendering_assets"))));
  Settings.RenderingProfile = ReadRenderingProfileSettings(
      ReadObjectValue(Field(Object, TEXT("rendering_profile"))));
  Settings.TextureCatalog = ReadTextureCatalogSettings(Object);
  Settings.TownspersonPresentation = ReadTownspersonPresentationSettings(
      ReadObjectValue(Field(Object, TEXT("townsperson_presentation"))));
  Settings.HorsePresentation = ReadHorsePresentationSettings(
      ReadObjectValue(Field(Object, TEXT("horse_presentation"))));
  return Settings;
}

} // namespace

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request) {
  const FString SourcePath = FPaths::ProjectContentDir() / Request.RelativePath;
  FString Source;
  if (!FFileHelper::LoadFileToString(Source, *SourcePath)) {
    UE_LOG(LogTemp, Error, TEXT("Data JSON missing: %s"), *SourcePath);
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  TSharedPtr<FJsonObject> Root;
  const TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(Source);
  if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
    UE_LOG(LogTemp, Error, TEXT("Data JSON invalid: %s"), *SourcePath);
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  return func::just(Root);
}

FString ReadString(const FJsonFieldRequest &Request) {
  FString Value;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetStringField(Request.FieldName, Value)
             ? Value
             : FString();
}

float ReadFloat(const FJsonFieldRequest &Request) {
  double Value = 0.0;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetNumberField(Request.FieldName, Value)
             ? static_cast<float>(Value)
             : 0.0f;
}

int32 ReadInt(const FJsonFieldRequest &Request) {
  return static_cast<int32>(ReadFloat(Request));
}

bool ReadBool(const FJsonFieldRequest &Request) {
  bool Value = false;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetBoolField(Request.FieldName, Value)
             ? Value
             : false;
}

TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  return Request.Object.IsValid() &&
                 Request.Object->TryGetArrayField(Request.FieldName, Values) &&
                 Values != nullptr
             ? *Values
             : TArray<TSharedPtr<FJsonValue>>();
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request) {
  if (!Request.Object.IsValid()) {
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  const TSharedPtr<FJsonObject> *Child = nullptr;
  Request.Object->TryGetObjectField(Request.FieldName, Child);
  return Child != nullptr && Child->IsValid()
             ? func::just(*Child)
             : func::nothing<TSharedPtr<FJsonObject>>();
}

FVector ReadVector(const FJsonFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FVector(ReadFloat(Field(Object, TEXT("x"))),
                 ReadFloat(Field(Object, TEXT("y"))),
                 ReadFloat(Field(Object, TEXT("z"))));
}

FRotator ReadRotator(const FJsonFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FRotator(ReadFloat(Field(Object, TEXT("pitch"))),
                  ReadFloat(Field(Object, TEXT("yaw"))),
                  ReadFloat(Field(Object, TEXT("roll"))));
}

FDemoRuntimeSettings LoadDemoRuntimeSettings() {
  const func::Maybe<TSharedPtr<FJsonObject>> Root =
      LoadObjectFromContent({TEXT("Data/runtime_demo_settings.json")});
  return Root.hasValue ? ReadDemoRuntimeSettings(Root.value)
                       : FDemoRuntimeSettings();
}

} // namespace DataAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
