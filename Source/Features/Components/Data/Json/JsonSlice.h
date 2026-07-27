#pragma once
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "CoreMinimal.h"

// Json field name reducers: pure state transitions that convert PascalCase
// field atoms to snake_case JSON field names. No IO, no side effects.
// These are reducer-owned because they are "given input, compute output"
// without reading store state.

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonReducers {

/** User Story: As a components data json consumer, I need to invoke append json settings field char through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString AppendJsonSettingsFieldChar(const FString &Value, TCHAR Char) */
inline FString AppendJsonSettingsFieldChar(const FString &Value, TCHAR Char) {
  FString Next = Value;
  Next.AppendChar(Char);
  return Next;
}

/** User Story: As a components data json consumer, I need to invoke reduce json settings field char when through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString ReduceJsonSettingsFieldCharWhen(FString Value, bool bAppend, TCHAR Char) */
inline FString ReduceJsonSettingsFieldCharWhen(FString Value,
                                               bool bAppend, TCHAR Char) {
  return bAppend ? AppendJsonSettingsFieldChar(Value, Char) : Value;
}

/** User Story: As a components data json consumer, I need to invoke reduce settings field atom from through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString ReduceSettingsFieldAtomFrom(FString Acc, const FString &Atom, int32 Index, int32 StartIndex) */
inline FString ReduceSettingsFieldAtomFrom(FString Acc, const FString &Atom,
                                           int32 Index, int32 StartIndex) {
  const TCHAR Current = Index < Atom.Len() ? Atom[Index] : TCHAR('\0');
  const bool bUpper = FChar::IsUpper(Current);
  const bool bPreviousLowerOrDigit =
      Index > StartIndex &&
      (FChar::IsLower(Atom[Index - FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4]) || FChar::IsDigit(Atom[Index - FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4]));
  const bool bNextLower =
      Index + FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4 < Atom.Len() && FChar::IsLower(Atom[Index + FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4]);
  const bool bSeparate =
      bUpper && Index > StartIndex && (bPreviousLowerOrDigit || bNextLower);
  return Index >= Atom.Len()
             ? Acc
             : ReduceSettingsFieldAtomFrom(
                   ReduceJsonSettingsFieldCharWhen(
                       ReduceJsonSettingsFieldCharWhen(std::move(Acc), bSeparate,
                                                       TCHAR('_')),
                       true, FChar::ToLower(Current)),
                   Atom, Index + FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4, StartIndex);
}

/** User Story: As a components data json consumer, I need to invoke reduce settings field atom through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString ReduceSettingsFieldAtom(FString Acc, const FString &Atom) */
inline FString ReduceSettingsFieldAtom(FString Acc, const FString &Atom) {
  const int32 StartIndex =
      Atom.StartsWith(TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV124E246F8B83)) && Atom.Len() > FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4 && FChar::IsUpper(Atom[FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4])
          ? FORBOCAI_DEMOUE5_AUTHORED_NUMBERV0063C33F45B4
          : FORBOCAI_DEMOUE5_AUTHORED_NUMBERV60732C8368BA;
  return ReduceSettingsFieldAtomFrom(std::move(Acc), Atom, StartIndex,
                                     StartIndex);
}

/** User Story: As a components data json consumer, I need to invoke settings field name through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString SettingsFieldName(const TCHAR *Atom) */
inline FString SettingsFieldName(const TCHAR *Atom) {
  return ReduceSettingsFieldAtom(FString(TEXT("")), Atom);
}

/** User Story: As a components data json consumer, I need to invoke settings field name through a stable signature so the components data json workflow remains explicit and composable. @fn inline FString SettingsFieldName(const char *Atom) */
inline FString SettingsFieldName(const char *Atom) {
  return ReduceSettingsFieldAtom(FString(TEXT("")), UTF8_TO_TCHAR(Atom));
}

} // namespace JsonReducers
} // namespace Data
} // namespace Game
} // namespace ForbocAI
