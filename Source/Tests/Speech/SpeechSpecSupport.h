#pragma once

/**
 * Speech Component Spec
 *
 * Tests the pure functional parts of the speech system:
 *   1. Phoneme estimation from text
 *   2. Viseme mapping lookup
 *   3. Active viseme at time queries
 *   4. Default viseme map completeness
 *
 * User Story: As the speech system, I need tests covering phoneme
 * estimation and viseme lookup so lip sync produces correct morph
 * target values for any input text.
 */

#include "Misc/AutomationTest.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/Speech/Phoneme/PhonemeSlice.h"

namespace {

/** User Story: As a tests speech consumer, I need to invoke load speech settings through a stable signature so the tests speech workflow remains explicit and composable. @fn ForbocAI::Game::Data::FSpeechSettings LoadSpeechSettings() */
ForbocAI::Game::Data::FSpeechSettings LoadSpeechSettings() {
  return ForbocAI::Game::Data::SettingsAdapters::
      LoadSettings()
          .Speech;
}

/** User Story: As a tests speech consumer, I need to invoke speech automation settings through a stable signature so the tests speech workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FAutomationSettings & SpeechAutomationSettings() */
const ForbocAI::Game::Data::FAutomationSettings &
SpeechAutomationSettings() {
  static const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  return Settings.Automation;
}

/** User Story: As a tests speech consumer, I need to invoke load viseme map through a stable signature so the tests speech workflow remains explicit and composable. @fn TMap<FString, FVisemeMapping> LoadVisemeMap( const ForbocAI::Game::Data::FSpeechSettings &Settings) */
TMap<FString, FVisemeMapping> LoadVisemeMap(
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  return SpeechOps::VisemeMapFromSettings(Settings);
}

/** User Story: As a tests speech consumer, I need to invoke required first through a stable signature so the tests speech workflow remains explicit and composable. @fn template <typename Item> const Item &RequiredFirst(const TArray<Item> &Items) */
template <typename Item> const Item &RequiredFirst(const TArray<Item> &Items) {
  check(!Items.IsEmpty());
  return Items[int32{}];
}

/** User Story: As a tests speech consumer, I need to invoke required non silence mapping through a stable signature so the tests speech workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FVisemeMappingSettings & RequiredNonSilenceMapping( const ForbocAI::Game::Data::FSpeechSettings &Settings) */
const ForbocAI::Game::Data::FVisemeMappingSettings &
RequiredNonSilenceMapping(
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  const func::Maybe<ForbocAI::Game::Data::FVisemeMappingSettings>
      Mapping = func::find_array<
          ForbocAI::Game::Data::FVisemeMappingSettings>(
          Settings.VisemeMappings, [&Settings](
                                       const ForbocAI::Game::Data::
                                           FVisemeMappingSettings &Item) {
            return Item.Phoneme != Settings.SilencePhoneme;
          });
  check(Mapping.hasValue);
  const ForbocAI::Game::Data::FVisemeMappingSettings *Found =
      Settings.VisemeMappings.FindByPredicate(
          [&Mapping](
              const ForbocAI::Game::Data::FVisemeMappingSettings &Item) {
            return Item == Mapping.value;
          });
  check(Found != nullptr);
  return *Found;
}

/** User Story: As a tests speech consumer, I need to invoke label through a stable signature so the tests speech workflow remains explicit and composable. @fn FString Label(const FString &Format, const FString &Value) */
FString Label(const FString &Format, const FString &Value) {
  return FString::Format(*Format, {FStringFormatArg(Value)});
}

/** User Story: As a tests speech consumer, I need to invoke label through a stable signature so the tests speech workflow remains explicit and composable. @fn FString Label(const FString &Format, int32 Value) */
FString Label(const FString &Format, int32 Value) {
  return FString::Format(*Format, {FStringFormatArg(Value)});
}

/** User Story: As a tests speech consumer, I need to invoke sample time through a stable signature so the tests speech workflow remains explicit and composable. @fn float SampleTime(const FPhonemeEvent &Event, float Ratio) */
float SampleTime(const FPhonemeEvent &Event, float Ratio) {
  return Event.StartTime + Event.Duration * Ratio;
}

} // namespace
