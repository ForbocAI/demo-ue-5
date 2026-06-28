#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

/**
 * @brief Logs one invalid required JSON field.
 *
 * @signature void LogInvalidField(const FJsonFieldRequest &Request)
 *
 * User story: As a feature adapter author, I can share required-field logging
 * without importing helpers from a sibling feature domain.
 */
void LogInvalidField(const FJsonFieldRequest &Request);

/**
 * @brief Logs one invalid JSON array object value.
 *
 * @signature void LogInvalidArrayValue(const FJsonArrayValueObjectRequest &Request)
 *
 * User story: As a data author, invalid authored array entries report the
 * field and index from the neutral data layer before reducers receive payloads.
 */
void LogInvalidArrayValue(const FJsonArrayValueObjectRequest &Request);

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

namespace detail {

/**
 * @brief Recursively maps required JSON object values into typed outputs.
 *
 * @signature template <typename Output> func::Maybe<TArray<Output>> MapRequiredJsonValuesRecursive(const TArray<TSharedPtr<FJsonValue>> &Values, const FString &FieldName, TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue, int32 Index, TArray<Output> Acc)
 *
 * User story: As a shared data primitive, array mapping can stay lazy-looking
 * and unary while higher feature domains import downward into it.
 */
template <typename Output>
func::Maybe<TArray<Output>>
MapRequiredJsonValuesRecursive(
    const TArray<TSharedPtr<FJsonValue>> &Values, const FString &FieldName,
    TFunction<func::Maybe<Output>(const TSharedPtr<FJsonObject> &)> MapValue,
    int32 Index, TArray<Output> Acc) {
  return Index >= Values.Num()
             ? func::just(Acc)
             : func::mbind(
                   ReadArrayObject({Values[Index], FieldName, Index}),
                   [&Values, FieldName, MapValue, Index, Acc](
                       const TSharedPtr<FJsonObject> &Object) mutable {
                     return func::mbind(
                         MapValue(Object),
                         [&Values, FieldName, MapValue, Index,
                          Acc](const Output &Parsed) mutable {
                           return MapRequiredJsonValuesRecursive<Output>(
                               Values, FieldName, MapValue, Index + 1,
                               ecs::appendValue<Output>(Acc, Parsed));
                         });
                   });
}

} // namespace detail

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
    TArray<Output> Acc;
    Acc.Reserve(Values.Num());
    return detail::MapRequiredJsonValuesRecursive<Output>(
        Values, FieldName, MapValue, 0, MoveTemp(Acc));
  };
}

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
