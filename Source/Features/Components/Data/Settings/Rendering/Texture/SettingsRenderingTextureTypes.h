#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSpecSettings {
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


struct FHashSettings {
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


struct FPredicateSettings {
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


struct FColorResultSettings {
  FString Kind;
  FRenderingRgbSettings Color;
  FRenderingRgbSettings ColorA;
  FRenderingRgbSettings ColorB;
  int32 NumeratorBase;
  int32 NumeratorNoiseModulus;
  int32 Denominator;
};


struct FRuleSettings {
  FPredicateSettings Predicate;
  FColorResultSettings Result;
};


struct FPaletteSettings {
  FString Texture;
  TArray<FRuleSettings> Rules;
};


struct FRenderingSettings {
  int32 TextureChannels;
  int32 TextureAlpha;
  FString TextureCacheKeyFormat;
  FString MaterialTextureParameter;
  FString PredicateAlwaysKind;
  FString PredicateModEqualsKind;
  FString PredicateModLessThanKind;
  FString ResultSolidKind;
  FString ResultMixKind;
  int32 TextureMipIndex;
  int32 TextureMaterialSlotIndex;
  FHashSettings TextureHash;
  TArray<FRenderingConsoleVariableSettings> ConsoleVariables;
  TArray<FPaletteSettings> TexturePalettes;
};

inline bool operator==(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return Left.R == Right.R && Left.G == Right.G && Left.B == Right.B;
}

inline bool operator!=(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHashSettings &Left,
                       const FHashSettings &Right) {
  return Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.SaltMultiplier == Right.SaltMultiplier &&
         Left.XorShift == Right.XorShift;
}

inline bool operator!=(const FHashSettings &Left,
                       const FHashSettings &Right) {
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

inline bool operator==(const FPredicateSettings &Left,
                       const FPredicateSettings &Right) {
  return Left.Kind == Right.Kind && Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.NoiseMultiplier == Right.NoiseMultiplier &&
         Left.XDivisor == Right.XDivisor &&
         Left.YDivisor == Right.YDivisor && Left.Modulus == Right.Modulus &&
         Left.Equals == Right.Equals && Left.LessThan == Right.LessThan;
}

inline bool operator!=(const FPredicateSettings &Left,
                       const FPredicateSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FColorResultSettings &Left,
                       const FColorResultSettings &Right) {
  return Left.Kind == Right.Kind && Left.Color == Right.Color &&
         Left.ColorA == Right.ColorA && Left.ColorB == Right.ColorB &&
         Left.NumeratorBase == Right.NumeratorBase &&
         Left.NumeratorNoiseModulus == Right.NumeratorNoiseModulus &&
         Left.Denominator == Right.Denominator;
}

inline bool operator!=(const FColorResultSettings &Left,
                       const FColorResultSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuleSettings &Left,
                       const FRuleSettings &Right) {
  return Left.Predicate == Right.Predicate && Left.Result == Right.Result;
}

inline bool operator!=(const FRuleSettings &Left,
                       const FRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPaletteSettings &Left,
                       const FPaletteSettings &Right) {
  return Left.Texture == Right.Texture && Left.Rules == Right.Rules;
}

inline bool operator!=(const FPaletteSettings &Left,
                       const FPaletteSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingSettings &Left,
                       const FRenderingSettings &Right) {
  return Left.TextureChannels == Right.TextureChannels &&
         Left.TextureAlpha == Right.TextureAlpha &&
         Left.TextureCacheKeyFormat == Right.TextureCacheKeyFormat &&
         Left.MaterialTextureParameter == Right.MaterialTextureParameter &&
         Left.PredicateAlwaysKind == Right.PredicateAlwaysKind &&
         Left.PredicateModEqualsKind == Right.PredicateModEqualsKind &&
         Left.PredicateModLessThanKind == Right.PredicateModLessThanKind &&
         Left.ResultSolidKind == Right.ResultSolidKind &&
         Left.ResultMixKind == Right.ResultMixKind &&
         Left.TextureMipIndex == Right.TextureMipIndex &&
         Left.TextureMaterialSlotIndex == Right.TextureMaterialSlotIndex &&
         Left.TextureHash == Right.TextureHash &&
         Left.ConsoleVariables == Right.ConsoleVariables &&
         Left.TexturePalettes == Right.TexturePalettes;
}

inline bool operator!=(const FRenderingSettings &Left,
                       const FRenderingSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
