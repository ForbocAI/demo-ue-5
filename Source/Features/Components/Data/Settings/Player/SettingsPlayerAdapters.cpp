#include "Features/Components/Data/Settings/Player/PlayerAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FCapsuleSettings, CapsuleRadius, CapsuleHalfHeight);
JSON_SETTINGS_REGISTRY(FCameraSettings, FollowCameraArmLength);
JSON_SETTINGS_REGISTRY(FMovementSettings, RotationRateYaw, JumpZVelocity,
                       AirControl, MaxWalkSpeed, FlyModeSpeed,
                       MinAnalogWalkSpeed);
JSON_SETTINGS_REGISTRY(FBrakingSettings, BrakingDecelerationWalking,
                       BrakingDecelerationFalling);
JSON_SETTINGS_REGISTRY(FMeshTransformSettings, MeshRelativeLocation,
                       MeshRelativeRotation);
JSON_SETTINGS_REGISTRY(FMeshLodSettings, SkeletalMeshForcedLodModel,
                       SkeletalMeshMinLodModel, MeshCullDistance);
JSON_SETTINGS_REGISTRY(FMeshBehaviorSettings, bMeshCastShadow,
                       bMeshComponentTickEnabled,
                       bMeshUpdateRateOptimizationsEnabled);
JSON_SETTINGS_REGISTRY(FMeshAssetSettings, MeshPath,
                       AnimationBlueprintClassPath);
JSON_SETTINGS_REGISTRY(FInputActionSettings, MoveActionPath, LookActionPath,
                       MouseLookActionPath, JumpActionPath);
JSON_SETTINGS_REGISTRY(FInputMappingSettings, DefaultMappingContextPath,
                       MouseMappingContextPath);

JSON_SETTINGS_REGISTRY(FInteractionSettings, TownspersonMaxDistanceLots,
                       NoTownspersonMessage);

} // namespace JsonAdapters
namespace PlayerSettingsAdapters {
namespace Json = JsonAdapters;

/** User Story: As a data settings player consumer, I need to invoke read player presentation settings through a stable signature so the data settings player workflow remains explicit and composable. @fn FPresentationSettings ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) */
FPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return {
      Json::ReadSettingsFields<FCapsuleSettings>(
          Object, JSON_SETTINGS_ATOMS(CapsuleRadius, CapsuleHalfHeight)),
      Json::ReadSettingsFields<FCameraSettings>(
          Object, JSON_SETTINGS_ATOMS(FollowCameraArmLength)),
      Json::ReadSettingsFields<FMovementSettings>(
          Object, JSON_SETTINGS_ATOMS(RotationRateYaw, JumpZVelocity,
                                      AirControl, MaxWalkSpeed, FlyModeSpeed,
                                      MinAnalogWalkSpeed)),
      Json::ReadSettingsFields<FBrakingSettings>(
          Object, JSON_SETTINGS_ATOMS(BrakingDecelerationWalking,
                                      BrakingDecelerationFalling)),
      Json::ReadSettingsFields<FMeshTransformSettings>(
          Object,
          JSON_SETTINGS_ATOMS(MeshRelativeLocation, MeshRelativeRotation)),
      Json::ReadSettingsFields<FMeshLodSettings>(
          Object, JSON_SETTINGS_ATOMS(SkeletalMeshForcedLodModel,
                                      SkeletalMeshMinLodModel,
                                      MeshCullDistance)),
      Json::ReadSettingsFields<FMeshBehaviorSettings>(
          Object, JSON_SETTINGS_ATOMS(
                      bMeshCastShadow, bMeshComponentTickEnabled,
                      bMeshUpdateRateOptimizationsEnabled)),
      Json::ReadSettingsFields<FMeshAssetSettings>(
          Object, JSON_SETTINGS_ATOMS(MeshPath,
                                      AnimationBlueprintClassPath)),
      Json::ReadSettingsFields<FInputActionSettings>(
          Object, JSON_SETTINGS_ATOMS(MoveActionPath, LookActionPath,
                                      MouseLookActionPath, JumpActionPath)),
      Json::ReadSettingsFields<FInputMappingSettings>(
          Object, JSON_SETTINGS_ATOMS(DefaultMappingContextPath,
                                      MouseMappingContextPath))};
}

/** User Story: As a data settings player consumer, I need to invoke read interaction settings through a stable signature so the data settings player workflow remains explicit and composable. @fn FInteractionSettings ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object) */
FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FInteractionSettings>(
      Object, JSON_SETTINGS_ATOMS(TownspersonMaxDistanceLots,
                                  NoTownspersonMessage));
}

} // namespace PlayerSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
