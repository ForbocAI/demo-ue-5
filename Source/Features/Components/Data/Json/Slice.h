#pragma once

#include "CoreMinimal.h"

// Json field name reducers: pure state transitions that convert PascalCase
// field atoms to snake_case JSON field names. No IO, no side effects.
// These are reducer-owned because they are "given input, compute output"
// without reading store state.

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonReducers {

inline FString AppendJsonSettingsFieldChar(const FString &Value, TCHAR Char) {
  FString Next = Value;
  Next.AppendChar(Char);
  return Next;
}

inline FString ReduceJsonSettingsFieldCharWhen(FString Value,
                                               bool bAppend, TCHAR Char) {
  return bAppend ? AppendJsonSettingsFieldChar(Value, Char) : Value;
}

inline FString ReduceSettingsFieldAtomFrom(FString Acc, const FString &Atom,
                                           int32 Index, int32 StartIndex) {
  const TCHAR Current = Index < Atom.Len() ? Atom[Index] : TCHAR('\0');
  const bool bUpper = FChar::IsUpper(Current);
  const bool bPreviousLowerOrDigit =
      Index > StartIndex &&
      (FChar::IsLower(Atom[Index - 1]) || FChar::IsDigit(Atom[Index - 1]));
  const bool bNextLower =
      Index + 1 < Atom.Len() && FChar::IsLower(Atom[Index + 1]);
  const bool bSeparate =
      bUpper && Index > StartIndex && (bPreviousLowerOrDigit || bNextLower);
  return Index >= Atom.Len()
             ? Acc
             : ReduceSettingsFieldAtomFrom(
                   ReduceJsonSettingsFieldCharWhen(
                       ReduceJsonSettingsFieldCharWhen(std::move(Acc), bSeparate,
                                                       TCHAR('_')),
                       true, FChar::ToLower(Current)),
                   Atom, Index + 1, StartIndex);
}

inline FString ReduceSettingsFieldAtom(FString Acc, const FString &Atom) {
  const int32 StartIndex =
      Atom.StartsWith(TEXT("b")) && Atom.Len() > 1 && FChar::IsUpper(Atom[1])
          ? 1
          : 0;
  return ReduceSettingsFieldAtomFrom(std::move(Acc), Atom, StartIndex,
                                     StartIndex);
}

inline FString SettingsFieldName(const TCHAR *Atom) {
  return ReduceSettingsFieldAtom(FString(TEXT("")), Atom);
}

inline FString SettingsFieldName(const char *Atom) {
  return ReduceSettingsFieldAtom(FString(TEXT("")), UTF8_TO_TCHAR(Atom));
}

} // namespace JsonReducers
} // namespace Data
} // namespace Game
} // namespace ForbocAI
