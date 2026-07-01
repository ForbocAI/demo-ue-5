#pragma once

#include "CoreMinimal.h"

#include <initializer_list>

namespace frmt {

inline FString Arg(const FString &Value) { return Value; }

inline FString Arg(const TCHAR *Value) { return FString(Value); }

inline FString Arg(TCHAR Value) { return FString(1, &Value); }

inline FString Arg(int32 Value) { return FString::FromInt(Value); }

inline FString Arg(float Value) { return FString::SanitizeFloat(Value); }

inline FString Arg(double Value) { return FString::SanitizeFloat(Value); }

inline TArray<FString> Args(std::initializer_list<FString> Values) {
  return TArray<FString>(Values);
}

inline int32 EarlierTokenIndex(int32 Left, int32 Right) {
  return Left == INDEX_NONE
             ? Right
             : (Right == INDEX_NONE ? Left : FMath::Min(Left, Right));
}

inline int32 FirstRuntimeFormatTokenIndex(const FString &Pattern) {
  return EarlierTokenIndex(
      EarlierTokenIndex(Pattern.Find(TEXT("%s")), Pattern.Find(TEXT("%d"))),
      EarlierTokenIndex(Pattern.Find(TEXT("%f")), Pattern.Find(TEXT("%c"))));
}

inline FString ReplaceRuntimeFormatToken(const FString &Pattern,
                                         const FString &Value) {
  const int32 TokenIndex = FirstRuntimeFormatTokenIndex(Pattern);
  return TokenIndex == INDEX_NONE
             ? Pattern
             : Pattern.Left(TokenIndex) + Value + Pattern.Mid(TokenIndex + 2);
}

inline FString FormatRuntimeStringAt(const FString &Pattern,
                                     const TArray<FString> &Values,
                                     int32 Index) {
  return Index >= Values.Num()
             ? Pattern
             : FormatRuntimeStringAt(
                   ReplaceRuntimeFormatToken(Pattern, Values[Index]), Values,
                   Index + 1);
}

inline FString RuntimeString(const FString &Pattern,
                             const TArray<FString> &Values) {
  return FormatRuntimeStringAt(Pattern, Values, 0);
}

} // namespace frmt
