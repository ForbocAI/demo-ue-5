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
#include "Features/Systems/Speech/Adapters/AdaptersSlice.h"

namespace {

ForbocAI::Game::Data::FSpeechSettings LoadSpeechSettings() {
  return ForbocAI::Game::Data::SettingsAdapters::
      LoadSettings()
          .Speech;
}

const ForbocAI::Game::Data::FAutomationSettings &
SpeechAutomationSettings() {
  static const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  return Settings.Automation;
}

TMap<FString, FVisemeMapping> LoadVisemeMap(
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  return SpeechOps::VisemeMapFromSettings(Settings);
}

template <typename Item> const Item &RequiredFirst(const TArray<Item> &Items) {
  check(!Items.IsEmpty());
  return Items[int32{}];
}

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

FString Label(const FString &Format, const FString &Value) {
  return FString::Format(*Format, {FStringFormatArg(Value)});
}

FString Label(const FString &Format, int32 Value) {
  return FString::Format(*Format, {FStringFormatArg(Value)});
}

float SampleTime(const FPhonemeEvent &Event, float Ratio) {
  return Event.StartTime + Event.Duration * Ratio;
}

} // namespace
