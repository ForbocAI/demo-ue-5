#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

typedef TFunction<FJsonFieldRequest(const TCHAR *)> FJsonFieldFactory;
typedef TFunction<FString(const TCHAR *)> FJsonStringReader;
typedef TFunction<float(const TCHAR *)> FJsonFloatReader;
typedef TFunction<int32(const TCHAR *)> FJsonIntReader;
typedef TFunction<bool(const TCHAR *)> FJsonBoolReader;
typedef TFunction<TArray<TSharedPtr<FJsonValue>>(const TCHAR *)>
    FJsonArrayReader;
typedef TFunction<func::Maybe<TSharedPtr<FJsonObject>>(const TCHAR *)>
    FJsonObjectReader;
typedef TFunction<TSharedPtr<FJsonObject>(const TCHAR *)>
    FJsonObjectValueReader;
template <typename Settings>
using TJsonObjectSettingsReader =
    TFunction<Settings(const TSharedPtr<FJsonObject> &)>;
typedef TFunction<FVector(const TCHAR *)> FJsonVectorReader;
typedef TFunction<FRotator(const TCHAR *)> FJsonRotatorReader;

/**
 * @brief Creates a JSON field request from one object and authored field name.
 * @signature FJsonFieldRequest Field(const TSharedPtr<FJsonObject> &Object, const TCHAR *Name)
 *
 * User story: As a data adapter author, I need field lookup requests to be
 * constructed through one neutral factory before higher domains parse payloads.
 */
FJsonFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name);

/**
 * @brief Creates a reusable field request factory for one JSON object.
 * @signature FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a settings adapter author, one object-scoped factory should
 * construct field payloads for every typed reader in that domain.
 */
FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Converts an optional JSON object field into a concrete object pointer.
 * @signature TSharedPtr<FJsonObject> ReadObjectValue(const FJsonFieldRequest &Request)
 *
 * User story: As a runtime settings reader, nested optional object sections can
 * collapse at the JSON boundary instead of leaking lookup logic upward.
 */
TSharedPtr<FJsonObject> ReadObjectValue(const FJsonFieldRequest &Request);

/**
 * @brief Loads a JSON object from the project Content directory.
 * @signature func::Maybe<TSharedPtr<FJsonObject>> LoadObjectFromContent(const FJsonContentObjectRequest &Request)
 *
 * User story: As a gameplay engineer, authored JSON enters RTK flows through a
 * single neutral data boundary before reducers receive typed payloads.
 */
func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request);

/**
 * @brief Loads a required JSON object from the project Content directory.
 * @signature TSharedPtr<FJsonObject> LoadRequiredObjectFromContent(const FJsonContentObjectRequest &Request)
 *
 * User story: As a runtime data boundary, missing authored JSON should fail
 * before actions dispatch instead of producing substitute state.
 */
TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FJsonContentObjectRequest &Request);

/**
 * @brief Reads one string field from a JSON object request.
 * @signature FString ReadString(const FJsonFieldRequest &Request)
 *
 * User story: As a feature reducer author, I can depend on typed settings
 * instead of knowing JSON field lookup details.
 */
FString ReadString(const FJsonFieldRequest &Request);

/**
 * @brief Reads one float field from a JSON object request.
 * @signature float ReadFloat(const FJsonFieldRequest &Request)
 *
 * User story: As a feature reducer author, numeric authored settings arrive as
 * ordinary payload data before reducer composition starts.
 */
float ReadFloat(const FJsonFieldRequest &Request);

/**
 * @brief Reads one integer field from a JSON object request.
 * @signature int32 ReadInt(const FJsonFieldRequest &Request)
 *
 * User story: As a rendering engineer, discrete authored settings stay in JSON
 * without leaking parsing into rendering reducers.
 */
int32 ReadInt(const FJsonFieldRequest &Request);

/**
 * @brief Reads one boolean field from a JSON object request.
 * @signature bool ReadBool(const FJsonFieldRequest &Request)
 *
 * User story: As a domain author, feature flags can be data-backed without
 * views or reducers touching JSON APIs.
 */
bool ReadBool(const FJsonFieldRequest &Request);

/**
 * @brief Reads one JSON array field from a JSON object request.
 * @signature TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request)
 *
 * User story: As a data author, repeated authored records can be mapped into
 * typed payloads before they enter RTK state transitions.
 */
TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request);

/**
 * @brief Reads one child JSON object field from a JSON object request.
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ReadObject(const FJsonFieldRequest &Request)
 *
 * User story: As a systems programmer, nested JSON sections remain an adapter
 * concern and do not shape feature dependencies.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request);

/**
 * @brief Reads a vector from a nested JSON object field.
 * @signature FVector ReadVector(const FJsonFieldRequest &Request)
 *
 * User story: As a presentation reducer author, authored offsets can be reused
 * across player, NPC, and rendering domains as neutral data.
 */
FVector ReadVector(const FJsonFieldRequest &Request);

/**
 * @brief Reads a rotator from a nested JSON object field.
 * @signature FRotator ReadRotator(const FJsonFieldRequest &Request)
 *
 * User story: As a presentation reducer author, authored rotations stay below
 * feature domains until reducers map them into view models.
 */
FRotator ReadRotator(const FJsonFieldRequest &Request);

/**
 * @brief Creates a reusable string reader for one JSON object.
 * @signature FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a domain settings reader, string field lookups should compose
 * through one object-scoped unary function.
 */
FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable float reader for one JSON object.
 * @signature FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As a domain settings reader, numeric field lookups should compose
 * through one object-scoped unary function.
 */
FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable integer reader for one JSON object.
 * @signature FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As rendering settings code, integer cvar fields should read
 * through the same object-scoped unary pattern as other scalars.
 */
FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable boolean reader for one JSON object.
 * @signature FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As data-backed feature code, boolean fields should read through
 * a neutral object-scoped factory instead of one-off payload braces.
 */
FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable array reader for one JSON object.
 * @signature FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As seed adapter code, array fields should read through a neutral
 * object-scoped factory before mapping into action payloads.
 */
FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable optional-object reader for one JSON object.
 * @signature FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As nested settings code, child object lookups should compose via
 * Maybe-returning unary functions.
 */
FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable concrete-object reader for one JSON object.
 * @signature FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As runtime settings composition, optional sections can collapse
 * explicitly at the JSON boundary through one reusable reader.
 */
FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable vector reader for one JSON object.
 * @signature FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As presentation settings code, vector fields should be read by a
 * neutral unary function shared by player, bot, and level domains.
 */
FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object);

/**
 * @brief Creates a reusable rotator reader for one JSON object.
 * @signature FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object)
 *
 * User story: As presentation settings code, rotation fields should share one
 * neutral unary reader instead of repeated field payload construction.
 */
FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object);

inline FString AppendJsonSettingsFieldChar(const FString &Value, TCHAR Char) {
  FString Next = Value;
  Next.AppendChar(Char);
  return Next;
}

inline FString AppendJsonSettingsFieldCharWhen(const FString &Value,
                                               bool bAppend, TCHAR Char) {
  return bAppend ? AppendJsonSettingsFieldChar(Value, Char) : Value;
}

inline FString NormalizeSettingsFieldAtomFrom(const FString &Atom, int32 Index,
                                              int32 StartIndex,
                                              const FString &Acc) {
  const TCHAR Current = Index < Atom.Len() ? Atom[Index] : TCHAR('\0');
  const bool bUpper = FChar::IsUpper(Current);
  const bool bPreviousLowerOrDigit =
      Index > StartIndex &&
      (FChar::IsLower(Atom[Index - 1]) || FChar::IsDigit(Atom[Index - 1]));
  const bool bNextLower =
      Index + 1 < Atom.Len() && FChar::IsLower(Atom[Index + 1]);
  const bool bSeparate =
      bUpper && Index > StartIndex && (bPreviousLowerOrDigit || bNextLower);
  return Index >= Atom.Len()
             ? Acc
             : NormalizeSettingsFieldAtomFrom(
                   Atom, Index + 1, StartIndex,
                   AppendJsonSettingsFieldChar(
                       AppendJsonSettingsFieldCharWhen(Acc, bSeparate,
                                                       TCHAR('_')),
                       FChar::ToLower(Current)));
}

inline FString NormalizeSettingsFieldAtom(const FString &Atom) {
  const int32 StartIndex =
      Atom.Len() > 1 && Atom[0] == TCHAR('b') && FChar::IsUpper(Atom[1]) ? 1
                                                                          : 0;
  return NormalizeSettingsFieldAtomFrom(Atom, StartIndex, StartIndex,
                                        FString());
}

inline FString SettingsFieldName(const TCHAR *Atom) {
  return NormalizeSettingsFieldAtom(FString(Atom));
}

inline FString SettingsFieldName(const char *Atom) {
  return NormalizeSettingsFieldAtom(FString(UTF8_TO_TCHAR(Atom)));
}

template <typename Value> Value ReadFieldValue(const FJsonFieldRequest &Request);

template <>
inline FString ReadFieldValue<FString>(const FJsonFieldRequest &Request) {
  return ReadString(Request);
}

template <>
inline float ReadFieldValue<float>(const FJsonFieldRequest &Request) {
  return ReadFloat(Request);
}

template <>
inline int32 ReadFieldValue<int32>(const FJsonFieldRequest &Request) {
  return ReadInt(Request);
}

template <>
inline bool ReadFieldValue<bool>(const FJsonFieldRequest &Request) {
  return ReadBool(Request);
}

template <>
inline TSharedPtr<FJsonObject>
ReadFieldValue<TSharedPtr<FJsonObject>>(const FJsonFieldRequest &Request) {
  return ReadObjectValue(Request);
}

template <>
inline TArray<TSharedPtr<FJsonValue>>
ReadFieldValue<TArray<TSharedPtr<FJsonValue>>>(
    const FJsonFieldRequest &Request) {
  return ReadArray(Request);
}

template <>
inline FVector ReadFieldValue<FVector>(const FJsonFieldRequest &Request) {
  return ReadVector(Request);
}

template <>
inline FRotator ReadFieldValue<FRotator>(const FJsonFieldRequest &Request) {
  return ReadRotator(Request);
}

inline TArray<FString>
ReadStringArrayValue(const TArray<TSharedPtr<FJsonValue>> &Values) {
  return func::fold_indexed(
      Values, static_cast<size_t>(Values.Num()), TArray<FString>(),
      [](const TArray<FString> &Acc, const TSharedPtr<FJsonValue> &Value) {
        check(Value.IsValid());
        TArray<FString> Next = Acc;
        Next.Add(Value->AsString());
        return Next;
      });
}

inline TArray<FString> ReadStringArray(const FJsonFieldRequest &Request) {
  return ReadStringArrayValue(ReadArray(Request));
}

inline FString ReadStringField(const TSharedPtr<FJsonObject> &Object,
                               const char *FieldAtom) {
  return ReadString(Field(Object, *SettingsFieldName(FieldAtom)));
}

inline TArray<FString> ReadStringArrayField(
    const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) {
  return ReadStringArray(Field(Object, *SettingsFieldName(FieldAtom)));
}

inline TSharedPtr<FJsonObject>
ReadObjectField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) {
  return ReadObjectValue(Field(Object, *SettingsFieldName(FieldAtom)));
}

template <>
inline TArray<FString>
ReadFieldValue<TArray<FString>>(const FJsonFieldRequest &Request) {
  return ReadStringArray(Request);
}

/**
 * @brief Maps JSON object values into typed payloads.
 * @signature template <typename Output> TArray<Output> MapJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values, TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue)
 *
 * User story: As a feature adapter author, repeated authored records can enter
 * actions as typed payloads while ECS owns the neutral array fold shape.
 */
template <typename Output>
TArray<Output>
MapJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values,
              TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue) {
  return func::map_array<TSharedPtr<FJsonValue>, Output>(
      Values, [MapValue](const TSharedPtr<FJsonValue> &Value) {
        const TSharedPtr<FJsonObject> Object =
            Value.IsValid() ? Value->AsObject() : TSharedPtr<FJsonObject>();
        check(Object.IsValid());
        return MapValue(Object);
      });
}

template <typename Output>
TArray<Output>
ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                     const char *FieldAtom,
                     Output (*MapValue)(const TSharedPtr<FJsonObject> &)) {
  return MapJsonValues<Output>(ReadArray(Field(Object, *SettingsFieldName(FieldAtom))),
                               MapValue);
}

template <typename Output>
TArray<Output>
ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                     const char *FieldAtom,
                     TJsonObjectSettingsReader<Output> MapValue) {
  return MapJsonValues<Output>(
      ReadArray(Field(Object, *SettingsFieldName(FieldAtom))), MapValue);
}

template <typename Output> struct TTextValueDeclaration {
  FString Text;
  Output Value;

  TTextValueDeclaration(const char *InText, Output InValue)
      : Text(FString(UTF8_TO_TCHAR(InText))), Value(InValue) {}
};

template <typename Output>
func::Maybe<Output>
ReadTextValue(const FString &Text,
              const TArray<TTextValueDeclaration<Output>> &Declarations) {
  const FString LowerText = Text.ToLower();
  return func::fold_indexed(
      Declarations, static_cast<size_t>(Declarations.Num()),
      func::nothing<Output>(),
      [LowerText](const func::Maybe<Output> &Current,
                  const TTextValueDeclaration<Output> &Declaration) {
        return Current.hasValue || Declaration.Text != LowerText
                   ? Current
                   : func::just(Declaration.Value);
      });
}

template <typename Output>
func::Maybe<Output>
ReadTextValue(const FString &Text,
              std::initializer_list<TTextValueDeclaration<Output>>
                  Declarations) {
  return ReadTextValue<Output>(
      Text, TArray<TTextValueDeclaration<Output>>(Declarations));
}

template <typename Output> struct TJsonTextValueRegistry;

template <typename Output>
func::Maybe<Output> ReadRegisteredTextValue(const FString &Text) {
  return ReadTextValue<Output>(Text,
                               TJsonTextValueRegistry<Output>::Values());
}

template <typename Output>
Output RequireRegisteredTextValue(const FString &Text, const TCHAR *Label) {
  const func::Maybe<Output> Parsed = ReadRegisteredTextValue<Output>(Text);
  checkf(Parsed.hasValue, TEXT("Invalid JSON text value for %s: %s"), Label,
         *Text);
  return Parsed.value;
}

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

#define JSON_EMPTY()
#define JSON_DEFER(Id) Id JSON_EMPTY()
#define JSON_OBSTRUCT(...) __VA_ARGS__ JSON_DEFER(JSON_EMPTY)()
#define JSON_EXPAND(...)                                                     \
  JSON_EXPAND_MORE(                                                          \
      JSON_EXPAND_MORE(                                                      \
          JSON_EXPAND_MORE(                                                  \
              JSON_EXPAND_MORE(                                              \
                  JSON_EXPAND_MORE(                                          \
                      JSON_EXPAND_MORE(                                      \
                          JSON_EXPAND_MORE(                                  \
                              JSON_EXPAND_MORE(__VA_ARGS__))))))))
#define JSON_EXPAND_MORE(...)                                                \
  JSON_EXPAND_DEEP(                                                          \
      JSON_EXPAND_DEEP(                                                      \
          JSON_EXPAND_DEEP(                                                  \
              JSON_EXPAND_DEEP(                                              \
                  JSON_EXPAND_DEEP(                                          \
                      JSON_EXPAND_DEEP(                                      \
                          JSON_EXPAND_DEEP(                                  \
                              JSON_EXPAND_DEEP(__VA_ARGS__))))))))
#define JSON_EXPAND_DEEP(...)                                                \
  JSON_EXPAND_AGAIN(                                                         \
      JSON_EXPAND_AGAIN(                                                     \
          JSON_EXPAND_AGAIN(                                                 \
              JSON_EXPAND_AGAIN(                                             \
                  JSON_EXPAND_AGAIN(                                         \
                      JSON_EXPAND_AGAIN(                                     \
                          JSON_EXPAND_AGAIN(                                 \
                              JSON_EXPAND_AGAIN(__VA_ARGS__))))))))
#define JSON_EXPAND_AGAIN(...) __VA_ARGS__
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
      static const TArray<TJsonSettingsField<Type>> RegisteredFields =        \
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
TArray<Settings>
ReadSettingsObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                             const char *FieldAtom,
                             std::initializer_list<const char *> FieldAtoms) {
  return MapSettingsJsonValues<Settings>(
      ReadArray(Field(Object, *SettingsFieldName(FieldAtom))), FieldAtoms);
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
