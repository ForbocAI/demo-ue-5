#pragma once

#include "Features/Components/Data/Settings/Registry/Color/ColorAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FMessage, Role, Text,
                       ExpectedText, ExpectedGreen);
JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FLine, Index,
                       ExpectedText);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FHistory> {
  /** User Story: As a conversation automation settings consumer, I need to access history fields through a stable signature so dialogue scenarios remain data-driven. @fn static const TArray<TField<Automation::Conversation::UI::FHistory>> &Fields() */
  static const TArray<TField<Automation::Conversation::UI::FHistory>>
      &Fields() {
    static const TArray<TField<Automation::Conversation::UI::FHistory>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FHistory, Lines,
                                ExpectedCount),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FHistory,
                ReadSettingsWith<Automation::Conversation::UI::FLine>(
                    JSON_SETTINGS_ATOMS(Index, ExpectedText)),
                Tagged, Untagged)};
    return RegisteredFields;
  }
};

JSON_SETTINGS_REGISTRY(Automation::Conversation::UI::FSubmitted, Input,
                       ExpectedText);

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FDialogue> {
  /** User Story: As a conversation automation settings consumer, I need to access dialogue fields through a stable signature so dialogue scenarios remain data-driven. @fn static const TArray<TField<Automation::Conversation::UI::FDialogue>> &Fields() */
  static const TArray<TField<Automation::Conversation::UI::FDialogue>>
      &Fields() {
    static const TArray<TField<Automation::Conversation::UI::FDialogue>>
        RegisteredFields = {
            NestedSettingField(
                JSON_SETTING_ATOM(SpeakerName),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Speaker,
                    &Automation::Conversation::UI::FSpeaker::SpeakerName)),
            NestedSettingField(
                JSON_SETTING_ATOM(SpeakerRole),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Speaker,
                    &Automation::Conversation::UI::FSpeaker::SpeakerRole)),
            NestedSettingField(
                JSON_SETTING_ATOM(PlayerLine),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Exchange,
                    &Automation::Conversation::UI::FExchange::PlayerLine)),
            NestedSettingField(
                JSON_SETTING_ATOM(NpcReply),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Exchange,
                    &Automation::Conversation::UI::FExchange::NpcReply)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedTitle),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::ExpectedTitle)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedPlayerLine),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::
                        ExpectedPlayerLine)),
            NestedSettingField(
                JSON_SETTING_ATOM(ExpectedNpcReply),
                NestedFieldMembers(
                    &Automation::Conversation::UI::FDialogue::Expected,
                    &Automation::Conversation::UI::FExpected::
                        ExpectedNpcReply))};
    return RegisteredFields;
  }
};

template <>
struct TJsonSettingsRegistry<Automation::Conversation::UI::FSettings> {
  /** User Story: As a conversation automation settings consumer, I need to access conversation scenario fields through a stable signature so UI scenarios remain data-driven. @fn static const TArray<TField<Automation::Conversation::UI::FSettings>> &Fields() */
  static const TArray<TField<Automation::Conversation::UI::FSettings>>
      &Fields() {
    static const TArray<TField<Automation::Conversation::UI::FSettings>>
        RegisteredFields = {
            JSON_SETTING_FIELDS(Automation::Conversation::UI::FSettings, Test,
                                Assertions),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FMessage>(
                    JSON_SETTINGS_ATOMS(Role, Text, ExpectedText,
                                        ExpectedGreen)),
                PlayerMessage),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FHistory>(
                    JSON_SETTINGS_ATOMS(Lines, ExpectedCount, Tagged,
                                        Untagged)),
                History),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FSubmitted>(
                    JSON_SETTINGS_ATOMS(Input, ExpectedText)),
                Submitted),
            JSON_OBJECT_SETTING_FIELDS(
                Automation::Conversation::UI::FSettings,
                ReadSettingsWith<Automation::Conversation::UI::FDialogue>(
                    JSON_SETTINGS_ATOMS(
                        SpeakerName, SpeakerRole, PlayerLine, NpcReply,
                        ExpectedTitle, ExpectedPlayerLine, ExpectedNpcReply)),
                Dialogue)};
    return RegisteredFields;
  }
};

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
