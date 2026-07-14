#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Misc/Paths.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {
namespace {

FString ResolvePartPath(const FString &ParentRelativePath,
                        const FString &PartRelativePath) {
  return FPaths::Combine(FPaths::GetPath(ParentRelativePath),
                         PartRelativePath);
}

FString ResolvePartPath(const FString &ParentRelativePath,
                        const TSharedPtr<FJsonValue> &Value) {
  check(Value.IsValid());
  return ResolvePartPath(ParentRelativePath, Value->AsString());
}

const FString &ManifestPartsFieldName() {
  static const FString Name = SettingsFieldName("Parts");
  return Name;
}

const FString &ManifestMergeFieldName() {
  static const FString Name = SettingsFieldName("Merge");
  return Name;
}

bool ReadPartValues(const TSharedPtr<FJsonObject> &Object,
                    const TArray<TSharedPtr<FJsonValue>> *&Parts) {
  return Object.IsValid() &&
         Object->TryGetArrayField(ManifestPartsFieldName(), Parts) &&
         Parts != nullptr;
}

bool ReadPartObject(const TSharedPtr<FJsonObject> &Object,
                    const TSharedPtr<FJsonObject> *&Parts) {
  return Object.IsValid() &&
         Object->TryGetObjectField(ManifestPartsFieldName(), Parts) &&
         Parts != nullptr && Parts->IsValid();
}

struct FSetJsonObjectFieldRequest {
  TSharedPtr<FJsonObject> Object;
  FString FieldName;
  TSharedPtr<FJsonValue> Value;
};

struct FFieldPartRequest {
  FString RelativePath;
  TSharedPtr<FJsonObject> ManifestParts;
  TSharedPtr<FJsonObject> Current;
  FString PartKey;
};

TSharedPtr<FJsonObject> MergeJsonObjects(
    const TSharedPtr<FJsonObject> &Left,
    const TSharedPtr<FJsonObject> &Right) {
  TSharedPtr<FJsonObject> Merged = MakeShared<FJsonObject>();
  Merged->Values = Left->Values;
  Merged->Values.Append(Right->Values);
  return Merged;
}

TSharedPtr<FJsonObject> SetJsonObjectField(
    const FSetJsonObjectFieldRequest &Request) {
  TSharedPtr<FJsonObject> Next = MakeShared<FJsonObject>();
  Next->Values = Request.Object->Values;
  Next->SetField(Request.FieldName, Request.Value);
  return Next;
}

func::Maybe<TSharedPtr<FJsonValue>>
LoadManifestFieldValue(const FString &ParentRelativePath,
                       const FString &PartRelativePath) {
  const FString RelativePath = ResolvePartPath(ParentRelativePath,
                                               PartRelativePath);
  const func::Maybe<TSharedPtr<FJsonObject>> Object =
      LoadObjectFromContent({RelativePath});
  return Object.hasValue
             ? func::just<TSharedPtr<FJsonValue>>(
                   MakeShared<FJsonValueObject>(Object.value))
             : func::mbind(
                   LoadArrayFromContent({RelativePath}),
                   [](const TArray<TSharedPtr<FJsonValue>> &Array) {
                     return func::just<TSharedPtr<FJsonValue>>(
                         MakeShared<FJsonValueArray>(Array));
                   });
}

TSharedPtr<FJsonObject>
LoadObjectManifestMergeParts(const FString &RelativePath,
                             const TSharedPtr<FJsonObject> &ManifestParts) {
  const TArray<TSharedPtr<FJsonValue>> *MergeParts = nullptr;
  const TSharedPtr<FJsonObject> Empty = MakeShared<FJsonObject>();
  return ManifestParts->TryGetArrayField(ManifestMergeFieldName(),
                                         MergeParts) &&
                 MergeParts != nullptr
             ? func::fold_indexed(
                   *MergeParts, static_cast<size_t>(MergeParts->Num()),
                   Empty,
                   [&RelativePath](const TSharedPtr<FJsonObject> &Current,
                                   const TSharedPtr<FJsonValue> &Part) {
                     const func::Maybe<TSharedPtr<FJsonObject>> Object =
                         LoadObjectFromContent({ResolvePartPath(RelativePath,
                                                                Part)});
                     check(Object.hasValue);
                     return MergeJsonObjects(Current, Object.value);
                   })
             : MakeShared<FJsonObject>();
}

TSharedPtr<FJsonObject>
LoadObjectManifestFieldPart(const FFieldPartRequest &Request) {
  const TSharedPtr<FJsonValue> PartValue =
      Request.ManifestParts->TryGetField(Request.PartKey);
  check(PartValue.IsValid());
  const func::Maybe<TSharedPtr<FJsonValue>> Value =
      LoadManifestFieldValue(Request.RelativePath, PartValue->AsString());
  check(Value.hasValue);
  return SetJsonObjectField({Request.Current, Request.PartKey, Value.value});
}

TSharedPtr<FJsonObject>
LoadObjectManifestFieldParts(const FString &RelativePath,
                             const TSharedPtr<FJsonObject> &ManifestParts) {
  TArray<FJsonObject::FStringType> StoredPartKeys;
  ManifestParts->Values.GenerateKeyArray(StoredPartKeys);
  const TArray<FString> PartKeys =
      func::map_array<FJsonObject::FStringType, FString>(
          StoredPartKeys, [](const FJsonObject::FStringType &PartKey) {
            return FString(PartKey.ToView());
          });
  return func::fold_indexed(
      PartKeys, static_cast<size_t>(PartKeys.Num()),
      LoadObjectManifestMergeParts(RelativePath, ManifestParts),
      [&RelativePath, ManifestParts](
          const TSharedPtr<FJsonObject> &Current,
          const FString &PartKey) {
        return PartKey == ManifestMergeFieldName()
                   ? Current
                   : LoadObjectManifestFieldPart(
                         {RelativePath, ManifestParts, Current, PartKey});
      });
}

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectManifestParts(const FString &RelativePath,
                        const TSharedPtr<FJsonObject> &Manifest) {
  const TArray<TSharedPtr<FJsonValue>> *Parts = nullptr;
  const TSharedPtr<FJsonObject> *PartObject = nullptr;
  const TSharedPtr<FJsonObject> Empty = MakeShared<FJsonObject>();
  return ReadPartValues(Manifest, Parts)
             ? func::just<TSharedPtr<FJsonObject>>(func::fold_indexed(
                   *Parts, static_cast<size_t>(Parts->Num()),
                   Empty,
                   [&RelativePath](const TSharedPtr<FJsonObject> &Current,
                                   const TSharedPtr<FJsonValue> &Part) {
                     const func::Maybe<TSharedPtr<FJsonObject>> Object =
                         LoadObjectFromContent({ResolvePartPath(RelativePath,
                                                                Part)});
                     check(Object.hasValue);
                     return MergeJsonObjects(Current, Object.value);
                   }))
             : (ReadPartObject(Manifest, PartObject)
                    ? func::just(LoadObjectManifestFieldParts(RelativePath,
                                                              *PartObject))
                    : func::nothing<TSharedPtr<FJsonObject>>());
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
