#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Data/Json/JsonAdapters.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

void LogInvalidField(const FJsonFieldRequest &Request);

/**
 * @brief Reads a required string field as a Maybe value.
 *
 * @signature func::Maybe<FString> ReadRequiredString(const FJsonFieldRequest &Request)
 *
 * User story: As a thunk or adapter author, I can reject incomplete authored
 * JSON before dispatching RTK actions into the single store.
 */
func::Maybe<FString> ReadRequiredString(const FJsonFieldRequest &Request);

/**
 * @brief Reads a required float field as a Maybe value.
 *
 * @signature func::Maybe<float> ReadRequiredFloat(const FJsonFieldRequest &Request)
 *
 * User story: As an ECS system adapter, numeric authored fields remain explicit
 * payload data instead of implicit hidden constants.
 */
func::Maybe<float> ReadRequiredFloat(const FJsonFieldRequest &Request);

/**
 * @brief Reads a required child JSON object field.
 *
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ReadRequiredObject(const FJsonFieldRequest &Request)
 *
 * User story: As a feature adapter author, nested JSON ownership stays below
 * reducers while missing required objects short-circuit cleanly.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredObject(const FJsonFieldRequest &Request);

/**
 * @brief Reads a required JSON array field.
 *
 * @signature func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadRequiredArray(const FJsonFieldRequest &Request)
 *
 * User story: As a feature adapter author, repeated authored records enter RTK
 * flows as validated payloads rather than view-time parsing.
 */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredArray(const FJsonFieldRequest &Request);

/**
 * @brief Converts one JSON array value into a required object.
 *
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ReadArrayObject(const FJsonArrayValueObjectRequest &Request)
 *
 * User story: As a data adapter author, array object validation is a neutral
 * primitive reusable by higher gameplay domains.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ReadArrayObject(const FJsonArrayValueObjectRequest &Request);

/**
 * @brief Creates a unary mapper for required JSON object arrays.
 *
 * @signature template <typename Output> TFunction<func::Maybe<TArray<Output>>(const TArray<TSharedPtr<FJsonValue>> &)> MapRequiredJsonValuesWith(const FString &FieldName, TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue)
 *
 * User story: As a feature adapter author, repeated authored ECS seed data can
 * be transformed by a captured parser and fail before reducer dispatch.
 */
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

inline FString RequiredFieldName(const char *FieldAtom) {
  return JsonAdapters::SettingsFieldName(FieldAtom);
}

inline FJsonFieldRequest RequiredField(const TSharedPtr<FJsonObject> &Object,
                                       const char *FieldAtom) {
  return {Object, RequiredFieldName(FieldAtom)};
}

template <typename Value>
func::Maybe<Value> ReadRequiredValue(const FJsonFieldRequest &Request);

template <>
inline func::Maybe<FString>
ReadRequiredValue<FString>(const FJsonFieldRequest &Request) {
  return ReadRequiredString(Request);
}

template <>
inline func::Maybe<float>
ReadRequiredValue<float>(const FJsonFieldRequest &Request) {
  return ReadRequiredFloat(Request);
}

template <>
inline func::Maybe<TSharedPtr<FJsonObject>>
ReadRequiredValue<TSharedPtr<FJsonObject>>(const FJsonFieldRequest &Request) {
  return ReadRequiredObject(Request);
}

template <>
inline func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadRequiredValue<TArray<TSharedPtr<FJsonValue>>>(
    const FJsonFieldRequest &Request) {
  return ReadRequiredArray(Request);
}

template <typename Value>
func::Maybe<Value> ReadRequiredField(const TSharedPtr<FJsonObject> &Object,
                                     const char *FieldAtom) {
  return ReadRequiredValue<Value>(RequiredField(Object, FieldAtom));
}

template <typename State, typename Value>
func::Maybe<State> AssignRequiredField(Value State::*Member,
                                       const State &Current,
                                       const Value &FieldValue) {
  State Next = Current;
  Next.*Member = FieldValue;
  return func::just(Next);
}

template <typename Output>
func::Maybe<TArray<Output>> ReadRequiredObjectArray(
    const FJsonFieldRequest &Request,
    func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &)) {
  return func::mbind(
      ReadRequiredArray(Request),
      [Request,
       MapObject](const TArray<TSharedPtr<FJsonValue>> &Values) {
        return MapRequiredJsonValuesWith<Output>(Request.FieldName,
                                                 MapObject)(Values);
      });
}

template <typename State> struct TRequiredJsonFieldDeclaration {
  FString FieldName;
  TFunction<func::Maybe<State>(const FJsonFieldRequest &, const State &)> Apply;

  template <typename Value>
  TRequiredJsonFieldDeclaration(const char *FieldAtom, Value State::*Member)
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member](const FJsonFieldRequest &Request,
                       const State &Current) {
          return func::mbind(
              ReadRequiredValue<Value>(Request),
              [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  template <typename Value>
  TRequiredJsonFieldDeclaration(
      const char *FieldAtom, Value State::*Member,
      func::Maybe<Value> (*ReadField)(const FJsonFieldRequest &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, ReadField](const FJsonFieldRequest &Request,
                                  const State &Current) {
          return func::mbind(
              ReadField(Request), [Member, Current](const Value &FieldValue) {
                return AssignRequiredField<State, Value>(Member, Current,
                                                         FieldValue);
              });
        }) {}

  template <typename Output>
  TRequiredJsonFieldDeclaration(
      const char *FieldAtom, TArray<Output> State::*Member,
      func::Maybe<Output> (*MapObject)(const TSharedPtr<FJsonObject> &))
      : FieldName(RequiredFieldName(FieldAtom)),
        Apply([Member, MapObject](const FJsonFieldRequest &Request,
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
func::Maybe<State> ReadRequiredFields(
    const TSharedPtr<FJsonObject> &Object,
    std::initializer_list<TRequiredJsonFieldDeclaration<State>> Fields,
    const State &Initial = State()) {
  return func::fold_array<TRequiredJsonFieldDeclaration<State>,
                          func::Maybe<State>>(
      TArray<TRequiredJsonFieldDeclaration<State>>(Fields),
      func::just<State>(Initial),
      [Object](const func::Maybe<State> &Current,
               const TRequiredJsonFieldDeclaration<State> &Field) {
        return func::mbind(Current, [Object, Field](const State &Value) {
          return Field.Apply({Object, Field.FieldName}, Value);
        });
      });
}

#define JSON_REQUIRED_FIELD(Type, Field) {#Field, &Type::Field}
#define JSON_REQUIRED_FIELD_READER(Type, Reader, Field)                      \
  {#Field, &Type::Field, Reader}
#define JSON_REQUIRED_FIELD_LIST_INDIRECT() JSON_REQUIRED_FIELD_LIST
#define JSON_REQUIRED_FIELD_LIST(Type, Field, ...)                           \
  JSON_REQUIRED_FIELD(Type, Field)                                           \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_REQUIRED_FIELD_LIST_INDIRECT)()(           \
                    Type, __VA_ARGS__))
#define JSON_REQUIRED_FIELDS(Type, ...)                                      \
  JSON_EXPAND(JSON_REQUIRED_FIELD_LIST(Type, __VA_ARGS__))
#define JSON_REQUIRED_FIELD_READER_LIST_INDIRECT()                           \
  JSON_REQUIRED_FIELD_READER_LIST
#define JSON_REQUIRED_FIELD_READER_LIST(Type, Reader, Field, ...)            \
  JSON_REQUIRED_FIELD_READER(Type, Reader, Field)                            \
  __VA_OPT__(, JSON_OBSTRUCT(JSON_REQUIRED_FIELD_READER_LIST_INDIRECT)()(    \
                    Type, Reader, __VA_ARGS__))
#define JSON_REQUIRED_FIELD_READERS(Type, Reader, ...)                       \
  JSON_EXPAND(JSON_REQUIRED_FIELD_READER_LIST(Type, Reader, __VA_ARGS__))

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
