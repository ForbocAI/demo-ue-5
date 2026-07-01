#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace PlayerSettingsAdapters {
namespace Json = JsonAdapters;

FPlayerPresentationSettings
ReadPlayerPresentationSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FPlayerPresentationSettings>(
      Object, JSON_SETTINGS_FIELDS(FPlayerPresentationSettings, CapsuleRadius,
                                   CapsuleHalfHeight, FollowCameraArmLength,
                                   RotationRateYaw, JumpZVelocity, AirControl,
                                   MaxWalkSpeed, MinAnalogWalkSpeed,
                                   BrakingDecelerationWalking,
                                   BrakingDecelerationFalling,
                                   MeshRelativeLocation, MeshRelativeRotation,
                                   MeshPath, AnimationBlueprintClassPath,
                                   MoveActionPath, LookActionPath,
                                   MouseLookActionPath, JumpActionPath,
                                   DefaultMappingContextPath,
                                   MouseMappingContextPath));
}

FInteractionSettings
ReadInteractionSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FInteractionSettings>(
      Object, JSON_SETTINGS_FIELDS(FInteractionSettings,
                                   TownspersonMaxDistanceLots,
                                   NoTownspersonMessage));
}

} // namespace PlayerSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
