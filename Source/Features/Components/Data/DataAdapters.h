#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataAdapters {

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request);
FString ReadString(const FJsonFieldRequest &Request);
float ReadFloat(const FJsonFieldRequest &Request);
bool ReadBool(const FJsonFieldRequest &Request);
TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request);
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request);

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
