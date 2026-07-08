#include "Tests/Speech/SpeechSpecSupport.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeEstimation,
    SpeechAutomationSettings().PhonemeEstimationTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechPhonemeEstimation::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(
          Settings.Automation.PhonemeEstimationText, Settings);

  TestTrue(Label(Settings.Automation.ProducesPhonemesLabelFormat,
                 Settings.Automation.PhonemeEstimationText),
           !Phonemes.IsEmpty());

  TestEqual(Settings.Automation.CorrectPhonemeCountLabel, Phonemes.Num(),
            Settings.Automation.PhonemeEstimationText.Len());

  TestEqual(Settings.Automation.FirstPhonemeStartLabel,
            Phonemes[int32{}].StartTime, Settings.InitialPlaybackTime);

  const auto CheckDurationsRecursive =
      [this, &Phonemes, &Settings](int32 Idx, const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : [&]() {
                   TestTrue(
                       Label(Settings.Automation.PositiveDurationLabelFormat,
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
    SpeechAutomationSettings().PhonemeEstimationWithSpacesTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechPhonemeWithSpaces::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TArray<FPhonemeEvent> Phonemes =
      SpeechOps::EstimatePhonemesFromText(Settings.Automation.SilenceText,
                                          Settings);

  TestTrue(Label(Settings.Automation.ProducesPhonemesLabelFormat,
                 Settings.Automation.SilenceText),
           !Phonemes.IsEmpty());

  bool bHasSilence = false;
  const auto FindSilRecursive =
      [&Phonemes, &bHasSilence, &Settings](int32 Idx,
                                           const auto &Self) -> void {
    return Idx >= Phonemes.Num()
               ? void()
               : [&]() {
                   Phonemes[Idx].Phoneme == Settings.SilencePhoneme
                       ? (void)(bHasSilence = true)
                       : void();
                   int32 Next = Idx;
                   ++Next;
                   Self(Next, Self);
                 }();
  };
  FindSilRecursive(int32{}, FindSilRecursive);

  TestTrue(Settings.Automation.SilencePhonemeLabel, bHasSilence);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeMapCompleteness,
    SpeechAutomationSettings().VisemeMapCompletenessTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeMapCompleteness::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);

  TestEqual(Settings.Automation.VisemeMapCountLabel, Map.Num(),
            Settings.VisemeMappings.Num());

  const auto CheckVowelsRecursive = [this, &Map, &Settings](
                                        int32 Idx,
                                        const auto &Self) -> void {
    return Idx >= Settings.VowelPhonemes.Num()
               ? void()
               : [&]() {
                   const ForbocAI::Game::Data::FVowelPhonemeSettings
                       Vowel = Settings.VowelPhonemes[Idx];
                   TestTrue(Label(Settings.Automation.VowelMappedLabelFormat,
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
    SpeechAutomationSettings().ActiveVisemeAtTimeTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);
  const ForbocAI::Game::Data::FVisemeMappingSettings &Primary =
      RequiredNonSilenceMapping(Settings);
  const ForbocAI::Game::Data::FVowelPhonemeSettings &SecondaryVowel =
      RequiredFirst(Settings.VowelPhonemes);

  TArray<FPhonemeEvent> Phonemes;
  float CurrentTime = Settings.InitialPlaybackTime;
  Phonemes.Add({Primary.Phoneme, CurrentTime,
                SpeechOps::EstimatePhonemeDuration(Primary.Phoneme,
                                                    Settings)});
  CurrentTime += Phonemes.Last().Duration;
  Phonemes.Add({Settings.SilencePhoneme, CurrentTime,
                SpeechOps::EstimatePhonemeDuration(Settings.SilencePhoneme,
                                                    Settings)});
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
      SpeechOps::RequiredVisemeForPhoneme(Settings.SilencePhoneme, Map);
  const FVisemeMapping ExpectedEh =
      SpeechOps::RequiredVisemeForPhoneme(SecondaryVowel.Phoneme, Map);

  const FVisemeMapping V1 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[PrimaryIndex],
                      Settings.Automation.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.ActiveVisemeLabelFormat,
                  Primary.Phoneme),
            V1.MorphTargetName, ExpectedAa.MorphTargetName);
  TestTrue(Label(Settings.Automation.ActiveVisemeWeightLabelFormat,
                 Primary.Phoneme),
           V1.BlendWeight > float{});

  const FVisemeMapping V2 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[SilenceIndex],
                      Settings.Automation.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.ActiveVisemeLabelFormat,
                  Settings.SilencePhoneme),
            V2.MorphTargetName,
            ExpectedSilence.MorphTargetName);

  const FVisemeMapping V3 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {SampleTime(Phonemes[SecondaryIndex],
                      Settings.Automation.ActiveVisemeSampleRatio),
           Map, Rest});
  TestEqual(Label(Settings.Automation.ActiveVisemeLabelFormat,
                  SecondaryVowel.Phoneme),
            V3.MorphTargetName,
            ExpectedEh.MorphTargetName);

  const FPhonemeEvent Last = Phonemes.Last();
  const FVisemeMapping V4 =
      SpeechOps::ActiveVisemeAtTime(
          Phonemes,
          {Last.StartTime + Last.Duration +
               Last.Duration * Settings.Automation.PastEndSampleRatio,
           Map, Rest});
  TestEqual(Settings.Automation.PastEndSilenceLabel, V4.MorphTargetName,
            Rest.MorphTargetName);

  return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechVisemeLookup,
    SpeechAutomationSettings().VisemeLookupUnknownTest,
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeLookup::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const ForbocAI::Game::Data::FVisemeMappingSettings &KnownMapping =
      RequiredNonSilenceMapping(Settings);

  const func::Maybe<FVisemeMapping> Known =
      SpeechOps::LookupViseme(KnownMapping.Phoneme, Map);
  const FVisemeMapping Expected =
      SpeechOps::RequiredVisemeForPhoneme(KnownMapping.Phoneme, Map);
  check(Known.hasValue);
  TestEqual(Label(Settings.Automation.KnownPhonemeLabelFormat,
                  KnownMapping.Phoneme),
            Known.value.MorphTargetName, Expected.MorphTargetName);

  const func::Maybe<FVisemeMapping> Unknown =
      SpeechOps::LookupViseme(Settings.Automation.UnknownPhoneme, Map);
  TestFalse(Label(Settings.Automation.UnknownPhonemeLabelFormat,
                  Settings.Automation.UnknownPhoneme),
            Unknown.hasValue);

  return true;
}
