#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FDialogueSettings, ReplyPayloadIdFormat);

JSON_SETTINGS_REGISTRY(FVisemeMappingSettings, Phoneme,
                       MorphTargetName, BlendWeight);

JSON_SETTINGS_REGISTRY(FVowelPhonemeSettings, Character, Phoneme);

JSON_SETTINGS_REGISTRY(FPhonemeDurationRuleSettings, Kind, Phoneme,
                       Multiplier);

JSON_SETTINGS_REGISTRY(FAutomationSettings, PhonemeEstimationTest,
                       PhonemeEstimationWithSpacesTest,
                       VisemeMapCompletenessTest, ActiveVisemeAtTimeTest,
                       VisemeLookupUnknownTest, PhonemeEstimationText,
                       SilenceText, UnknownPhoneme, ActiveVisemeSampleRatio,
                       PastEndSampleRatio, ProducesPhonemesLabelFormat,
                       CorrectPhonemeCountLabel, FirstPhonemeStartLabel,
                       PositiveDurationLabelFormat, SilencePhonemeLabel,
                       VisemeMapCountLabel, VowelMappedLabelFormat,
                       ActiveVisemeLabelFormat, ActiveVisemeWeightLabelFormat,
                       PastEndSilenceLabel, KnownPhonemeLabelFormat,
                       UnknownPhonemeLabelFormat);

template <> struct TJsonSettingsRegistry<FSpeechSettings> {
  /** User Story: As a settings registry speech consumer, I need to invoke fields through a stable signature so the settings registry speech workflow remains explicit and composable. @fn static const TArray<TField<FSpeechSettings>> &Fields() */
  static const TArray<TField<FSpeechSettings>> &Fields() {
    static const TArray<TField<FSpeechSettings>> RegisteredFields = {
        JSON_SETTING_FIELDS(FSpeechSettings, RestViseme, RestWeight,
                            SpeechRate, Volume, bEnableLipSync,
                            bCanEverTick, bStartTickEnabled,
                            InitialPlaybackTime, bInitialSpeechActive,
                            EstimatedBasePhonemeSeconds,
                            SilenceCharacters, SilencePhoneme,
                            VisemeChangeTolerance, TtsEndpoint, TtsVerb,
                            TtsContentTypeHeader, TtsContentType,
                            TtsRequestFormat, TtsSuccessResponseCode,
                            MinimumAudioBytes, SpeechStartLogFormat,
                            SpeechAudioReceivedLogFormat,
                            ResetMorphTargets),
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FSpeechSettings,
            ReadSettingsWith<FVowelPhonemeSettings>(
                JSON_SETTINGS_ATOMS(Character, Phoneme)),
            VowelPhonemes),
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FSpeechSettings,
            ReadSettingsWith<FVisemeMappingSettings>(
                JSON_SETTINGS_ATOMS(Phoneme, MorphTargetName,
                                    BlendWeight)),
            VisemeMappings),
        JSON_OBJECT_ARRAY_SETTING_FIELDS(
            FSpeechSettings,
            ReadSettingsWith<FPhonemeDurationRuleSettings>(
                JSON_SETTINGS_ATOMS(Kind, Phoneme, Multiplier)),
            DurationRules),
        JSON_OBJECT_SETTING_FIELDS(
            FSpeechSettings,
            ReadSettingsWith<FAutomationSettings>(JSON_SETTINGS_ATOMS(
                PhonemeEstimationTest,
                PhonemeEstimationWithSpacesTest,
                VisemeMapCompletenessTest, ActiveVisemeAtTimeTest,
                VisemeLookupUnknownTest, PhonemeEstimationText,
                SilenceText, UnknownPhoneme, ActiveVisemeSampleRatio,
                PastEndSampleRatio, ProducesPhonemesLabelFormat,
                CorrectPhonemeCountLabel, FirstPhonemeStartLabel,
                PositiveDurationLabelFormat, SilencePhonemeLabel,
                VisemeMapCountLabel, VowelMappedLabelFormat,
                ActiveVisemeLabelFormat, ActiveVisemeWeightLabelFormat,
                PastEndSilenceLabel, KnownPhonemeLabelFormat,
                UnknownPhonemeLabelFormat)),
            Automation)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
