#pragma once

#include "Core/fp.hpp"
#include "Features/Components/Data/Json/JsonSlice.h"
#include "Features/Components/Data/Json/DataJsonTypes.h"

// Json read adapters: neutral boundary projection from JSON objects into typed
// values. These are adapter-layer plumbing shared across all feature domains.
// No state mutation, no domain decisions — just JSON → typed value projection.

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

using JsonReducers::SettingsFieldName;

// --- Reader type aliases (data contracts) --------------------------------
typedef TFunction<FFieldRequest(const TCHAR *)> FJsonFieldFactory;
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
 * @fn FFieldRequest Field(const TSharedPtr<FJsonObject> &Object, const TCHAR *Name)
 * @brief Creates a JSON field request from one object and authored field name.
 *
 * User story: As a data adapter author, I need field lookup requests to be
 * constructed through one neutral factory before higher domains parse payloads.
 * User Story: As a data json read consumer, I need to invoke field through a stable signature so the data json read workflow remains explicit and composable.
 */
FFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name);

/**
 * @fn FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object)
 * @brief Creates a reusable field request factory for one JSON object.
 *
 * User story: As a settings adapter author, one object-scoped factory should
 * construct field payloads for every typed reader in that domain.
 * User Story: As a data json read consumer, I need to invoke field in through a stable signature so the data json read workflow remains explicit and composable.
 */
FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object);

// --- Content loading -----------------------------------------------------

/** User Story: As a data json read consumer, I need to invoke read object value through a stable signature so the data json read workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> ReadObjectValue(const FFieldRequest &Request) */
TSharedPtr<FJsonObject> ReadObjectValue(const FFieldRequest &Request);

/** User Story: As a data json read consumer, I need to invoke load object from content through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> LoadObjectFromContent(const FContentObjectRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FContentObjectRequest &Request);

/** User Story: As a data json read consumer, I need to invoke load required object from content through a stable signature so the data json read workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadRequiredObjectFromContent(const FContentObjectRequest &Request) */
TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FContentObjectRequest &Request);

/** User Story: As a data json read consumer, I need to invoke load array from content through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> LoadArrayFromContent(const FContentArrayRequest &Request) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
LoadArrayFromContent(const FContentArrayRequest &Request);

/** User Story: As a data json read consumer, I need to invoke load required array from content through a stable signature so the data json read workflow remains explicit and composable. @fn TArray<TSharedPtr<FJsonValue>> LoadRequiredArrayFromContent(const FContentArrayRequest &Request) */
TArray<TSharedPtr<FJsonValue>>
LoadRequiredArrayFromContent(const FContentArrayRequest &Request);

// --- Typed value readers -------------------------------------------------

/** User Story: As a data json read consumer, I need to invoke read string through a stable signature so the data json read workflow remains explicit and composable. @fn FString ReadString(const FFieldRequest &Request) */
FString ReadString(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read float through a stable signature so the data json read workflow remains explicit and composable. @fn float ReadFloat(const FFieldRequest &Request) */
float ReadFloat(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read int through a stable signature so the data json read workflow remains explicit and composable. @fn int32 ReadInt(const FFieldRequest &Request) */
int32 ReadInt(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read bool through a stable signature so the data json read workflow remains explicit and composable. @fn bool ReadBool(const FFieldRequest &Request) */
bool ReadBool(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read array through a stable signature so the data json read workflow remains explicit and composable. @fn TArray<TSharedPtr<FJsonValue>> ReadArray(const FFieldRequest &Request) */
TArray<TSharedPtr<FJsonValue>> ReadArray(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read object through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadObject(const FFieldRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read vector through a stable signature so the data json read workflow remains explicit and composable. @fn FVector ReadVector(const FFieldRequest &Request) */
FVector ReadVector(const FFieldRequest &Request);
/** User Story: As a data json read consumer, I need to invoke read rotator through a stable signature so the data json read workflow remains explicit and composable. @fn FRotator ReadRotator(const FFieldRequest &Request) */
FRotator ReadRotator(const FFieldRequest &Request);

// --- Object-scoped reader factories --------------------------------------

/** User Story: As a data json read consumer, I need to invoke string in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object) */
FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke float in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object) */
FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke int in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object) */
FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke bool in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object) */
FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke array in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object) */
FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke object in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object) */
FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke object value in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object) */
FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke vector in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object) */
FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object);
/** User Story: As a data json read consumer, I need to invoke rotator in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object) */
FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object);

// --- ReadFieldValue dispatch (type-driven reader selection) ---------------

/** User Story: As a data json read consumer, I need to invoke read field value through a stable signature so the data json read workflow remains explicit and composable. @fn template <typename Value> Value ReadFieldValue(const FFieldRequest &Request) */
template <typename Value> Value ReadFieldValue(const FFieldRequest &Request);

template <>
inline FString ReadFieldValue<FString>(const FFieldRequest &Request) {
  return ReadString(Request);
}

template <>
inline float ReadFieldValue<float>(const FFieldRequest &Request) {
  return ReadFloat(Request);
}

template <>
inline int32 ReadFieldValue<int32>(const FFieldRequest &Request) {
  return ReadInt(Request);
}

template <>
inline bool ReadFieldValue<bool>(const FFieldRequest &Request) {
  return ReadBool(Request);
}

template <>
inline TSharedPtr<FJsonObject>
ReadFieldValue<TSharedPtr<FJsonObject>>(const FFieldRequest &Request) {
  return ReadObjectValue(Request);
}

template <>
inline TArray<TSharedPtr<FJsonValue>>
ReadFieldValue<TArray<TSharedPtr<FJsonValue>>>(
    const FFieldRequest &Request) {
  return ReadArray(Request);
}

template <>
inline FVector ReadFieldValue<FVector>(const FFieldRequest &Request) {
  return ReadVector(Request);
}

template <>
inline FRotator ReadFieldValue<FRotator>(const FFieldRequest &Request) {
  return ReadRotator(Request);
}

// --- String array adapters -----------------------------------------------

/** User Story: As a data json read consumer, I need to invoke read string array value through a stable signature so the data json read workflow remains explicit and composable. @fn inline TArray<FString> ReadStringArrayValue(const TArray<TSharedPtr<FJsonValue>> &Values) */
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

/** User Story: As a data json read consumer, I need to invoke read string array through a stable signature so the data json read workflow remains explicit and composable. @fn inline TArray<FString> ReadStringArray(const FFieldRequest &Request) */
inline TArray<FString> ReadStringArray(const FFieldRequest &Request) {
  return ReadStringArrayValue(ReadArray(Request));
}

template <>
inline TArray<FString>
ReadFieldValue<TArray<FString>>(const FFieldRequest &Request) {
  return ReadStringArray(Request);
}

// --- Convenience field readers -------------------------------------------

/** User Story: As a data json read consumer, I need to invoke read string field through a stable signature so the data json read workflow remains explicit and composable. @fn inline FString ReadStringField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) */
inline FString ReadStringField(const TSharedPtr<FJsonObject> &Object,
                               const char *FieldAtom) {
  return ReadString(Field(Object, *SettingsFieldName(FieldAtom)));
}

/** User Story: As a data json read consumer, I need to invoke read string array field through a stable signature so the data json read workflow remains explicit and composable. @fn inline TArray<FString> ReadStringArrayField( const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) */
inline TArray<FString> ReadStringArrayField(
    const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) {
  return ReadStringArray(Field(Object, *SettingsFieldName(FieldAtom)));
}

/** User Story: As a data json read consumer, I need to invoke read object field through a stable signature so the data json read workflow remains explicit and composable. @fn inline TSharedPtr<FJsonObject> ReadObjectField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) */
inline TSharedPtr<FJsonObject>
ReadObjectField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) {
  return ReadObjectValue(Field(Object, *SettingsFieldName(FieldAtom)));
}

// --- Object array mapping ------------------------------------------------

/** User Story: As a data json read consumer, I need to invoke map json values through a stable signature so the data json read workflow remains explicit and composable. @fn template <typename Output> TArray<Output> MapJsonValues(const TArray<TSharedPtr<FJsonValue>> &Values, TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue) */
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

/** User Story: As a data json read consumer, I need to invoke read object array field through a stable signature so the data json read workflow remains explicit and composable. @fn template <typename Output> TArray<Output> ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom, TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue) */
template <typename Output>
TArray<Output>
ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                     const char *FieldAtom,
                     TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue) {
  return MapJsonValues<Output>(ReadArray(Field(Object, *SettingsFieldName(FieldAtom))),
                               MapValue);
}

/** User Story: As a data json read consumer, I need to invoke read object array field through a stable signature so the data json read workflow remains explicit and composable. @fn template <typename Output> TArray<Output> ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom, TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) */
template <typename Output>
TArray<Output>
ReadObjectArrayField(const TSharedPtr<FJsonObject> &Object,
                     const char *FieldAtom,
                     TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) {
  return MapJsonValues<Output>(
      ReadArray(Field(Object, *SettingsFieldName(FieldAtom))), MapValue);
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
