#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace PlayerSettingsAdapters {
namespace Json = JsonAdapters;

FPlayerPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonVectorReader Vector = Json::VectorIn(Object);
  const Json::FJsonRotatorReader Rotator = Json::RotatorIn(Object);
  FPlayerPresentationSettings Settings;
  Settings.CapsuleRadius = Float(TEXT("capsule_radius"));
  Settings.CapsuleHalfHeight = Float(TEXT("capsule_half_height"));
  Settings.FollowCameraArmLength = Float(TEXT("follow_camera_arm_length"));
  Settings.RotationRateYaw = Float(TEXT("rotation_rate_yaw"));
  Settings.JumpZVelocity = Float(TEXT("jump_z_velocity"));
  Settings.AirControl = Float(TEXT("air_control"));
  Settings.MaxWalkSpeed = Float(TEXT("max_walk_speed"));
  Settings.MinAnalogWalkSpeed = Float(TEXT("min_analog_walk_speed"));
  Settings.BrakingDecelerationWalking =
      Float(TEXT("braking_deceleration_walking"));
  Settings.BrakingDecelerationFalling =
      Float(TEXT("braking_deceleration_falling"));
  Settings.MeshRelativeLocation = Vector(TEXT("mesh_relative_location"));
  Settings.MeshRelativeRotation = Rotator(TEXT("mesh_relative_rotation"));
  Settings.MeshPath = String(TEXT("mesh_path"));
  Settings.AnimationBlueprintClassPath =
      String(TEXT("animation_blueprint_class_path"));
  Settings.MoveActionPath = String(TEXT("move_action_path"));
  Settings.LookActionPath = String(TEXT("look_action_path"));
  Settings.MouseLookActionPath = String(TEXT("mouse_look_action_path"));
  Settings.JumpActionPath = String(TEXT("jump_action_path"));
  Settings.DefaultMappingContextPath =
      String(TEXT("default_mapping_context_path"));
  Settings.MouseMappingContextPath =
      String(TEXT("mouse_mapping_context_path"));
  return Settings;
}

FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FInteractionSettings Settings;
  Settings.TownspersonMaxDistanceLots =
      Float(TEXT("townsperson_max_distance_lots"));
  Settings.NoTownspersonMessage = String(TEXT("no_townsperson_message"));
  return Settings;
}

} // namespace PlayerSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
