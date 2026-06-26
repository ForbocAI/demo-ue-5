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
#include "Features/Systems/Speech/SpeechComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeEstimation,
    "ForbocAI.Speech.PhonemeEstimation",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechPhonemeEstimation::RunTest(const FString &Parameters) {
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(TEXT("Hello"));

  TestTrue(TEXT("Produces phonemes for 'Hello'"), Phonemes.Num() > 0);

  // 'H', 'E' (vowel -> EE), 'L', 'L', 'O' (vowel -> OO) = 5 phonemes
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
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(TEXT("Hi there"));

  TestTrue(TEXT("Produces phonemes for 'Hi there'"), Phonemes.Num() > 0);

  // Space produces a SIL phoneme with shorter duration
  bool bHasSilence = false;
  const auto FindSilRecursive =
      [&Phonemes, &bHasSilence](int32 Idx, const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : (Phonemes[Idx].Phoneme == TEXT("SIL")
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
  const TMap<FString, FVisemeMapping> Map =
      SpeechOps::DefaultVisemeMap();

  // Should have at least 35 phoneme-to-viseme mappings
  TestTrue(TEXT("Map has 35+ entries"), Map.Num() >= 35);

  // All vowels present
  const TArray<FString> Vowels = {TEXT("AA"), TEXT("AE"), TEXT("AH"),
                                   TEXT("EH"), TEXT("IH"), TEXT("IY"),
                                   TEXT("UH"), TEXT("UW")};

  const auto CheckVowelsRecursive =
      [this, &Map, &Vowels](int32 Idx, const auto &Self) -> void {
    return Idx >= Vowels.Num()
               ? void()
               : (TestTrue(
                      FString::Printf(TEXT("Vowel %s mapped"),
                                      *Vowels[Idx]),
                      Map.Contains(Vowels[Idx])),
                  Self(Idx + 1, Self));
  };
  CheckVowelsRecursive(0, CheckVowelsRecursive);

  // Core consonants present
  const TArray<FString> Consonants = {TEXT("B"), TEXT("D"), TEXT("F"),
                                       TEXT("K"), TEXT("M"), TEXT("N"),
                                       TEXT("P"), TEXT("S"), TEXT("T")};

  const auto CheckConsonantsRecursive =
      [this, &Map, &Consonants](int32 Idx, const auto &Self) -> void {
    return Idx >= Consonants.Num()
               ? void()
               : (TestTrue(
                      FString::Printf(TEXT("Consonant %s mapped"),
                                      *Consonants[Idx]),
                      Map.Contains(Consonants[Idx])),
                  Self(Idx + 1, Self));
  };
  CheckConsonantsRecursive(0, CheckConsonantsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechActiveVisemeAtTime,
    "ForbocAI.Speech.ActiveVisemeAtTime",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) {
  const TMap<FString, FVisemeMapping> Map =
      SpeechOps::DefaultVisemeMap();

  // Create a simple phoneme timeline: AA at 0.0-0.1, SIL at 0.1-0.15,
  // EH at 0.15-0.25
  TArray<FPhonemeEvent> Phonemes;
  Phonemes.Add({TEXT("AA"), 0.0f, 0.1f});
  Phonemes.Add({TEXT("SIL"), 0.1f, 0.05f});
  Phonemes.Add({TEXT("EH"), 0.15f, 0.1f});

  // At t=0.05, should be in AA -> viseme_aa
  const FVisemeMapping V1 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.05f, Map);
  TestEqual(TEXT("At 0.05s: viseme_aa"), V1.MorphTargetName,
            TEXT("viseme_aa"));
  TestTrue(TEXT("At 0.05s: weight > 0"), V1.BlendWeight > 0.0f);

  // At t=0.12, should be in SIL -> viseme_sil
  const FVisemeMapping V2 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.12f, Map);
  TestEqual(TEXT("At 0.12s: viseme_sil"), V2.MorphTargetName,
            TEXT("viseme_sil"));

  // At t=0.20, should be in EH -> viseme_E
  const FVisemeMapping V3 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.20f, Map);
  TestEqual(TEXT("At 0.20s: viseme_E"), V3.MorphTargetName,
            TEXT("viseme_E"));

  // At t=0.30 (past all phonemes), should be silence
  const FVisemeMapping V4 =
      SpeechOps::ActiveVisemeAtTime(Phonemes, 0.30f, Map);
  TestEqual(TEXT("At 0.30s: silence"), V4.MorphTargetName,
            TEXT("viseme_sil"));

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeLookup,
    "ForbocAI.Speech.VisemeLookupUnknown",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeLookup::RunTest(const FString &Parameters) {
  const TMap<FString, FVisemeMapping> Map =
      SpeechOps::DefaultVisemeMap();

  // Known phoneme
  const FVisemeMapping Known =
      SpeechOps::LookupViseme(TEXT("AA"), Map);
  TestEqual(TEXT("AA maps to viseme_aa"), Known.MorphTargetName,
            TEXT("viseme_aa"));

  // Unknown phoneme should return silence
  const FVisemeMapping Unknown =
      SpeechOps::LookupViseme(TEXT("UNKNOWN"), Map);
  TestEqual(TEXT("Unknown maps to viseme_sil"), Unknown.MorphTargetName,
            TEXT("viseme_sil"));
  TestEqual(TEXT("Unknown has 0 weight"), Unknown.BlendWeight, 0.0f);

  return true;
}
