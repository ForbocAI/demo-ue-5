#pragma once

#include "Core/ue_fp.hpp"
#include "Features/Components/Data/Json/JsonSlice.h"
#include "Features/Components/Data/Json/JsonTypes.h"

// Json read adapters: neutral boundary projection from JSON objects into typed
// values. These are adapter-layer plumbing shared across all feature domains.
// No state mutation, no domain decisions — just JSON → typed value projection.

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

using JsonReducers::SettingsFieldName;

// --- Reader type aliases (data contracts) --------------------------------
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

// --- Core field construction ---------------------------------------------

/**
 * @brief Creates a JSON field request from one object and authored field name.
 * @signature FJsonFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
 * const TCHAR *Name)
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

// --- Content loading -----------------------------------------------------

TSharedPtr<FJsonObject> ReadObjectValue(const FJsonFieldRequest &Request);

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request);

TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FJsonContentObjectRequest &Request);

func::Maybe<TArray<TSharedPtr<FJsonValue>>>
LoadArrayFromContent(const FJsonContentArrayRequest &Request);

TArray<TSharedPtr<FJsonValue>>
LoadRequiredArrayFromContent(const FJsonContentArrayRequest &Request);

// --- Typed value readers -------------------------------------------------

FString ReadString(const FJsonFieldRequest &Request);
float ReadFloat(const FJsonFieldRequest &Request);
int32 ReadInt(const FJsonFieldRequest &Request);
bool ReadBool(const FJsonFieldRequest &Request);
TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request);
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request);
FVector ReadVector(const FJsonFieldRequest &Request);
FRotator ReadRotator(const FJsonFieldRequest &Request);

// --- Object-scoped reader factories --------------------------------------

FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object);
FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object);
FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object);
FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object);
FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object);
FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object);
FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object);
FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object);
FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object);

// --- ReadFieldValue dispatch (type-driven reader selection) ---------------

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

// --- String array adapters -----------------------------------------------

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

template <>
inline TArray<FString>
ReadFieldValue<TArray<FString>>(const FJsonFieldRequest &Request) {
  return ReadStringArray(Request);
}

// --- Convenience field readers -------------------------------------------

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

// --- Object array mapping ------------------------------------------------

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
                     TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue) {
  return MapJsonValues<Output>(ReadArray(Field(Object, *SettingsFieldName(FieldAtom))),
                               MapValue);
}

template <typename Output>
TArray<Output>
ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                     const char *FieldAtom,
                     TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) {
  return MapJsonValues<Output>(
      ReadArray(Field(Object, *SettingsFieldName(FieldAtom))), MapValue);
}

// --- Text value registry -------------------------------------------------

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

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
