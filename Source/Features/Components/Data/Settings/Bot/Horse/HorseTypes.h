#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FHorseBodySettings {
  float HorseLengthFeet;
  float BodyHeightFeet;
  float LegHeightFeet;
  float NeckHeightFeet;
  float HeadHeightFeet;
  float SaddleHeightFeet;
};

struct FHorseMotionSettings {
  float PatrolPauseSeconds;
  float WalkSpeedHorseLengthRatio;
  float RouteArrivalLegRatio;
};

struct FRiderSettings {
  float ImportedHorseScale;
  float MountedRiderScale;
  FVector MountedRiderOffsetFeet;
};

struct FHorseNameSettings {
  FString DefaultName;
  float NameTextWorldSizeFeet;
};

struct FHorseAssetsSettings {
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};

struct FHorsePresentationSettings {
  FHorseBodySettings Body;
  FHorseMotionSettings Motion;
  FRiderSettings Rider;
  FHorseNameSettings Name;
  FHorseAssetsSettings Assets;
};

/** User Story: As a horse presentation consumer, I need to compare values for equality through a stable signature so presentation settings remain explicit and composable. @fn inline bool operator==(const FHorsePresentationSettings &Left, const FHorsePresentationSettings &Right) */
inline bool operator==(const FHorsePresentationSettings &Left,
                       const FHorsePresentationSettings &Right) {
  return FMath::IsNearlyEqual(Left.Body.HorseLengthFeet,
                              Right.Body.HorseLengthFeet) &&
         FMath::IsNearlyEqual(Left.Body.BodyHeightFeet,
                              Right.Body.BodyHeightFeet) &&
         FMath::IsNearlyEqual(Left.Body.LegHeightFeet,
                              Right.Body.LegHeightFeet) &&
         FMath::IsNearlyEqual(Left.Body.NeckHeightFeet,
                              Right.Body.NeckHeightFeet) &&
         FMath::IsNearlyEqual(Left.Body.HeadHeightFeet,
                              Right.Body.HeadHeightFeet) &&
         FMath::IsNearlyEqual(Left.Body.SaddleHeightFeet,
                              Right.Body.SaddleHeightFeet) &&
         FMath::IsNearlyEqual(Left.Motion.PatrolPauseSeconds,
                              Right.Motion.PatrolPauseSeconds) &&
         FMath::IsNearlyEqual(Left.Motion.WalkSpeedHorseLengthRatio,
                              Right.Motion.WalkSpeedHorseLengthRatio) &&
         FMath::IsNearlyEqual(Left.Motion.RouteArrivalLegRatio,
                              Right.Motion.RouteArrivalLegRatio) &&
         FMath::IsNearlyEqual(Left.Rider.ImportedHorseScale,
                              Right.Rider.ImportedHorseScale) &&
         FMath::IsNearlyEqual(Left.Rider.MountedRiderScale,
                              Right.Rider.MountedRiderScale) &&
         Left.Rider.MountedRiderOffsetFeet.Equals(
             Right.Rider.MountedRiderOffsetFeet) &&
         Left.Name.DefaultName == Right.Name.DefaultName &&
         FMath::IsNearlyEqual(Left.Name.NameTextWorldSizeFeet,
                              Right.Name.NameTextWorldSizeFeet) &&
         Left.Assets.HorseMeshPath == Right.Assets.HorseMeshPath &&
         Left.Assets.HorseWalkAnimationPath ==
             Right.Assets.HorseWalkAnimationPath &&
         Left.Assets.RiderMeshPath == Right.Assets.RiderMeshPath &&
         Left.Assets.RiderWalkAnimationPath ==
             Right.Assets.RiderWalkAnimationPath;
}

/** User Story: As a horse presentation consumer, I need to compare values for inequality through a stable signature so presentation settings remain explicit and composable. @fn inline bool operator!=(const FHorsePresentationSettings &Left, const FHorsePresentationSettings &Right) */
inline bool operator!=(const FHorsePresentationSettings &Left,
                       const FHorsePresentationSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
