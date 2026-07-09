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

struct FConsoleIntegerProfileSettings {
  FString AntiAliasingMethod;
  FString PostProcessAaQuality;
  FString ShadowCascades;
  FString ShadowMaxResolution;
};


struct FConsoleFloatProfileSettings {
  FString ScreenPercentage;
  FString ViewDistanceScale;
  FString FoliageDensityScale;
  FString GrassDensityScale;
};


struct FConsoleValueSettings {
  FString FixedInt;
  FString ProfileInt;
  FString FixedFloat;
  FString ProfileFloat;
};


struct FRenderingConsoleSettings {
  FConsoleIntegerProfileSettings IntegerProfile;
  FConsoleFloatProfileSettings FloatProfile;
  FConsoleValueSettings Value;
};


struct FPredicateTermSettings {
  int32 XMultiplier;
  int32 YMultiplier;
  int32 NoiseMultiplier;
  int32 XDivisor;
  int32 YDivisor;
};


struct FPredicateComparisonSettings {
  int32 Modulus;
  int32 Equals;
  int32 LessThan;
};


struct FPredicateSettings {
  FString Kind;
  FPredicateTermSettings Term;
  FPredicateComparisonSettings Comparison;
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


struct FFormatSettings {
  FString CacheKeyFormat;
  FString MaterialParameter;
};


struct FBufferSettings {
  int32 Channels;
  int32 Alpha;
  int32 MipIndex;
  int32 MaterialSlotIndex;
};


struct FRuleNameSettings {
  FString PredicateAlways;
  FString PredicateModEquals;
  FString PredicateModLessThan;
  FString ResultSolid;
  FString ResultMix;
};


struct FRenderingSettings {
  FFormatSettings Format;
  FBufferSettings Buffer;
  FRuleNameSettings RuleNames;
  FRenderingConsoleSettings Console;
  FHashSettings TextureHash;
  TArray<FRenderingConsoleVariableSettings> ConsoleVariables;
  TArray<FPaletteSettings> TexturePalettes;
};

inline bool operator==(const FRenderingRgbSettings &Left, const FRenderingRgbSettings &Right) {
  return Left.R == Right.R && Left.G == Right.G && Left.B == Right.B;
}
inline bool operator!=(const FRenderingRgbSettings &Left, const FRenderingRgbSettings &Right) { return !(Left == Right); }

inline bool operator==(const FHashSettings &Left, const FHashSettings &Right) {
  return Left.XMultiplier == Right.XMultiplier && Left.YMultiplier == Right.YMultiplier &&
         Left.SaltMultiplier == Right.SaltMultiplier && Left.XorShift == Right.XorShift;
}
inline bool operator!=(const FHashSettings &Left, const FHashSettings &Right) { return !(Left == Right); }

inline bool operator==(const FRenderingConsoleVariableSettings &Left, const FRenderingConsoleVariableSettings &Right) {
  return Left.Name == Right.Name && Left.ValueKind == Right.ValueKind &&
         Left.ProfileField == Right.ProfileField && Left.IntValue == Right.IntValue &&
         FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue);
}
inline bool operator!=(const FRenderingConsoleVariableSettings &Left, const FRenderingConsoleVariableSettings &Right) { return !(Left == Right); }

inline bool operator==(const FConsoleIntegerProfileSettings &Left, const FConsoleIntegerProfileSettings &Right) {
  return Left.AntiAliasingMethod == Right.AntiAliasingMethod &&
         Left.PostProcessAaQuality == Right.PostProcessAaQuality &&
         Left.ShadowCascades == Right.ShadowCascades &&
         Left.ShadowMaxResolution == Right.ShadowMaxResolution;
}
inline bool operator!=(const FConsoleIntegerProfileSettings &Left, const FConsoleIntegerProfileSettings &Right) { return !(Left == Right); }

inline bool operator==(const FConsoleFloatProfileSettings &Left, const FConsoleFloatProfileSettings &Right) {
  return Left.ScreenPercentage == Right.ScreenPercentage &&
         Left.ViewDistanceScale == Right.ViewDistanceScale &&
         Left.FoliageDensityScale == Right.FoliageDensityScale &&
         Left.GrassDensityScale == Right.GrassDensityScale;
}
inline bool operator!=(const FConsoleFloatProfileSettings &Left, const FConsoleFloatProfileSettings &Right) { return !(Left == Right); }

inline bool operator==(const FConsoleValueSettings &Left, const FConsoleValueSettings &Right) {
  return Left.FixedInt == Right.FixedInt && Left.ProfileInt == Right.ProfileInt &&
         Left.FixedFloat == Right.FixedFloat && Left.ProfileFloat == Right.ProfileFloat;
}
inline bool operator!=(const FConsoleValueSettings &Left, const FConsoleValueSettings &Right) { return !(Left == Right); }

inline bool operator==(const FRenderingConsoleSettings &Left, const FRenderingConsoleSettings &Right) {
  return Left.IntegerProfile == Right.IntegerProfile && Left.FloatProfile == Right.FloatProfile &&
         Left.Value == Right.Value;
}
inline bool operator!=(const FRenderingConsoleSettings &Left, const FRenderingConsoleSettings &Right) { return !(Left == Right); }

inline bool operator==(const FPredicateTermSettings &Left, const FPredicateTermSettings &Right) {
  return Left.XMultiplier == Right.XMultiplier && Left.YMultiplier == Right.YMultiplier &&
         Left.NoiseMultiplier == Right.NoiseMultiplier && Left.XDivisor == Right.XDivisor &&
         Left.YDivisor == Right.YDivisor;
}
inline bool operator!=(const FPredicateTermSettings &Left, const FPredicateTermSettings &Right) { return !(Left == Right); }

inline bool operator==(const FPredicateComparisonSettings &Left, const FPredicateComparisonSettings &Right) {
  return Left.Modulus == Right.Modulus && Left.Equals == Right.Equals && Left.LessThan == Right.LessThan;
}
inline bool operator!=(const FPredicateComparisonSettings &Left, const FPredicateComparisonSettings &Right) { return !(Left == Right); }

inline bool operator==(const FPredicateSettings &Left, const FPredicateSettings &Right) {
  return Left.Kind == Right.Kind && Left.Term == Right.Term && Left.Comparison == Right.Comparison;
}
inline bool operator!=(const FPredicateSettings &Left, const FPredicateSettings &Right) { return !(Left == Right); }

inline bool operator==(const FColorResultSettings &Left, const FColorResultSettings &Right) {
  return Left.Kind == Right.Kind && Left.Color == Right.Color && Left.ColorA == Right.ColorA &&
         Left.ColorB == Right.ColorB && Left.NumeratorBase == Right.NumeratorBase &&
         Left.NumeratorNoiseModulus == Right.NumeratorNoiseModulus &&
         Left.Denominator == Right.Denominator;
}
inline bool operator!=(const FColorResultSettings &Left, const FColorResultSettings &Right) { return !(Left == Right); }

inline bool operator==(const FRuleSettings &Left, const FRuleSettings &Right) {
  return Left.Predicate == Right.Predicate && Left.Result == Right.Result;
}
inline bool operator!=(const FRuleSettings &Left, const FRuleSettings &Right) { return !(Left == Right); }

inline bool operator==(const FPaletteSettings &Left, const FPaletteSettings &Right) {
  return Left.Texture == Right.Texture && Left.Rules == Right.Rules;
}
inline bool operator!=(const FPaletteSettings &Left, const FPaletteSettings &Right) { return !(Left == Right); }

inline bool operator==(const FFormatSettings &Left, const FFormatSettings &Right) {
  return Left.CacheKeyFormat == Right.CacheKeyFormat && Left.MaterialParameter == Right.MaterialParameter;
}
inline bool operator!=(const FFormatSettings &Left, const FFormatSettings &Right) { return !(Left == Right); }

inline bool operator==(const FBufferSettings &Left, const FBufferSettings &Right) {
  return Left.Channels == Right.Channels && Left.Alpha == Right.Alpha &&
         Left.MipIndex == Right.MipIndex && Left.MaterialSlotIndex == Right.MaterialSlotIndex;
}
inline bool operator!=(const FBufferSettings &Left, const FBufferSettings &Right) { return !(Left == Right); }

inline bool operator==(const FRuleNameSettings &Left, const FRuleNameSettings &Right) {
  return Left.PredicateAlways == Right.PredicateAlways &&
         Left.PredicateModEquals == Right.PredicateModEquals &&
         Left.PredicateModLessThan == Right.PredicateModLessThan &&
         Left.ResultSolid == Right.ResultSolid && Left.ResultMix == Right.ResultMix;
}
inline bool operator!=(const FRuleNameSettings &Left, const FRuleNameSettings &Right) { return !(Left == Right); }

inline bool operator==(const FRenderingSettings &Left, const FRenderingSettings &Right) {
  return Left.Format == Right.Format && Left.Buffer == Right.Buffer &&
         Left.RuleNames == Right.RuleNames && Left.Console == Right.Console &&
         Left.TextureHash == Right.TextureHash && Left.ConsoleVariables == Right.ConsoleVariables &&
         Left.TexturePalettes == Right.TexturePalettes;
}
inline bool operator!=(const FRenderingSettings &Left, const FRenderingSettings &Right) { return !(Left == Right); }

} // namespace Data
} // namespace Game
} // namespace ForbocAI
