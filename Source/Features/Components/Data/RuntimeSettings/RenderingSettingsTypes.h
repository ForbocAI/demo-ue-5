#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FRenderingAssetPathSettings {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
};


struct FRenderingProfileSettings {
  float TimeOfDayHour = 0.0f;
  int32 AntiAliasingMethod = 0;
  int32 PostProcessAAQuality = 0;
  float ScreenPercentage = 0.0f;
  float ViewDistanceScale = 0.0f;
  float FoliageDensityScale = 0.0f;
  float GrassDensityScale = 0.0f;
  float SunPitchDegrees = 0.0f;
  float SunYawDegrees = 0.0f;
  float DirectionalLightIntensity = 0.0f;
  float DirectionalLightSourceAngle = 0.0f;
  int32 ShadowCascades = 0;
  int32 ShadowMaxResolution = 0;
};


struct FRenderingTextureSpecSettings {
  FString Texture;
  FString Name;
  FString Use;
  int32 Size = 0;
};


struct FRenderingRgbSettings {
  int32 R = 0;
  int32 G = 0;
  int32 B = 0;
};


struct FRenderingTextureHashSettings {
  int32 XMultiplier = 0;
  int32 YMultiplier = 0;
  int32 SaltMultiplier = 0;
  int32 XorShift = 0;
};


struct FRenderingConsoleVariableSettings {
  FString Name;
  FString ValueKind;
  FString ProfileField;
  int32 IntValue = 0;
  float FloatValue = 0.0f;
};


struct FRenderingTexturePredicateSettings {
  FString Kind;
  int32 XMultiplier = 0;
  int32 YMultiplier = 0;
  int32 NoiseMultiplier = 0;
  int32 XDivisor = 0;
  int32 YDivisor = 0;
  int32 Modulus = 0;
  int32 Equals = 0;
  int32 LessThan = 0;
};


struct FRenderingTextureColorResultSettings {
  FString Kind;
  FRenderingRgbSettings Color;
  FRenderingRgbSettings ColorA;
  FRenderingRgbSettings ColorB;
  int32 NumeratorBase = 0;
  int32 NumeratorNoiseModulus = 0;
  int32 Denominator = 0;
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
  int32 TextureChannels = 0;
  int32 TextureAlpha = 0;
  FString TextureCacheKeyFormat;
  FString MaterialTextureParameter;
  FRenderingTextureHashSettings TextureHash;
  TArray<FRenderingConsoleVariableSettings> ConsoleVariables;
  TArray<FRenderingTexturePaletteSettings> TexturePalettes;
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

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
