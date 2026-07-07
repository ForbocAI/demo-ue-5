#pragma once

#include "Features/Components/Data/Json/JsonReadAdapters.h"

#include <initializer_list>

// Json settings adapters: RTK-style adapter plumbing that folds JSON objects
// into typed settings structs. Owns the TJsonSettingsField<T> fold machinery,
// declaration macros, settings registry, and composition helpers.
// Imports downward from JsonReadAdapters (core readers) and JsonReducers
// (field name conversion).

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

// --- Settings field binding (adapter fold machinery) ---------------------
// TJsonSettingsField<Settings> is an RTK entity adapter pattern: it owns the
// per-field read-and-apply fold step. Feature domains declare field lists as
// grouped atoms; this adapter machinery owns traversal and typed dispatch.

template <typename Settings> struct TJsonSettingsField {
  FString FieldName;
  TFunction<Settings(const FJsonFieldRequest &, const Settings &)> Apply;

  TJsonSettingsField()
      : FieldName(),
        Apply([](const FJsonFieldRequest &, const Settings &Current) {
          return Current;
        }) {}

  template <typename Value>
  TJsonSettingsField(const char *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FJsonFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

  template <typename Value>
  TJsonSettingsField(const TCHAR *FieldAtom, Value Settings::*Member)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member](const FJsonFieldRequest &Request,
                       const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadFieldValue<Value>(Request);
          return Next;
        }) {}

  template <typename Value>
  TJsonSettingsField(
      const char *FieldAtom, Value Settings::*Member,
      Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FJsonFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  template <typename Value>
  TJsonSettingsField(
      const char *FieldAtom, Value Settings::*Member,
      TJsonObjectSettingsReader<Value> ReadObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FJsonFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  template <typename Value>
  TJsonSettingsField(
      const TCHAR *FieldAtom, Value Settings::*Member,
      Value (*ReadObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FJsonFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  template <typename Value>
  TJsonSettingsField(
      const TCHAR *FieldAtom, Value Settings::*Member,
      TJsonObjectSettingsReader<Value> ReadObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, ReadObjectFn](const FJsonFieldRequest &Request,
                                     const Settings &Current) {
          Settings Next = Current;
          Next.*Member = ReadObjectFn(ReadObjectValue(Request));
          return Next;
        }) {}

  template <typename Output>
  TJsonSettingsField(
      const char *FieldAtom, TArray<Output> Settings::*Member,
      Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FJsonFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  template <typename Output>
  TJsonSettingsField(
      const char *FieldAtom, TArray<Output> Settings::*Member,
      TJsonObjectSettingsReader<Output> MapObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FJsonFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  template <typename Output>
  TJsonSettingsField(
      const TCHAR *FieldAtom, TArray<Output> Settings::*Member,
      Output (*MapObjectFn)(const TSharedPtr<FJsonObject> &))
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FJsonFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}

  template <typename Output>
  TJsonSettingsField(
      const TCHAR *FieldAtom, TArray<Output> Settings::*Member,
      TJsonObjectSettingsReader<Output> MapObjectFn)
      : FieldName(SettingsFieldName(FieldAtom)),
        Apply([Member, MapObjectFn](const FJsonFieldRequest &Request,
                                    const Settings &Current) {
          Settings Next = Current;
          Next.*Member = MapJsonValues<Output>(ReadArray(Request), MapObjectFn);
          return Next;
        }) {}
};

// --- Declaration macros (grouped field atom shorthand) --------------------
// These macros own arity expansion so feature call sites declare grouped
// atoms like JSON_SETTINGS_FIELDS(Type, Field1, Field2, ...) instead of
// hand-writing per-field TJsonSettingsField constructors.
// Expansion primitives (JSON_EXPAND, JSON_OBSTRUCT, etc.) live in JsonTypes.h.


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
  __VA_OPT__(, JSON_OBSTRUCT(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST_INDIRECT)()(  \
                    Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, ...)                  \
  JSON_EXPAND(JSON_OBJECT_ARRAY_SETTING_FIELD_LIST(Type, Reader, __VA_ARGS__))
#define JSON_OBJECT_ARRAY_SETTINGS_FIELDS(Type, Reader, ...)                 \
  {JSON_OBJECT_ARRAY_SETTING_FIELDS(Type, Reader, __VA_ARGS__)}

// --- Settings composition (RTK-style fold over field bindings) ------------

template <typename Settings>
Settings
ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                   const TArray<TJsonSettingsField<Settings>> &Fields) {
  return func::fold_indexed(
      Fields, static_cast<size_t>(Fields.Num()), Settings{},
      [Object](const Settings &Current,
               const TJsonSettingsField<Settings> &Binding) {
        return Binding.Apply(Field(Object, *Binding.FieldName), Current);
      });
}

template <typename Settings>
Settings
ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                   std::initializer_list<TJsonSettingsField<Settings>> Fields) {
  return ReadSettingsFields<Settings>(
      Object, TArray<TJsonSettingsField<Settings>>(Fields));
}

// --- Settings registry (RTK entity adapter pattern) ----------------------

#define JSON_SETTING_ATOM(Field) #Field
#define JSON_SETTING_ATOM_LIST_INDIRECT() JSON_SETTING_ATOM_LIST
#define JSON_SETTING_ATOM_LIST(Field, ...)                                   \
  JSON_SETTING_ATOM(Field)                                                   \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_SETTING_ATOM_LIST_INDIRECT)()(             \
                    __VA_ARGS__))
#define JSON_SETTINGS_ATOMS(...)                                             \
  {JSON_EXPAND(JSON_SETTING_ATOM_LIST(__VA_ARGS__))}

template <typename Settings> struct TJsonSettingsRegistry;

#define JSON_SETTINGS_REGISTRY(Type, ...)                                    \
  template <> struct TJsonSettingsRegistry<Type> {                           \
    static const TArray<TJsonSettingsField<Type>> &Fields() {                \
      static const TArray<TJsonSettingsField<Type>> RegisteredFields =       \
          JSON_SETTINGS_FIELDS(Type, __VA_ARGS__);                           \
      return RegisteredFields;                                               \
    }                                                                        \
  }

template <typename Settings>
func::Maybe<TJsonSettingsField<Settings>>
FindRegisteredSettingsField(const char *FieldAtom) {
  const FString FieldName = SettingsFieldName(FieldAtom);
  const TArray<TJsonSettingsField<Settings>> &Fields =
      TJsonSettingsRegistry<Settings>::Fields();
  return func::fold_indexed(
      Fields, static_cast<size_t>(Fields.Num()),
      func::nothing<TJsonSettingsField<Settings>>(),
      [FieldName](const func::Maybe<TJsonSettingsField<Settings>> &Current,
                  const TJsonSettingsField<Settings> &Binding) {
        return Current.hasValue || Binding.FieldName != FieldName
                   ? Current
                   : func::just(Binding);
      });
}

template <typename Settings>
Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                            const TArray<const char *> &FieldAtoms) {
  return func::fold_indexed(
      FieldAtoms, static_cast<size_t>(FieldAtoms.Num()), Settings{},
      [Object](const Settings &Current, const char *FieldAtom) {
        const func::Maybe<TJsonSettingsField<Settings>> Binding =
            FindRegisteredSettingsField<Settings>(FieldAtom);
        check(Binding.hasValue);
        return Binding.value.Apply(Field(Object, *Binding.value.FieldName),
                                   Current);
      });
}

template <typename Settings>
Settings ReadSettingsFields(const TSharedPtr<FJsonObject> &Object,
                            std::initializer_list<const char *> FieldAtoms) {
  return ReadSettingsFields<Settings>(Object, TArray<const char *>(FieldAtoms));
}

template <typename Settings>
TJsonObjectSettingsReader<Settings>
ReadSettingsWith(std::initializer_list<const char *> FieldAtoms) {
  const TArray<const char *> Atoms(FieldAtoms);
  return [Atoms](const TSharedPtr<FJsonObject> &Object) {
    return ReadSettingsFields<Settings>(Object, Atoms);
  };
}

template <typename Settings>
TArray<Settings>
MapSettingsJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values,
                      std::initializer_list<const char *> FieldAtoms) {
  return MapJsonValues<Settings>(Values, ReadSettingsWith<Settings>(FieldAtoms));
}

template <typename Settings>
std::function<TArray<Settings>(const TSharedPtr<FJsonObject> &)>
ReadSettingsObjectArrayField(const char *FieldAtom, std::initializer_list<const char *> FieldAtoms) {
  FString AtomStr(FieldAtom);
  TArray<const char *> AtomsArray(FieldAtoms);
  return [AtomStr, AtomsArray](const TSharedPtr<FJsonObject> &Object) {
    return MapSettingsJsonValues<Settings>(
        ReadArray(Field(Object, *SettingsFieldName(*AtomStr))),
        std::initializer_list<const char *>(AtomsArray.GetData(), AtomsArray.GetData() + AtomsArray.Num()));
  };
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
