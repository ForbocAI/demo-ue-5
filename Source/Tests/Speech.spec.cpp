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

TMap<FString, FVisemeMapping> LoadVisemeMap(
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  return SpeechOps::VisemeMapFromSettings(Settings);
}

template <typename Item> const Item &RequiredFirst(const TArray<Item> &Items) {
  check(!Items.IsEmpty());
  return Items[int32{}];
}

const ForbocAI::Game::Data::FSpeechVisemeMappingSettings &
RequiredNonSilenceMapping(
    const ForbocAI::Game::Data::FSpeechSettings &Settings) {
  const func::Maybe<ForbocAI::Game::Data::FSpeechVisemeMappingSettings>
      Mapping = func::find_array<
          ForbocAI::Game::Data::FSpeechVisemeMappingSettings>(
          Settings.VisemeMappings, [&Settings](
                                       const ForbocAI::Game::Data::
                                           FSpeechVisemeMappingSettings &Item) {
            return Item.Phoneme != Settings.SilencePhoneme;
          });
  check(Mapping.hasValue);
  const ForbocAI::Game::Data::FSpeechVisemeMappingSettings *Found =
      Settings.VisemeMappings.FindByPredicate(
          [&Mapping](
              const ForbocAI::Game::Data::FSpeechVisemeMappingSettings &Item) {
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FSpeechPhonemeEstimation,
    "ForbocAI.Speech.PhonemeEstimation",
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
    "ForbocAI.Speech.PhonemeEstimationWithSpaces",
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
    "ForbocAI.Speech.VisemeMapCompleteness",
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
                   const ForbocAI::Game::Data::FSpeechVowelPhonemeSettings
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
    "ForbocAI.Speech.ActiveVisemeAtTime",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechActiveVisemeAtTime::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const FVisemeMapping Rest = SpeechOps::RestViseme(Settings);
  const ForbocAI::Game::Data::FSpeechVisemeMappingSettings &Primary =
      RequiredNonSilenceMapping(Settings);
  const ForbocAI::Game::Data::FSpeechVowelPhonemeSettings &SecondaryVowel =
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
    "ForbocAI.Speech.VisemeLookupUnknown",
    EAutomationTestFlags::EditorContext |
        EAutomationTestFlags::EngineFilter)

bool FSpeechVisemeLookup::RunTest(const FString &Parameters) {
  const ForbocAI::Game::Data::FSpeechSettings Settings =
      LoadSpeechSettings();
  const TMap<FString, FVisemeMapping> Map = LoadVisemeMap(Settings);
  const ForbocAI::Game::Data::FSpeechVisemeMappingSettings &KnownMapping =
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
