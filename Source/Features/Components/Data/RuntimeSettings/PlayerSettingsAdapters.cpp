#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonSettingsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FPlayerPresentationSettings, CapsuleRadius,
                       CapsuleHalfHeight, FollowCameraArmLength,
                       RotationRateYaw, JumpZVelocity, AirControl,
                       MaxWalkSpeed, FlyModeSpeed, MinAnalogWalkSpeed,
                       BrakingDecelerationWalking, BrakingDecelerationFalling,
                       MeshRelativeLocation, MeshRelativeRotation,
                       SkeletalMeshForcedLodModel, SkeletalMeshMinLodModel,
                       MeshCullDistance, bMeshCastShadow,
                       bMeshComponentTickEnabled,
                       bMeshUpdateRateOptimizationsEnabled, MeshPath,
                       AnimationBlueprintClassPath, MoveActionPath,
                       LookActionPath, MouseLookActionPath, JumpActionPath,
                       DefaultMappingContextPath, MouseMappingContextPath);

JSON_SETTINGS_REGISTRY(FInteractionSettings, TownspersonMaxDistanceLots,
                       NoTownspersonMessage);

} // namespace JsonAdapters
namespace PlayerSettingsAdapters {
namespace Json = JsonAdapters;

FPlayerPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FPlayerPresentationSettings>(
      Object, JSON_SETTINGS_ATOMS(CapsuleRadius, CapsuleHalfHeight,
                                  FollowCameraArmLength, RotationRateYaw,
                                  JumpZVelocity, AirControl, MaxWalkSpeed,
                                  FlyModeSpeed, MinAnalogWalkSpeed,
                                  BrakingDecelerationWalking,
                                  BrakingDecelerationFalling,
                                  MeshRelativeLocation, MeshRelativeRotation,
                                  SkeletalMeshForcedLodModel,
                                  SkeletalMeshMinLodModel, MeshCullDistance,
                                  bMeshCastShadow, bMeshComponentTickEnabled,
                                  bMeshUpdateRateOptimizationsEnabled,
                                  MeshPath, AnimationBlueprintClassPath,
                                  MoveActionPath, LookActionPath,
                                  MouseLookActionPath, JumpActionPath,
                                  DefaultMappingContextPath,
                                  MouseMappingContextPath));
}

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
