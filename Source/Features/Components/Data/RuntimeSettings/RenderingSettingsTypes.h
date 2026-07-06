#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRenderingAssetPathSettings {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
};


struct FRenderingProfileSettings {
  float TimeOfDayHour;
  int32 AntiAliasingMethod;
  int32 PostProcessAAQuality;
  float ScreenPercentage;
  float MinimumScreenPercentage;
  int32 InternalRenderWidth;
  int32 InternalRenderHeight;
  int32 OutputScaleMultiplier;
  bool bFullscreenOutput;
  float ViewDistanceScale;
  float FoliageDensityScale;
  float GrassDensityScale;
  float SunPitchDegrees;
  float SunYawDegrees;
  float DirectionalLightIntensity;
  float DirectionalLightSourceAngle;
  int32 ShadowCascades;
  int32 ShadowMaxResolution;
  bool bFogEnabled;
  bool bVolumetricFogEnabled;
  float FogDensity;
  float FogHeightFalloff;
  float FogStartDistance;
  float FogCutoffDistance;
  float FogMaxOpacity;
  float FogColorR;
  float FogColorG;
  float FogColorB;
  float FogColorA;
};


struct FRenderingTextureSpecSettings {
  FString Texture;
  FString Name;
  FString Use;
  int32 Size;
};


struct FRenderingRgbSettings {
  int32 R;
  int32 G;
  int32 B;
};


struct FRenderingTextureHashSettings {
  int32 XMultiplier;
  int32 YMultiplier;
  int32 SaltMultiplier;
  int32 XorShift;
};


struct FRenderingConsoleVariableSettings {
  FString Name;
  FString ValueKind;
  FString ProfileField;
  int32 IntValue;
  float FloatValue;
};


struct FRenderingTexturePredicateSettings {
  FString Kind;
  int32 XMultiplier;
  int32 YMultiplier;
  int32 NoiseMultiplier;
  int32 XDivisor;
  int32 YDivisor;
  int32 Modulus;
  int32 Equals;
  int32 LessThan;
};


struct FRenderingTextureColorResultSettings {
  FString Kind;
  FRenderingRgbSettings Color;
  FRenderingRgbSettings ColorA;
  FRenderingRgbSettings ColorB;
  int32 NumeratorBase;
  int32 NumeratorNoiseModulus;
  int32 Denominator;
};


struct FRenderingTextureRuleSettings {
  FRenderingTexturePredicateSettings Predicate;
  FRenderingTextureColorResultSettings Result;
};


struct FRenderingTexturePaletteSettings {
  FString Texture;
  TArray<FRenderingTextureRuleSettings> Rules;
};


struct FRenderingRuntimeSettings {
  int32 TextureChannels;
  int32 TextureAlpha;
  FString TextureCacheKeyFormat;
  FString MaterialTextureParameter;
  FRenderingTextureHashSettings TextureHash;
  TArray<FRenderingConsoleVariableSettings> ConsoleVariables;
  TArray<FRenderingTexturePaletteSettings> TexturePalettes;
};

struct FRenderingDistanceLodStageSettings {
  FString Id;
  float MaxDistance;
  int32 StaticMeshForcedLodModel;
  int32 SkeletalMeshForcedLodModel;
  int32 SkeletalMeshMinLodModel;
  float CullDistance;
  float ActorTickIntervalSeconds;
  bool bStaticVisible;
  bool bDynamicVisible;
  bool bLabelsVisible;
  bool bAnimated;
  bool bUpdateRateOptimizationsEnabled;
  bool bPatrolEnabled;
  bool bCollisionEnabled;
  bool bCastShadow;
};

struct FRenderingDistanceLodSettings {
  TArray<FRenderingDistanceLodStageSettings> Stages;
};


inline bool operator==(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return Left.R == Right.R && Left.G == Right.G && Left.B == Right.B;
}

inline bool operator!=(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureHashSettings &Left,
                       const FRenderingTextureHashSettings &Right) {
  return Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.SaltMultiplier == Right.SaltMultiplier &&
         Left.XorShift == Right.XorShift;
}

inline bool operator!=(const FRenderingTextureHashSettings &Left,
                       const FRenderingTextureHashSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingConsoleVariableSettings &Left,
                       const FRenderingConsoleVariableSettings &Right) {
  return Left.Name == Right.Name && Left.ValueKind == Right.ValueKind &&
         Left.ProfileField == Right.ProfileField &&
         Left.IntValue == Right.IntValue &&
         FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue);
}

inline bool operator!=(const FRenderingConsoleVariableSettings &Left,
                       const FRenderingConsoleVariableSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTexturePredicateSettings &Left,
                       const FRenderingTexturePredicateSettings &Right) {
  return Left.Kind == Right.Kind && Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.NoiseMultiplier == Right.NoiseMultiplier &&
         Left.XDivisor == Right.XDivisor &&
         Left.YDivisor == Right.YDivisor && Left.Modulus == Right.Modulus &&
         Left.Equals == Right.Equals && Left.LessThan == Right.LessThan;
}

inline bool operator!=(const FRenderingTexturePredicateSettings &Left,
                       const FRenderingTexturePredicateSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureColorResultSettings &Left,
                       const FRenderingTextureColorResultSettings &Right) {
  return Left.Kind == Right.Kind && Left.Color == Right.Color &&
         Left.ColorA == Right.ColorA && Left.ColorB == Right.ColorB &&
         Left.NumeratorBase == Right.NumeratorBase &&
         Left.NumeratorNoiseModulus == Right.NumeratorNoiseModulus &&
         Left.Denominator == Right.Denominator;
}

inline bool operator!=(const FRenderingTextureColorResultSettings &Left,
                       const FRenderingTextureColorResultSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureRuleSettings &Left,
                       const FRenderingTextureRuleSettings &Right) {
  return Left.Predicate == Right.Predicate && Left.Result == Right.Result;
}

inline bool operator!=(const FRenderingTextureRuleSettings &Left,
                       const FRenderingTextureRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTexturePaletteSettings &Left,
                       const FRenderingTexturePaletteSettings &Right) {
  return Left.Texture == Right.Texture && Left.Rules == Right.Rules;
}

inline bool operator!=(const FRenderingTexturePaletteSettings &Left,
                       const FRenderingTexturePaletteSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingRuntimeSettings &Left,
                       const FRenderingRuntimeSettings &Right) {
  return Left.TextureChannels == Right.TextureChannels &&
         Left.TextureAlpha == Right.TextureAlpha &&
         Left.TextureCacheKeyFormat == Right.TextureCacheKeyFormat &&
         Left.MaterialTextureParameter == Right.MaterialTextureParameter &&
         Left.TextureHash == Right.TextureHash &&
         Left.ConsoleVariables == Right.ConsoleVariables &&
         Left.TexturePalettes == Right.TexturePalettes;
}

inline bool operator!=(const FRenderingRuntimeSettings &Left,
                       const FRenderingRuntimeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingDistanceLodStageSettings &Left,
                       const FRenderingDistanceLodStageSettings &Right) {
  return Left.Id == Right.Id &&
         FMath::IsNearlyEqual(Left.MaxDistance, Right.MaxDistance) &&
         Left.StaticMeshForcedLodModel == Right.StaticMeshForcedLodModel &&
         Left.SkeletalMeshForcedLodModel == Right.SkeletalMeshForcedLodModel &&
         Left.SkeletalMeshMinLodModel == Right.SkeletalMeshMinLodModel &&
         FMath::IsNearlyEqual(Left.CullDistance, Right.CullDistance) &&
         FMath::IsNearlyEqual(Left.ActorTickIntervalSeconds,
                              Right.ActorTickIntervalSeconds) &&
         Left.bStaticVisible == Right.bStaticVisible &&
         Left.bDynamicVisible == Right.bDynamicVisible &&
         Left.bLabelsVisible == Right.bLabelsVisible &&
         Left.bAnimated == Right.bAnimated &&
         Left.bUpdateRateOptimizationsEnabled ==
             Right.bUpdateRateOptimizationsEnabled &&
         Left.bPatrolEnabled == Right.bPatrolEnabled &&
         Left.bCollisionEnabled == Right.bCollisionEnabled &&
         Left.bCastShadow == Right.bCastShadow;
}

inline bool operator!=(const FRenderingDistanceLodStageSettings &Left,
                       const FRenderingDistanceLodStageSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingDistanceLodSettings &Left,
                       const FRenderingDistanceLodSettings &Right) {
  return Left.Stages == Right.Stages;
}

inline bool operator!=(const FRenderingDistanceLodSettings &Left,
                       const FRenderingDistanceLodSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
