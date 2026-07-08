#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Misc/Paths.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {
namespace {

FString ResolvePartPath(const FString &ParentRelativePath,
                        const TSharedPtr<FJsonValue> &Value) {
  check(Value.IsValid());
  return FPaths::Combine(FPaths::GetPath(ParentRelativePath),
                         Value->AsString());
}

bool ReadPartValues(const TSharedPtr<FJsonObject> &Object,
                    const TArray<TSharedPtr<FJsonValue>> *&Parts) {
  return Object.IsValid() && Object->TryGetArrayField(TEXT("parts"), Parts) &&
         Parts != nullptr;
}

TSharedPtr<FJsonObject> MergeJsonObjects(
    const TSharedPtr<FJsonObject> &Left,
    const TSharedPtr<FJsonObject> &Right) {
  TSharedPtr<FJsonObject> Merged = MakeShared<FJsonObject>();
  for (const TPair<FString, TSharedPtr<FJsonValue>> &Pair : Left->Values) {
    Merged->SetField(Pair.Key, Pair.Value);
  }
  for (const TPair<FString, TSharedPtr<FJsonValue>> &Pair : Right->Values) {
    Merged->SetField(Pair.Key, Pair.Value);
  }
  return Merged;
}

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectManifestParts(const FString &RelativePath,
                        const TSharedPtr<FJsonObject> &Manifest) {
  const TArray<TSharedPtr<FJsonValue>> *Parts = nullptr;
  return ReadPartValues(Manifest, Parts)
             ? func::just(func::fold_indexed(
                   *Parts, static_cast<size_t>(Parts->Num()),
                   MakeShared<FJsonObject>(),
                   [&RelativePath](const TSharedPtr<FJsonObject> &Current,
                                   const TSharedPtr<FJsonValue> &Part) {
                     const func::Maybe<TSharedPtr<FJsonObject>> Object =
                         LoadObjectFromContent({ResolvePartPath(RelativePath,
                                                                Part)});
                     check(Object.hasValue);
                     return MergeJsonObjects(Current, Object.value);
                   }))
             : func::nothing<TSharedPtr<FJsonObject>>();
}

func::Maybe<TArray<TSharedPtr<FJsonValue>>>
LoadArrayManifestParts(const FString &RelativePath,
                       const TSharedPtr<FJsonObject> &Manifest) {
  const TArray<TSharedPtr<FJsonValue>> *Parts = nullptr;
  return ReadPartValues(Manifest, Parts)
             ? func::just(func::fold_indexed(
                   *Parts, static_cast<size_t>(Parts->Num()),
                   TArray<TSharedPtr<FJsonValue>>(),
                   [&RelativePath](const TArray<TSharedPtr<FJsonValue>> &Current,
                                   const TSharedPtr<FJsonValue> &Part) {
                     const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Values =
                         LoadArrayFromContent({ResolvePartPath(RelativePath,
                                                               Part)});
                     check(Values.hasValue);
                     TArray<TSharedPtr<FJsonValue>> Next = Current;
                     Next.Append(Values.value);
                     return Next;
                   }))
             : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
}

} // namespace
} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
