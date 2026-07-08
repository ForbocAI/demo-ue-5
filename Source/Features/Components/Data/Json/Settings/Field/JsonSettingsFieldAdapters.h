#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

template <typename Settings> struct TField {
  FString FieldName;
  TFunction<Settings(const FFieldRequest &, const Settings &)> Apply;

  TField()
      : FieldName(),
        Apply([](const FFieldRequest &, const Settings &Current) {
          return Current;
        }) {}

  template <typename Value>
  TField(const char *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

  template <typename Value>
  TField(const TCHAR *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

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

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
