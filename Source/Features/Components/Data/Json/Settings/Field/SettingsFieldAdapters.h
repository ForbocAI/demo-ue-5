#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

#define JSON_SETTING_FIELD(Type, Field) {#Field, &Type::Field}
#define JSON_SETTING_FIELD_LIST_INDIRECT() JSON_SETTING_FIELD_LIST
#define JSON_SETTING_FIELD_LIST(Type, Field, ...)                            \
  JSON_SETTING_FIELD(Type, Field)                                            \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_SETTING_FIELD_LIST_INDIRECT)()(            \
                    Type, __VA_ARGS__))
#define JSON_SETTING_FIELDS(Type, ...)                                       \
  JSON_EXPAND(JSON_SETTING_FIELD_LIST(Type, __VA_ARGS__))
#define JSON_SETTINGS_FIELDS(Type, ...)                                      \
  {JSON_SETTING_FIELDS(Type, __VA_ARGS__)}
#define JSON_OBJECT_SETTING_FIELD(Type, Reader, Field)                       \
  {#Field, &Type::Field, Reader}
#define JSON_OBJECT_SETTING_FIELD_LIST_INDIRECT()                            \
  JSON_OBJECT_SETTING_FIELD_LIST
#define JSON_OBJECT_SETTING_FIELD_LIST(Type, Reader, Field, ...)             \
  JSON_OBJECT_SETTING_FIELD(Type, Reader, Field)                             \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_OBJECT_SETTING_FIELD_LIST_INDIRECT)()(      \
                    Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_SETTING_FIELDS(Type, Reader, ...)                        \
  JSON_EXPAND(JSON_OBJECT_SETTING_FIELD_LIST(Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_SETTINGS_FIELDS(Type, Reader, ...)                       \
  {JSON_OBJECT_SETTING_FIELDS(Type, Reader, __VA_ARGS__)}
#define JSON_OBJECT_ARRAY_SETTING_FIELD(Type, Reader, Field)                 \
  {#Field, &Type::Field, Reader}
#define JSON_OBJECT_ARRAY_SETTING_FIELD_LIST_INDIRECT()                      \
  JSON_OBJECT_ARRAY_SETTING_FIELD_LIST
#define JSON_OBJECT_ARRAY_SETTING_FIELD_LIST(Type, Reader, Field, ...)       \
  JSON_OBJECT_ARRAY_SETTING_FIELD(Type, Reader, Field)                       \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST_INDIRECT)()(\
                    Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, ...)                  \
  JSON_EXPAND(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST(Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTINGS_FIELDS(Type, Reader, ...)                 \
  {JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, __VA_ARGS__)}

template <typename Settings> struct TField {
  FString FieldName;
  TFunction<Settings(const FFieldRequest &, const Settings &)> Apply;

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn TField() */
  TField()
      : FieldName(),
        Apply([](const FFieldRequest &, const Settings &Current) {
          return Current;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn TField( const char *FieldAtom, TFunction<Settings(const FFieldRequest &, const Settings &)> ApplyFn) */
  TField(
      const char *FieldAtom,
      TFunction<Settings(const FFieldRequest &, const Settings &)> ApplyFn)
      : FieldName(SettingsFieldName(FieldAtom)), Apply(ApplyFn) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField(const char *FieldAtom, Value Settings::*Member) */
  template <typename Value>
  TField(const char *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField(const TCHAR *FieldAtom, Value Settings::*Member) */
  template <typename Value>
  TField(const TCHAR *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField( const char *FieldAtom, Value Settings::*Member, Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &)) */
  template <typename Value>
  TField(
      const char *FieldAtom, Value Settings::*Member,
      Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField( const char *FieldAtom, Value Settings::*Member, TJsonObjectSettingsReader<Value> ReadObjectFn) */
  template <typename Value>
  TField(
      const char *FieldAtom, Value Settings::*Member,
      TJsonObjectSettingsReader<Value> ReadObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField( const TCHAR *FieldAtom, Value Settings::*Member, Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &)) */
  template <typename Value>
  TField(
      const TCHAR *FieldAtom, Value Settings::*Member,
      Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Value> TField( const TCHAR *FieldAtom, Value Settings::*Member, TJsonObjectSettingsReader<Value> ReadObjectFn) */
  template <typename Value>
  TField(
      const TCHAR *FieldAtom, Value Settings::*Member,
      TJsonObjectSettingsReader<Value> ReadObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Output> TField( const char *FieldAtom, TArray<Output> Settings::*Member, Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &)) */
  template <typename Output>
  TField(
      const char *FieldAtom, TArray<Output> Settings::*Member,
      Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Output> TField( const char *FieldAtom, TArray<Output> Settings::*Member, TJsonObjectSettingsReader<Output> MapObjectFn) */
  template <typename Output>
  TField(
      const char *FieldAtom, TArray<Output> Settings::*Member,
      TJsonObjectSettingsReader<Output> MapObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Output> TField( const TCHAR *FieldAtom, TArray<Output> Settings::*Member, Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &)) */
  template <typename Output>
  TField(
      const TCHAR *FieldAtom, TArray<Output> Settings::*Member,
      Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  /** User Story: As a json settings field consumer, I need to invoke tfield through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Output> TField( const TCHAR *FieldAtom, TArray<Output> Settings::*Member, TJsonObjectSettingsReader<Output> MapObjectFn) */
  template <typename Output>
  TField(
      const TCHAR *FieldAtom, TArray<Output> Settings::*Member,
      TJsonObjectSettingsReader<Output> MapObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}
};

template <typename Settings, typename Group, typename Value>
struct TNestedFieldMembers {
  Group Settings::*GroupMember;
  Value Group::*ValueMember;
};

/** User Story: As a json settings field consumer, I need to invoke nested field members through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Settings, typename Group, typename Value> TNestedFieldMembers<Settings, Group, Value> NestedFieldMembers(Group Settings::*GroupMember, Value Group::*ValueMember) */
template <typename Settings, typename Group, typename Value>
TNestedFieldMembers<Settings, Group, Value>
NestedFieldMembers(Group Settings::*GroupMember, Value Group::*ValueMember) {
  return {GroupMember, ValueMember};
}

/** User Story: As a json settings field consumer, I need to invoke nested setting field through a stable signature so the json settings field workflow remains explicit and composable. @fn template <typename Settings, typename Group, typename Value> TField<Settings> NestedSettingField( const char *FieldAtom, const TNestedFieldMembers<Settings, Group, Value> &Members) */
template <typename Settings, typename Group, typename Value>
TField<Settings>
NestedSettingField(
    const char *FieldAtom,
    const TNestedFieldMembers<Settings, Group, Value> &Members) {
  return TField<Settings>(
      FieldAtom,
      [Members](const FFieldRequest &Request, const Settings &Current) {
        Settings Next = Current;
        ((Next.*(Members.GroupMember)).*(Members.ValueMember)) =
            ReadFieldValue<Value>(Request);
        return Next;
      });
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
