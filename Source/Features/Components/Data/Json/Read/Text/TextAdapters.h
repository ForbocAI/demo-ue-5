#pragma once

#include "Features/Components/Data/Json/Read/Field/ReadFieldAdapters.h"

namespace ForbocAI::Game::Data::JsonAdapters {

template <typename Output> struct TValueDeclaration {
  FString Text;
  Output Value;

  /** User Story: As a JSON text registry consumer, I need typed text declarations through a stable signature so authored labels map to values explicitly. @fn TValueDeclaration(const char *InText, Output InValue) */
  TValueDeclaration(const char *InText, Output InValue)
      : Text(FString(UTF8_TO_TCHAR(InText))), Value(InValue) {}
};

/** User Story: As a JSON text registry consumer, I need to parse registered values through a stable signature so authored labels resolve compositionally. @fn template <typename Output> func::Maybe<Output> ReadTextValue(const FString &Text, const TArray<TValueDeclaration<Output>> &Declarations) */
template <typename Output>
func::Maybe<Output>
ReadTextValue(const FString &Text,
              const TArray<TValueDeclaration<Output>> &Declarations) {
  const FString LowerText = Text.ToLower();
  return func::fold_indexed(
      Declarations, static_cast<size_t>(Declarations.Num()),
      func::nothing<Output>(),
      [LowerText](const func::Maybe<Output> &Current,
                  const TValueDeclaration<Output> &Declaration) {
        return Current.hasValue || Declaration.Text != LowerText
                   ? Current
                   : func::just(Declaration.Value);
      });
}

/** User Story: As a JSON text registry consumer, I need to parse initializer-list declarations through a stable signature so small registries remain composable. @fn template <typename Output> func::Maybe<Output> ReadTextValue(const FString &Text, std::initializer_list<TValueDeclaration<Output>> Declarations) */
template <typename Output>
func::Maybe<Output>
ReadTextValue(const FString &Text,
              std::initializer_list<TValueDeclaration<Output>>
                  Declarations) {
  return ReadTextValue<Output>(
      Text, TArray<TValueDeclaration<Output>>(Declarations));
}

template <typename Output> struct TJsonTextValueRegistry;

/** User Story: As a JSON text registry consumer, I need to read the canonical registry through a stable signature so value parsing uses one source of truth. @fn template <typename Output> func::Maybe<Output> ReadRegisteredTextValue(const FString &Text) */
template <typename Output>
func::Maybe<Output> ReadRegisteredTextValue(const FString &Text) {
  return ReadTextValue<Output>(Text,
                               TJsonTextValueRegistry<Output>::Values());
}

/** User Story: As a JSON text registry consumer, I need to require a registered value through a stable signature so invalid authored labels fail at the data boundary. @fn template <typename Output> Output RequireRegisteredTextValue(const FString &Text, const TCHAR *Label) */
template <typename Output>
Output RequireRegisteredTextValue(const FString &Text, const TCHAR *Label) {
  const func::Maybe<Output> Parsed = ReadRegisteredTextValue<Output>(Text);
  checkf(Parsed.hasValue, TEXT("Invalid JSON text value for %s: %s"), Label,
         *Text);
  return Parsed.value;
}

} // namespace ForbocAI::Game::Data::JsonAdapters
