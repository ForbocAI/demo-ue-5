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
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"
#include "Features/Systems/Speech/SpeechAdapters.h"

namespace {

ForbocAI::Game::Data::FSpeechRuntimeSettings LoadSpeechSettings() {
  return ForbocAI::Game::Data::RuntimeSettingsAdapters::
      LoadRuntimeSettings()
          .SpeechRuntime;
}

TMap<FString, FVisemeMapping> LoadVisemeMap(
    const ForbocAI::Game::Data::FSpeechRuntimeSettings &Settings) {
  return SpeechOps::VisemeMapFromSettings(Settings);
}

} // namespace

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeEstimation,
    "ForbocAI.Speech.PhonemeEstimation",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechPhonemeEstimation::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechRuntimeSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(TEXT("Hello"), Settings);

  TestTrue(TEXT("Produces phonemes for 'Hello'"), Phonemes.Num() > 0);

  // 'H', 'E' (vowel), 'L', 'L', 'O' (vowel) = 5 phonemes
  TestEqual(TEXT("Correct phoneme count for 'Hello'"), Phonemes.Num(), 5);

  // First phoneme starts at 0
  TestEqual(TEXT("First phoneme starts at 0"), Phonemes[0].StartTime,
            0.0f);

  // All phonemes have positive duration
  const auto CheckDurationsRecursive =
      [this, &Phonemes](int32 Idx, const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : (TestTrue(
                      FString::Printf(TEXT("Phoneme[%d] has positive "
                                           "duration"),
                                      Idx),
                      Phonemes[Idx].Duration > 0.0f),
                  Self(Idx + 1, Self));
  };
  CheckDurationsRecursive(0, CheckDurationsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeWithSpaces,
    "ForbocAI.Speech.PhonemeEstimationWithSpaces",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechPhonemeWithSpaces::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechRuntimeSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(TEXT("Hi there"), Settings);

  TestTrue(TEXT("Produces phonemes for 'Hi there'"), Phonemes.Num() > 0);

  // Space produces a SIL phoneme with shorter duration
  bool bHasSilence = false;
  const auto FindSilRecursive =
      [&Phonemes, &bHasSilence, &Settings](int32 Idx,
                                           const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : (Phonemes[Idx].Phoneme == Settings.SilencePhoneme
                      ? (void)(bHasSilence = true)
                      : void(),
                  Self(Idx + 1, Self));
  };
  FindSilRecursive(0, FindSilRecursive);

  TestTrue(TEXT("Space produces SIL phoneme"), bHasSilence);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeMapCompleteness,
    "ForbocAI.Speech.VisemeMapCompleteness",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeMapCompleteness::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechRuntimeSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);

  TestEqual(TEXT("Map matches authored viseme mappings"), Map.Num(),
            Settings.VisemeMappings.Num());

  const auto CheckVowelsRecursive = [this, &Map, &Settings](
                                        int32 Idx,
                                        const auto &Self) -> void {
    return Idx >= Settings.VowelPhonemes.Num()
               ? void()
               : [&]() {
                   const ForbocAI::Game::Data::FSpeechVowelPhonemeSettings
                       Vowel = Settings.VowelPhonemes[Idx];
                   TestTrue(
                       FString::Printf(TEXT("Vowel %s mapped"),
                                       *Vowel.Character),
                       Map.Contains(Vowel.Phoneme));
                   Self(Idx + 1, Self);
                 }();
  };
  CheckVowelsRecursive(0, CheckVowelsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechActiveVisemeAtTime,
    "ForbocAI.Speech.ActiveVisemeAtTime",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechRuntimeSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);

  // Create a simple phoneme timeline: AA at 0.0-0.1, SIL at 0.1-0.15,
  // EH at 0.15-0.25
  TArray<FPhonemeEvent> Phonemes;
  Phonemes.Add({TEXT("AA"), 0.0f, 0.1f});
  Phonemes.Add({TEXT("SIL"), 0.1f, 0.05f});
  Phonemes.Add({TEXT("EH"), 0.15f, 0.1f});
  const FVisemeMapping ExpectedAa =
      SpeechOps::RequiredVisemeForPhoneme(TEXT("AA"), Map);
  const FVisemeMapping ExpectedSilence =
      SpeechOps::RequiredVisemeForPhoneme(Settings.SilencePhoneme, Map);
  const FVisemeMapping ExpectedEh =
      SpeechOps::RequiredVisemeForPhoneme(TEXT("EH"), Map);

  // At t=0.05, should be in AA -> viseme_aa
  const FVisemeMapping V1 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.05f, Map, Rest);
  TestEqual(TEXT("At 0.05s: viseme_aa"), V1.MorphTargetName,
            ExpectedAa.MorphTargetName);
  TestTrue(TEXT("At 0.05s: weight > 0"), V1.BlendWeight > 0.0f);

  // At t=0.12, should be in SIL -> viseme_sil
  const FVisemeMapping V2 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.12f, Map, Rest);
  TestEqual(TEXT("At 0.12s: viseme_sil"), V2.MorphTargetName,
            ExpectedSilence.MorphTargetName);

  // At t=0.20, should be in EH -> viseme_E
  const FVisemeMapping V3 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.20f, Map, Rest);
  TestEqual(TEXT("At 0.20s: viseme_E"), V3.MorphTargetName,
            ExpectedEh.MorphTargetName);

  // At t=0.30 (past all phonemes), should be silence
  const FVisemeMapping V4 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.30f, Map, Rest);
  TestEqual(TEXT("At 0.30s: silence"), V4.MorphTargetName,
            Rest.MorphTargetName);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeLookup,
    "ForbocAI.Speech.VisemeLookupUnknown",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeLookup::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechRuntimeSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);

  // Known phoneme
  const func::Maybe<FVisemeMapping> Known =
      SpeechOps::LookupViseme(TEXT("AA"), Map);
  const FVisemeMapping Expected =
      SpeechOps::RequiredVisemeForPhoneme(TEXT("AA"), Map);
  check(Known.hasValue);
  TestEqual(TEXT("AA maps to viseme_aa"), Known.value.MorphTargetName,
            Expected.MorphTargetName);

  const func::Maybe<FVisemeMapping> Unknown =
      SpeechOps::LookupViseme(TEXT("UNKNOWN"), Map);
  TestFalse(TEXT("Unknown phoneme is unmapped"), Unknown.hasValue);

  return true;
}
