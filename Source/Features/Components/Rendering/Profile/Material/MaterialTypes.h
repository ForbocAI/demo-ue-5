#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace RenderingProfile {

struct FBaseMaterialParameters {
  FString MaterialBaseColorParameter;
  FString MaterialColorParameter;
  FString MaterialTintColorParameter;
  FString MaterialDiffuseColorParameter;
  FString MaterialEmissiveColorParameter;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FBaseMaterialParameters &Left, const FBaseMaterialParameters &Right) */
inline bool operator==(const FBaseMaterialParameters &Left,
                       const FBaseMaterialParameters &Right) {
  return Left.MaterialBaseColorParameter == Right.MaterialBaseColorParameter &&
         Left.MaterialColorParameter == Right.MaterialColorParameter &&
         Left.MaterialTintColorParameter == Right.MaterialTintColorParameter &&
         Left.MaterialDiffuseColorParameter ==
             Right.MaterialDiffuseColorParameter &&
         Left.MaterialEmissiveColorParameter ==
             Right.MaterialEmissiveColorParameter;
}

struct FSkyDomeIlluminationParameters {
  FString SkyDomeSkyBrightnessParameter;
  FString SkyDomeCloudBrightnessParameter;
  FString SkyDomeCloudDarknessParameter;
  FString SkyDomeRimBrightnessParameter;
  FString SkyDomeStarsParameter;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FSkyDomeIlluminationParameters &Left, const FSkyDomeIlluminationParameters &Right) */
inline bool operator==(const FSkyDomeIlluminationParameters &Left,
                       const FSkyDomeIlluminationParameters &Right) {
  return Left.SkyDomeSkyBrightnessParameter ==
             Right.SkyDomeSkyBrightnessParameter &&
         Left.SkyDomeCloudBrightnessParameter ==
             Right.SkyDomeCloudBrightnessParameter &&
         Left.SkyDomeCloudDarknessParameter ==
             Right.SkyDomeCloudDarknessParameter &&
         Left.SkyDomeRimBrightnessParameter ==
             Right.SkyDomeRimBrightnessParameter &&
         Left.SkyDomeStarsParameter == Right.SkyDomeStarsParameter;
}

struct FSkyDomeColorParameters {
  FString SkyDomeStarColorParameter;
  FString SkyDomeHorizonColorParameter;
  FString SkyDomeZenithColorParameter;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FSkyDomeColorParameters &Left, const FSkyDomeColorParameters &Right) */
inline bool operator==(const FSkyDomeColorParameters &Left,
                       const FSkyDomeColorParameters &Right) {
  return Left.SkyDomeStarColorParameter ==
             Right.SkyDomeStarColorParameter &&
         Left.SkyDomeHorizonColorParameter ==
             Right.SkyDomeHorizonColorParameter &&
         Left.SkyDomeZenithColorParameter ==
             Right.SkyDomeZenithColorParameter;
}

struct FSkyDomeStarMaskColor {
  float SkyDomeTextureStarMaskColorR;
  float SkyDomeTextureStarMaskColorG;
  float SkyDomeTextureStarMaskColorB;
  float SkyDomeTextureStarMaskColorA;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FSkyDomeStarMaskColor &Left, const FSkyDomeStarMaskColor &Right) */
inline bool operator==(const FSkyDomeStarMaskColor &Left,
                       const FSkyDomeStarMaskColor &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorR,
                              Right.SkyDomeTextureStarMaskColorR) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorG,
                              Right.SkyDomeTextureStarMaskColorG) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorB,
                              Right.SkyDomeTextureStarMaskColorB) &&
         FMath::IsNearlyEqual(Left.SkyDomeTextureStarMaskColorA,
                              Right.SkyDomeTextureStarMaskColorA);
}

struct FSkyDomeStarVisibility {
  float SkyDomeStarsScalarValue;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FSkyDomeStarVisibility &Left, const FSkyDomeStarVisibility &Right) */
inline bool operator==(const FSkyDomeStarVisibility &Left,
                       const FSkyDomeStarVisibility &Right) {
  return FMath::IsNearlyEqual(Left.SkyDomeStarsScalarValue,
                              Right.SkyDomeStarsScalarValue);
}

struct FSkyDomeStars {
  FSkyDomeStarVisibility Visibility;
  FSkyDomeStarMaskColor MaskColor;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FSkyDomeStars &Left, const FSkyDomeStars &Right) */
inline bool operator==(const FSkyDomeStars &Left,
                       const FSkyDomeStars &Right) {
  return Left.Visibility == Right.Visibility && Left.MaskColor == Right.MaskColor;
}

struct FMaterial {
  FBaseMaterialParameters Base;
  FSkyDomeIlluminationParameters Illumination;
  FSkyDomeColorParameters Color;
  FSkyDomeStars Stars;
};

/** User Story: As the rendering profile state owner, I need material parameter concerns compared structurally so unchanged authored values do not rebuild runtime materials. @fn inline bool operator==(const FMaterial &Left, const FMaterial &Right) */
inline bool operator==(const FMaterial &Left, const FMaterial &Right) {
  return Left.Base == Right.Base &&
         Left.Illumination == Right.Illumination &&
         Left.Color == Right.Color && Left.Stars == Right.Stars;
}

} // namespace RenderingProfile
} // namespace Game
} // namespace ForbocAI
