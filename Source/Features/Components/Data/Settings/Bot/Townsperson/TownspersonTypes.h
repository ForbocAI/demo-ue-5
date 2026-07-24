#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FCharacterGeometrySettings {
  float CharacterHeightFeet;
  float CharacterShoulderWidthFeet;
};

struct FTownspersonMotionSettings {
  float PatrolPauseSeconds;
  float InteractionRadiusLots;
  float WalkSpeedHeightRatio;
};

struct FTextPlacementSettings {
  float PromptAboveHeadFeet;
  float NameAbovePromptFeet;
  float DialogueAboveNameHeightRatio;
  float PromptTextScale;
  float DialogueTextScale;
};

struct FMannequinSettings {
  float MannequinScale;
  FVector MannequinOffsetFeet;
  FRotator MannequinRotation;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FTownspersonPresentationSettings {
  FCharacterGeometrySettings Geometry;
  FTownspersonMotionSettings Motion;
  FTextPlacementSettings Text;
  FMannequinSettings Mannequin;
};

/** User Story: As a townsperson presentation consumer, I need to compare values for equality through a stable signature so presentation settings remain explicit and composable. @fn inline bool operator==(const FTownspersonPresentationSettings &Left, const FTownspersonPresentationSettings &Right) */
inline bool operator==(const FTownspersonPresentationSettings &Left,
                       const FTownspersonPresentationSettings &Right) {
  return FMath::IsNearlyEqual(Left.Geometry.CharacterHeightFeet,
                              Right.Geometry.CharacterHeightFeet) &&
         FMath::IsNearlyEqual(Left.Geometry.CharacterShoulderWidthFeet,
                              Right.Geometry.CharacterShoulderWidthFeet) &&
         FMath::IsNearlyEqual(Left.Motion.PatrolPauseSeconds,
                              Right.Motion.PatrolPauseSeconds) &&
         FMath::IsNearlyEqual(Left.Motion.InteractionRadiusLots,
                              Right.Motion.InteractionRadiusLots) &&
         FMath::IsNearlyEqual(Left.Motion.WalkSpeedHeightRatio,
                              Right.Motion.WalkSpeedHeightRatio) &&
         FMath::IsNearlyEqual(Left.Text.PromptAboveHeadFeet,
                              Right.Text.PromptAboveHeadFeet) &&
         FMath::IsNearlyEqual(Left.Text.NameAbovePromptFeet,
                              Right.Text.NameAbovePromptFeet) &&
         FMath::IsNearlyEqual(Left.Text.DialogueAboveNameHeightRatio,
                              Right.Text.DialogueAboveNameHeightRatio) &&
         FMath::IsNearlyEqual(Left.Text.PromptTextScale,
                              Right.Text.PromptTextScale) &&
         FMath::IsNearlyEqual(Left.Text.DialogueTextScale,
                              Right.Text.DialogueTextScale) &&
         FMath::IsNearlyEqual(Left.Mannequin.MannequinScale,
                              Right.Mannequin.MannequinScale) &&
         Left.Mannequin.MannequinOffsetFeet.Equals(
             Right.Mannequin.MannequinOffsetFeet) &&
         Left.Mannequin.MannequinRotation.Equals(
             Right.Mannequin.MannequinRotation) &&
         Left.Mannequin.MeshPath == Right.Mannequin.MeshPath &&
         Left.Mannequin.AnimationBlueprintClassPath ==
             Right.Mannequin.AnimationBlueprintClassPath;
}

/** User Story: As a townsperson presentation consumer, I need to compare values for inequality through a stable signature so presentation settings remain explicit and composable. @fn inline bool operator!=(const FTownspersonPresentationSettings &Left, const FTownspersonPresentationSettings &Right) */
inline bool operator!=(const FTownspersonPresentationSettings &Left,
                       const FTownspersonPresentationSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
