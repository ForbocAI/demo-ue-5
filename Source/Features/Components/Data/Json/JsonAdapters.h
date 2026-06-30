#pragma once

#include "Core/ecs.hpp"
#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
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
 * before actions dispatch instead of producing fallback state.
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
  return ecs::mapArray<TSharedPtr<FJsonValue>, Output>(
      Values, [MapValue](const TSharedPtr<FJsonValue> &Value) {
        const TSharedPtr<FJsonObject> Object =
            Value.IsValid() ? Value->AsObject() : TSharedPtr<FJsonObject>();
        check(Object.IsValid());
        return MapValue(Object);
      });
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
