#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(FDialogueSettings, ReplyPayloadIdFormat);

JSON_SETTINGS_REGISTRY(FMappingSettings, Phoneme,
                       MorphTargetName, BlendWeight);

JSON_SETTINGS_REGISTRY(FVowelPhonemeSettings, Character, Phoneme);

JSON_SETTINGS_REGISTRY(FDurationRuleSettings, Kind, Phoneme,
                       Multiplier);

#define FORBOCAI_SPEECH_AUTOMATION_FIELD(Group, GroupType, Field)           \
  NestedSettingField(                                                       \
      JSON_SETTING_ATOM(Field),                                             \
      NestedFieldMembers(&FAutomationSettings::Group, &GroupType::Field))

template <> struct TJsonSettingsRegistry<FAutomationSettings> {
  /** User Story: As a speech automation registry consumer, I need to enumerate authored fields through a stable signature so test evidence remains composable. @fn static const TArray<TField<FAutomationSettings>> &Fields() */
  static const TArray<TField<FAutomationSettings>> &Fields() {
    static const TArray<TField<FAutomationSettings>> RegisteredFields = {
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Cases, FCaseSettings, PhonemeEstimationTest),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Cases, FCaseSettings, PhonemeEstimationWithSpacesTest),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Cases, FCaseSettings, VisemeMapCompletenessTest),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Cases, FCaseSettings, ActiveVisemeAtTimeTest),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Cases, FCaseSettings, VisemeLookupUnknownTest),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Samples, FSampleSettings, PhonemeEstimationText),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Samples, FSampleSettings, SilenceText),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Samples, FSampleSettings, UnknownPhoneme),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Samples, FSampleSettings, ActiveVisemeSampleRatio),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            Samples, FSampleSettings, PastEndSampleRatio),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            PhonemeAssertions, FPhonemeAssertionSettings,
            ProducesPhonemesLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            PhonemeAssertions, FPhonemeAssertionSettings,
            CorrectPhonemeCountLabel),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            PhonemeAssertions, FPhonemeAssertionSettings,
            FirstPhonemeStartLabel),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            PhonemeAssertions, FPhonemeAssertionSettings,
            PositiveDurationLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            PhonemeAssertions, FPhonemeAssertionSettings,
            SilencePhonemeLabel),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            VisemeAssertions, FVisemeAssertionSettings, VisemeMapCountLabel),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            VisemeAssertions, FVisemeAssertionSettings,
            VowelMappedLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            VisemeAssertions, FVisemeAssertionSettings,
            ActiveVisemeLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            VisemeAssertions, FVisemeAssertionSettings,
            ActiveVisemeWeightLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            VisemeAssertions, FVisemeAssertionSettings, PastEndSilenceLabel),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            LookupAssertions, FLookupAssertionSettings,
            KnownPhonemeLabelFormat),
        FORBOCAI_SPEECH_AUTOMATION_FIELD(
            LookupAssertions, FLookupAssertionSettings,
            UnknownPhonemeLabelFormat)};
    return RegisteredFields;
  }
};

#undef FORBOCAI_SPEECH_AUTOMATION_FIELD

#define FORBOCAI_SPEECH_SETTING_FIELD(Group, GroupType, Field)              \
  NestedSettingField(JSON_SETTING_ATOM(Field),                              \
                     NestedFieldMembers(&FSpeechSettings::Group,            \
                                        &GroupType::Field))

template <> struct TJsonSettingsRegistry<FSpeechSettings> {
  /** User Story: As a speech settings registry consumer, I need to enumerate authored fields through a stable signature so runtime concerns remain composable. @fn static const TArray<TField<FSpeechSettings>> &Fields() */
  static const TArray<TField<FSpeechSettings>> &Fields() {
    static const TArray<TField<FSpeechSettings>> RegisteredFields = {
        FORBOCAI_SPEECH_SETTING_FIELD(Voice, FVoiceSettings, SpeechRate),
        FORBOCAI_SPEECH_SETTING_FIELD(Voice, FVoiceSettings, Volume),
        FORBOCAI_SPEECH_SETTING_FIELD(
            LipSync, FLipSyncSettings, RestViseme),
        FORBOCAI_SPEECH_SETTING_FIELD(
            LipSync, FLipSyncSettings, RestWeight),
        FORBOCAI_SPEECH_SETTING_FIELD(
            LipSync, FLipSyncSettings, bEnableLipSync),
        FORBOCAI_SPEECH_SETTING_FIELD(
            LipSync, FLipSyncSettings, VisemeChangeTolerance),
        FORBOCAI_SPEECH_SETTING_FIELD(
            LipSync, FLipSyncSettings, ResetMorphTargets),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Playback, FPlaybackSettings, bCanEverTick),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Playback, FPlaybackSettings, bStartTickEnabled),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Playback, FPlaybackSettings, InitialPlaybackTime),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Playback, FPlaybackSettings, bInitialSpeechActive),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Phoneme, FPhonemeSettings, EstimatedBasePhonemeSeconds),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Phoneme, FPhonemeSettings, SilenceCharacters),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Phoneme, FPhonemeSettings, SilencePhoneme),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsRequest, FRequestSettings, TtsEndpoint),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsRequest, FRequestSettings, TtsVerb),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsRequest, FRequestSettings, TtsContentTypeHeader),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsRequest, FRequestSettings, TtsContentType),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsRequest, FRequestSettings, TtsRequestFormat),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsResponse, FResponseSettings, TtsSuccessResponseCode),
        FORBOCAI_SPEECH_SETTING_FIELD(
            TtsResponse, FResponseSettings, MinimumAudioBytes),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Diagnostics, FDiagnosticSettings, SpeechStartLogFormat),
        FORBOCAI_SPEECH_SETTING_FIELD(
            Diagnostics, FDiagnosticSettings, SpeechAudioReceivedLogFormat),
        NestedObjectArraySettingField(
            JSON_SETTING_ATOM(VowelPhonemes),
            NestedFieldMembers(&FSpeechSettings::Phoneme,
                               &FPhonemeSettings::VowelPhonemes),
            ReadSettingsWith<FVowelPhonemeSettings>(
                JSON_SETTINGS_ATOMS(Character, Phoneme))),
        NestedObjectArraySettingField(
            JSON_SETTING_ATOM(VisemeMappings),
            NestedFieldMembers(&FSpeechSettings::LipSync,
                               &FLipSyncSettings::VisemeMappings),
            ReadSettingsWith<FMappingSettings>(JSON_SETTINGS_ATOMS(
                Phoneme, MorphTargetName, BlendWeight))),
        NestedObjectArraySettingField(
            JSON_SETTING_ATOM(DurationRules),
            NestedFieldMembers(&FSpeechSettings::Phoneme,
                               &FPhonemeSettings::DurationRules),
            ReadSettingsWith<FDurationRuleSettings>(
                JSON_SETTINGS_ATOMS(Kind, Phoneme, Multiplier))),
        JSON_OBJECT_SETTING_FIELDS(
            FSpeechSettings,
            ReadSettingsWith<FAutomationSettings>(JSON_SETTINGS_ATOMS(
                PhonemeEstimationTest, PhonemeEstimationWithSpacesTest,
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

#undef FORBOCAI_SPEECH_SETTING_FIELD

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
