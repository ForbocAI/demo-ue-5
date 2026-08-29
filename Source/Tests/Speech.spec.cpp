#include "Tests/Speech/SpeechSpecSupport.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeEstimation,
    SpeechAutomationSettings().Cases.PhonemeEstimationTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FSpeechPhonemeEstimation::RunTest(const FString &Parameters) */
bool FSpeechPhonemeEstimation::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(
          Settings.Automation.Samples.PhonemeEstimationText, Settings);

  TestTrue(Label(
               Settings.Automation.PhonemeAssertions
                   .ProducesPhonemesLabelFormat,
               Settings.Automation.Samples.PhonemeEstimationText),
           !Phonemes.IsEmpty());

  TestEqual(Settings.Automation.PhonemeAssertions.CorrectPhonemeCountLabel,
            Phonemes.Num(),
            Settings.Automation.Samples.PhonemeEstimationText.Len());

  TestEqual(Settings.Automation.PhonemeAssertions.FirstPhonemeStartLabel,
            Phonemes[int32{}].StartTime,
            Settings.Playback.InitialPlaybackTime);

  const auto CheckDurationsRecursive =
      [this, &Phonemes, &Settings](int32 Idx, const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : [&]() {
                   TestTrue(
                       Label(Settings.Automation.PhonemeAssertions
                                 .PositiveDurationLabelFormat,
                             Idx),
                       Phonemes[Idx].Duration > float{});
                   int32 Next = Idx;
                   ++Next;
                   Self(Next, Self);
                 }();
  };
  CheckDurationsRecursive(int32{}, CheckDurationsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeWithSpaces,
    SpeechAutomationSettings().Cases.PhonemeEstimationWithSpacesTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FSpeechPhonemeWithSpaces::RunTest(const FString &Parameters) */
bool FSpeechPhonemeWithSpaces::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(
          Settings.Automation.Samples.SilenceText, Settings);

  TestTrue(Label(
               Settings.Automation.PhonemeAssertions
                   .ProducesPhonemesLabelFormat,
               Settings.Automation.Samples.SilenceText),
           !Phonemes.IsEmpty());

  bool bHasSilence = false;
  const auto FindSilRecursive =
      [&Phonemes, &bHasSilence, &Settings](int32 Idx,
                                           const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : [&]() {
                   Phonemes[Idx].Phoneme ==
                           Settings.Phoneme.SilencePhoneme
                       ? (void)(bHasSilence = true)
                       : void();
                   int32 Next = Idx;
                   ++Next;
                   Self(Next, Self);
                 }();
  };
  FindSilRecursive(int32{}, FindSilRecursive);

  TestTrue(Settings.Automation.PhonemeAssertions.SilencePhonemeLabel,
           bHasSilence);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeMapCompleteness,
    SpeechAutomationSettings().Cases.VisemeMapCompletenessTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FSpeechVisemeMapCompleteness::RunTest(const FString &Parameters) */
bool FSpeechVisemeMapCompleteness::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);

  TestEqual(Settings.Automation.VisemeAssertions.VisemeMapCountLabel,
            Map.Num(), Settings.LipSync.VisemeMappings.Num());

  const auto CheckVowelsRecursive = [this, &Map, &Settings](
                                        int32 Idx,
                                        const auto &Self) -> void {
    return Idx >= Settings.Phoneme.VowelPhonemes.Num()
               ? void()
               : [&]() {
                   const ForbocAI::Game::Data::FVowelPhonemeSettings
                       Vowel = Settings.Phoneme.VowelPhonemes[Idx];
                   TestTrue(Label(
                                Settings.Automation.VisemeAssertions
                                    .VowelMappedLabelFormat,
                                Vowel.Character),
                            Map.Contains(Vowel.Phoneme));
                   int32 Next = Idx;
                   ++Next;
                   Self(Next, Self);
                 }();
  };
  CheckVowelsRecursive(int32{}, CheckVowelsRecursive);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechActiveVisemeAtTime,
    SpeechAutomationSettings().Cases.ActiveVisemeAtTimeTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) */
bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);
  const ForbocAI::Game::Data::FMappingSettings &Primary =
      RequiredNonSilenceMapping(Settings);
  const ForbocAI::Game::Data::FVowelPhonemeSettings &SecondaryVowel =
      RequiredFirst(Settings.Phoneme.VowelPhonemes);

  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = Settings.Playback.InitialPlaybackTime;
  Phonemes.Add({Primary.Phoneme, CurrentTime,
                SpeechOps::EstimatePhonemeDuration(Primary.Phoneme,
                                                    Settings)});
  CurrentTime += Phonemes.Last().Duration;
  Phonemes.Add(
      {Settings.Phoneme.SilencePhoneme, CurrentTime,
       SpeechOps::EstimatePhonemeDuration(
           Settings.Phoneme.SilencePhoneme, Settings)});
  CurrentTime += Phonemes.Last().Duration;
  Phonemes.Add({SecondaryVowel.Phoneme, CurrentTime,
                SpeechOps::EstimatePhonemeDuration(SecondaryVowel.Phoneme,
                                                    Settings)});

  int32 PrimaryIndex = int32{};
  int32 SilenceIndex = PrimaryIndex;
  ++SilenceIndex;
  int32 SecondaryIndex = SilenceIndex;
  ++SecondaryIndex;

  const FVisemeMapping ExpectedAa =
      SpeechOps::RequiredVisemeForPhoneme(Primary.Phoneme, Map);
  const FVisemeMapping ExpectedSilence =
      SpeechOps::RequiredVisemeForPhoneme(
          Settings.Phoneme.SilencePhoneme, Map);
  const FVisemeMapping ExpectedEh =
      SpeechOps::RequiredVisemeForPhoneme(SecondaryVowel.Phoneme, Map);

  const FVisemeMapping V1 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[PrimaryIndex],
                      Settings.Automation.Samples.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.VisemeAssertions
                      .ActiveVisemeLabelFormat,
                  Primary.Phoneme),
            V1.MorphTargetName, ExpectedAa.MorphTargetName);
  TestTrue(Label(Settings.Automation.VisemeAssertions
                     .ActiveVisemeWeightLabelFormat,
                 Primary.Phoneme),
           V1.BlendWeight > float{});

  const FVisemeMapping V2 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[SilenceIndex],
                      Settings.Automation.Samples.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.VisemeAssertions
                      .ActiveVisemeLabelFormat,
                  Settings.Phoneme.SilencePhoneme),
            V2.MorphTargetName,
            ExpectedSilence.MorphTargetName);

  const FVisemeMapping V3 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[SecondaryIndex],
                      Settings.Automation.Samples.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.VisemeAssertions
                      .ActiveVisemeLabelFormat,
                  SecondaryVowel.Phoneme),
            V3.MorphTargetName,
            ExpectedEh.MorphTargetName);

  const FPhonemeEvent Last = Phonemes.Last();
  const FVisemeMapping V4 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {Last.StartTime + Last.Duration +
               Last.Duration *
                   Settings.Automation.Samples.PastEndSampleRatio,
           Map, Rest});
  TestEqual(Settings.Automation.VisemeAssertions.PastEndSilenceLabel,
            V4.MorphTargetName,
            Rest.MorphTargetName);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeLookup,
    SpeechAutomationSettings().Cases.VisemeLookupUnknownTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

/** User Story: As a tests consumer, I need to invoke run test through a stable signature so the tests workflow remains explicit and composable. @fn bool FSpeechVisemeLookup::RunTest(const FString &Parameters) */
bool FSpeechVisemeLookup::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const ForbocAI::Game::Data::FMappingSettings &KnownMapping =
      RequiredNonSilenceMapping(Settings);

  const func::Maybe<FVisemeMapping> Known =
      SpeechOps::LookupViseme(KnownMapping.Phoneme, Map);
  const FVisemeMapping Expected =
      SpeechOps::RequiredVisemeForPhoneme(KnownMapping.Phoneme, Map);
  check(Known.hasValue);
  TestEqual(Label(Settings.Automation.LookupAssertions
                      .KnownPhonemeLabelFormat,
                  KnownMapping.Phoneme),
            Known.value.MorphTargetName, Expected.MorphTargetName);

  const func::Maybe<FVisemeMapping> Unknown =
      SpeechOps::LookupViseme(
          Settings.Automation.Samples.UnknownPhoneme, Map);
  TestFalse(Label(Settings.Automation.LookupAssertions
                      .UnknownPhonemeLabelFormat,
                  Settings.Automation.Samples.UnknownPhoneme),
            Unknown.hasValue);

  return true;
}
