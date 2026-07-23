#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Components/Data/Json/Read/ReadAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

/** User Story: As a data json value consumer, I need to invoke log invalid field through a stable signature so the data json value workflow remains explicit and composable. @fn void LogInvalidField(const FFieldRequest &Request) */
void LogInvalidField(const FFieldRequest &Request);

/** User Story: As a data json value consumer, I need to invoke read required string through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<FString> ReadRequiredString(const FFieldRequest &Request) */
func::Maybe<FString> ReadRequiredString(const FFieldRequest &Request);
/** User Story: As a data json value consumer, I need to invoke read required float through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<float> ReadRequiredFloat(const FFieldRequest &Request) */
func::Maybe<float> ReadRequiredFloat(const FFieldRequest &Request);
/** User Story: As a data json value consumer, I need to invoke read required object through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(const FFieldRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(const FFieldRequest &Request);
/** User Story: As a data json value consumer, I need to invoke read required array through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(const FFieldRequest &Request) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(const FFieldRequest &Request);
/** User Story: As a data json value consumer, I need to invoke read array object through a stable signature so the data json value workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadArrayObject(const FArrayValueObjectRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>> ReadArrayObject(const FArrayValueObjectRequest &Request);

/** User Story: As a data json value consumer, I need to invoke map required json values with through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Output> TFunction<func::Maybe<TArray<Output>>( const TArray<TSharedPtr<FJsonValue>> &)> MapRequiredJsonValuesWith( const FString &FieldName, TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) */
template <typename Output>
TFunction<func::Maybe<TArray<Output>>(
    const TArray<TSharedPtr<FJsonValue>> &)>
MapRequiredJsonValuesWith(
    const FString &FieldName,
    TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue) {
  return [FieldName, MapValue](const TArray<TSharedPtr<FJsonValue>> &Values) {
    return func::traverse_maybe_array_with_index<TSharedPtr<FJsonValue>, Output>(
        Values,
        [FieldName, MapValue](const TSharedPtr<FJsonValue> &Value,
                              int32 Index) {
          return func::mbind(
              ReadArrayObject({Value, FieldName, Index}),
              [MapValue](const TSharedPtr<FJsonObject> &Object) {
                return MapValue(Object);
              });
      });
  };
}

/** User Story: As a data json value consumer, I need to invoke required field name through a stable signature so the data json value workflow remains explicit and composable. @fn inline FString RequiredFieldName(const char *FieldAtom) */
inline FString RequiredFieldName(const char *FieldAtom) {
  return JsonAdapters::SettingsFieldName(FieldAtom);
}

/** User Story: As a data json value consumer, I need to invoke required field through a stable signature so the data json value workflow remains explicit and composable. @fn inline FFieldRequest RequiredField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) */
inline FFieldRequest RequiredField(const TSharedPtr<FJsonObject> &Object,
                                       const char *FieldAtom) {
  return {Object, RequiredFieldName(FieldAtom)};
}

/** User Story: As a data json value consumer, I need to invoke read required value through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Value> func::Maybe<Value> ReadRequiredValue(const FFieldRequest &Request) */
template <typename Value>
func::Maybe<Value> ReadRequiredValue(const FFieldRequest &Request);

template <>
inline func::Maybe<FString>
ReadRequiredValue<FString>(const FFieldRequest &Request) {
  return ReadRequiredString(Request);
}

template <>
inline func::Maybe<float>
ReadRequiredValue<float>(const FFieldRequest &Request) {
  return ReadRequiredFloat(Request);
}

template <>
inline func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredValue<TSharedPtr<FJsonObject>>(const FFieldRequest &Request) {
  return ReadRequiredObject(Request);
}

template <>
inline func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredValue<TArray<TSharedPtr<FJsonValue>>>(
    const FFieldRequest &Request) {
  return ReadRequiredArray(Request);
}

/** User Story: As a data json value consumer, I need to invoke read required field through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Value> func::Maybe<Value> ReadRequiredField(const TSharedPtr<FJsonObject> &Object, const char *FieldAtom) */
template <typename Value>
func::Maybe<Value> ReadRequiredField(const TSharedPtr<FJsonObject> &Object,
                                     const char *FieldAtom) {
  return ReadRequiredValue<Value>(RequiredField(Object, FieldAtom));
}

/** User Story: As a data json value consumer, I need to invoke assign required field through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename State, typename Value> func::Maybe<State> AssignRequiredField(Value State::*Member, const State &Current, const Value &FieldValue) */
template <typename State, typename Value>
func::Maybe<State> AssignRequiredField(Value State::*Member,
                                       const State &Current,
                                       const Value &FieldValue) {
  State Next = Current;
  Next.*Member = FieldValue;
  return func::just(Next);
}

/** User Story: As a data json value consumer, I need to invoke read required object array through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Output> func::Maybe<TArray<Output>> ReadRequiredObjectArray( const FFieldRequest &Request, func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &)) */
template <typename Output>
func::Maybe<TArray<Output>> ReadRequiredObjectArray(
    const FFieldRequest &Request,
    func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &)) {
  return func::mbind(
      ReadRequiredArray(Request),
      [Request,
       MapObject](const TArray<TSharedPtr<FJsonValue>> &Values) {
        return MapRequiredJsonValuesWith<Output>(Request.FieldName,
                                                 MapObject)(Values);
      });
}

template <typename Output> struct TRequiredJsonObjectMapper;

/** User Story: As a data json value consumer, I need to invoke read required mapped object array through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Output> func::Maybe<TArray<Output>> ReadRequiredMappedObjectArray(const FFieldRequest &Request) */
template <typename Output>
func::Maybe<TArray<Output>>
ReadRequiredMappedObjectArray(const FFieldRequest &Request) {
  return ReadRequiredObjectArray<Output>(
      Request, TRequiredJsonObjectMapper<Output>::Read);
}

template <typename State> struct TJsonFieldDeclaration {
  FString FieldName;
  TFunction<func::Maybe<State>(const FFieldRequest &, const State &)> Apply;

  /** User Story: As a data json value consumer, I need to invoke trequired json field declaration through a stable signature so the data json value workflow remains explicit and composable. @fn TJsonFieldDeclaration() */
  TJsonFieldDeclaration()
      : FieldName(),
        Apply([](const FFieldRequest &, const State &Current) {
          return func::just(Current);
        }) {}

  template <typename Output>
  TJsonFieldDeclaration(const char *FieldAtom,
                                TArray<Output> State::*Member)
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const State &Current) {
          return func::mbind(
              ReadRequiredMappedObjectArray<Output>(Request),
              [Member, Current](const TArray<Output> &FieldValue) {
                return AssignRequiredField<State, TArray<Output>>(
                    Member, Current, FieldValue);
              });
        }) {}

  /** User Story: As a data json value consumer, I need to invoke trequired json field declaration through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Value> TJsonFieldDeclaration(const char *FieldAtom, Value State::*Member) */
  template <typename Value>
  TJsonFieldDeclaration(const char *FieldAtom, Value State::*Member)
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member](const FFieldRequest &Request,
                       const State &Current) {
          return func::mbind(
              ReadRequiredValue<Value>(Request),
              [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  /** User Story: As a data json value consumer, I need to invoke trequired json field declaration through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename Value> TJsonFieldDeclaration( const char *FieldAtom, Value State::*Member, func::Maybe<Value> (*ReadValue)(const FFieldRequest &)) */
  template <typename Value>
  TJsonFieldDeclaration(
      const char *FieldAtom, Value State::*Member,
      func::Maybe<Value> (*ReadValue)(const FFieldRequest &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, ReadValue](const FFieldRequest &Request,
                                  const State &Current) {
          return func::mbind(
              ReadValue(Request), [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  template <typename Output>
  TJsonFieldDeclaration(
      const char *FieldAtom, TArray<Output> State::*Member,
      func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, MapObject](const FFieldRequest &Request,
                                  const State &Current) {
          return func::mbind(
              ReadRequiredObjectArray<Output>(Request, MapObject),
              [Member, Current](const TArray<Output> &FieldValue) {
                return AssignRequiredField<State, TArray<Output>>(
                    Member, Current, FieldValue);
              });
        }) {}
};

template <typename State>
struct FJsonReadStateRequest {
  State Initial;
  const TSharedPtr<FJsonObject> &Object;
};

/** User Story: As a data json value consumer, I need to invoke read required fields through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename State> func::Maybe<State> ReadRequiredFields( const FJsonReadStateRequest<State> &Request, const TArray<TJsonFieldDeclaration<State>> &Fields) */
template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    const TArray<TJsonFieldDeclaration<State>> &Fields) {
  return func::fold_array<TJsonFieldDeclaration<State>,
                          func::Maybe<State>>(
      Fields, func::just<State>(Request.Initial),
      [Object = Request.Object](const func::Maybe<State> &Current,
               const TJsonFieldDeclaration<State> &Field) {
        return func::mbind(Current, [Object, Field](const State &Value) {
          return Field.Apply({Object, Field.FieldName}, Value);
        });
      });
}

/** User Story: As a data json value consumer, I need to invoke read required fields through a stable signature so the data json value workflow remains explicit and composable. @fn template <typename State> func::Maybe<State> ReadRequiredFields( const FJsonReadStateRequest<State> &Request, std::initializer_list<TJsonFieldDeclaration<State>> Fields) */
template <typename State>
func::Maybe<State> ReadRequiredFields(
    const FJsonReadStateRequest<State> &Request,
    std::initializer_list<TJsonFieldDeclaration<State>> Fields) {
  return ReadRequiredFields<State>(Request, TArray<TJsonFieldDeclaration<State>>(Fields));
}

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
