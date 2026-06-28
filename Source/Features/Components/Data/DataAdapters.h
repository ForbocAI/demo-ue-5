#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataAdapters {

/**
 * @brief Loads a JSON object from the project Content directory.
 *
 * @signature func::Maybe<TSharedPtr<FJsonObject>>
 * LoadObjectFromContent(const FJsonContentObjectRequest &Request)
 *
 * User story: As a gameplay engineer, I can keep authored demo data in JSON
 * while reducers receive typed payloads through one data boundary.
 */
func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request);

/**
 * @brief Reads one string field from a JSON object request.
 *
 * @signature FString ReadString(const FJsonFieldRequest &Request)
 *
 * User story: As a feature reducer author, I can depend on typed settings
 * instead of knowing JSON field lookup details.
 */
FString ReadString(const FJsonFieldRequest &Request);

/**
 * @brief Reads one float field from a JSON object request.
 *
 * @signature float ReadFloat(const FJsonFieldRequest &Request)
 *
 * User story: As a feature reducer author, numeric authored settings arrive as
 * ordinary C++ values before reducer composition starts.
 */
float ReadFloat(const FJsonFieldRequest &Request);

/**
 * @brief Reads one integer field from a JSON object request.
 *
 * @signature int32 ReadInt(const FJsonFieldRequest &Request)
 *
 * User story: As a rendering engineer, discrete authored settings like cvar
 * modes can stay in JSON without leaking parsing into rendering reducers.
 */
int32 ReadInt(const FJsonFieldRequest &Request);

/**
 * @brief Reads one boolean field from a JSON object request.
 *
 * @signature bool ReadBool(const FJsonFieldRequest &Request)
 *
 * User story: As a domain author, optional feature flags can be data-backed
 * without views or reducers touching JSON APIs.
 */
bool ReadBool(const FJsonFieldRequest &Request);

/**
 * @brief Reads one JSON array field from a JSON object request.
 *
 * @signature TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest
 * &Request)
 *
 * User story: As a data author, repeated authored records can be mapped into
 * typed payloads before they enter RTK state transitions.
 */
TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request);

/**
 * @brief Reads one child JSON object field from a JSON object request.
 *
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ReadObject(const
 * FJsonFieldRequest &Request)
 *
 * User story: As a systems programmer, nested JSON sections remain an adapter
 * concern and do not shape feature dependencies.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request);

/**
 * @brief Reads a vector from a nested JSON object field.
 *
 * @signature FVector ReadVector(const FJsonFieldRequest &Request)
 *
 * User story: As a presentation reducer author, authored offsets can be reused
 * across player, NPC, and rendering domains as neutral data.
 */
FVector ReadVector(const FJsonFieldRequest &Request);

/**
 * @brief Reads a rotator from a nested JSON object field.
 *
 * @signature FRotator ReadRotator(const FJsonFieldRequest &Request)
 *
 * User story: As a presentation reducer author, authored rotations stay below
 * feature domains until reducers map them into view models.
 */
FRotator ReadRotator(const FJsonFieldRequest &Request);

/**
 * @brief Loads all authored UE demo runtime settings from JSON.
 *
 * @signature FDemoRuntimeSettings LoadDemoRuntimeSettings()
 *
 * User story: As a runtime engineer, one store initialization path can load
 * data once and reduce it into the single RTK store before views render.
 */
FDemoRuntimeSettings LoadDemoRuntimeSettings();

namespace detail {

template <typename Source, typename Output>
TArray<Output> MapArrayRecursive(TArrayMapStep<Source, Output> Step) {
  return Step.Index >= Step.Values.Num()
             ? Step.Acc
             : ([Step](TArray<Output> Next) mutable {
                 Next.Add(Step.MapValue(Step.Values[Step.Index]));
                 Step.Acc = MoveTemp(Next);
                 Step.Index = Step.Index + 1;
                 return MapArrayRecursive<Source, Output>(MoveTemp(Step));
               })(MoveTemp(Step.Acc));
}

template <typename Source, typename Output>
TArray<Output> FilterMapArrayRecursive(
    TArrayFilterMapStep<Source, Output> Step) {
  return Step.Index >= Step.Values.Num()
             ? Step.Acc
             : ([Step](TArray<Output> Next) mutable {
                 Step.Keep(Step.Values[Step.Index])
                     ? (Next.Add(Step.MapValue(Step.Values[Step.Index])),
                        void())
                     : void();
                 Step.Acc = MoveTemp(Next);
                 Step.Index = Step.Index + 1;
                 return FilterMapArrayRecursive<Source, Output>(
                     MoveTemp(Step));
               })(MoveTemp(Step.Acc));
}

template <typename Output>
TArray<Output> MapJsonValuesRecursive(TJsonValueMapStep<Output> Step) {
  return Step.Index >= Step.Values.Num()
             ? Step.Acc
             : ([Step](TArray<Output> Next) mutable {
                 Next.Add(Step.MapValue(Step.Values[Step.Index]->AsObject()));
                 Step.Acc = MoveTemp(Next);
                 Step.Index = Step.Index + 1;
                 return MapJsonValuesRecursive<Output>(MoveTemp(Step));
               })(MoveTemp(Step.Acc));
}

} // namespace detail

template <typename Source, typename Output>
TArray<Output> MapArray(TArrayMapRequest<Source, Output> Request) {
  TArray<Output> Acc;
  Acc.Reserve(Request.Values.Num());
  return detail::MapArrayRecursive<Source, Output>(
      {MoveTemp(Request.Values), MoveTemp(Request.MapValue), 0,
       MoveTemp(Acc)});
}

template <typename Source, typename Output>
TArray<Output> FilterMapArray(
    TArrayFilterMapRequest<Source, Output> Request) {
  TArray<Output> Acc;
  Acc.Reserve(Request.Values.Num());
  return detail::FilterMapArrayRecursive<Source, Output>(
      {MoveTemp(Request.Values), MoveTemp(Request.Keep),
       MoveTemp(Request.MapValue), 0, MoveTemp(Acc)});
}

template <typename Output>
TArray<Output> MapJsonValues(TJsonValueMapRequest<Output> Request) {
  TArray<Output> Acc;
  Acc.Reserve(Request.Values.Num());
  return detail::MapJsonValuesRecursive<Output>(
      {MoveTemp(Request.Values), MoveTemp(Request.MapValue), 0, MoveTemp(Acc)});
}

} // namespace DataAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
