#pragma once

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Misc/Paths.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {
namespace {

/** User Story: As a data json manifest consumer, I need to invoke resolve part path through a stable signature so the data json manifest workflow remains explicit and composable. @fn FString ResolvePartPath(const FString &ParentRelativePath, const FString &PartRelativePath) */
FString ResolvePartPath(const FString &ParentRelativePath,
                        const FString &PartRelativePath) {
  return FPaths::Combine(FPaths::GetPath(ParentRelativePath),
                         PartRelativePath);
}

/** User Story: As a data json manifest consumer, I need to invoke resolve part path through a stable signature so the data json manifest workflow remains explicit and composable. @fn FString ResolvePartPath(const FString &ParentRelativePath, const TSharedPtr<FJsonValue> &Value) */
FString ResolvePartPath(const FString &ParentRelativePath,
                        const TSharedPtr<FJsonValue> &Value) {
  const func::Maybe<TSharedPtr<FJsonValue>> Present =
      func::from_shared(Value);
  check(Present.hasValue);
  return ResolvePartPath(ParentRelativePath, Present.value->AsString());
}

/** User Story: As a data json manifest consumer, I need to invoke manifest parts field name through a stable signature so the data json manifest workflow remains explicit and composable. @fn const FString &ManifestPartsFieldName() */
const FString &ManifestPartsFieldName() {
  static const FString Name = SettingsFieldName("Parts");
  return Name;
}

/** User Story: As a data json manifest consumer, I need to invoke manifest merge field name through a stable signature so the data json manifest workflow remains explicit and composable. @fn const FString &ManifestMergeFieldName() */
const FString &ManifestMergeFieldName() {
  static const FString Name = SettingsFieldName("Merge");
  return Name;
}

/** User Story: As manifest array loading, I need optional part declarations lifted once so callers compose over a Maybe instead of sharing nullable output pointers. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> ReadPartValues(const TSharedPtr<FJsonObject> &Object) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ReadPartValues(const TSharedPtr<FJsonObject> &Object) {
  return func::mbind(
      func::from_shared(Object),
      [](const TSharedPtr<FJsonObject> &Present) {
        const TArray<TSharedPtr<FJsonValue>> *Parts = nullptr;
        return Present->TryGetArrayField(ManifestPartsFieldName(), Parts)
                   ? func::from_nullable(Parts)
                   : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
      });
}

/** User Story: As manifest object loading, I need optional keyed part declarations lifted once so callers compose over a Maybe instead of sharing nullable output pointers. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadPartObject(const TSharedPtr<FJsonObject> &Object) */
func::Maybe<TSharedPtr<FJsonObject>>
ReadPartObject(const TSharedPtr<FJsonObject> &Object) {
  return func::mbind(
      func::from_shared(Object),
      [](const TSharedPtr<FJsonObject> &Present) {
        const TSharedPtr<FJsonObject> *Parts = nullptr;
        return Present->TryGetObjectField(ManifestPartsFieldName(), Parts)
                   ? func::mbind(
                         func::from_nullable(Parts),
                         [](const TSharedPtr<FJsonObject> &PartObject) {
                           return func::from_shared(PartObject);
                         })
                   : func::nothing<TSharedPtr<FJsonObject>>();
      });
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

/** User Story: As a data json manifest consumer, I need to invoke merge json objects through a stable signature so the data json manifest workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> MergeJsonObjects( const TSharedPtr<FJsonObject> &Left, const TSharedPtr<FJsonObject> &Right) */
TSharedPtr<FJsonObject> MergeJsonObjects(
    const TSharedPtr<FJsonObject> &Left,
    const TSharedPtr<FJsonObject> &Right) {
  TSharedPtr<FJsonObject> Merged = MakeShared<FJsonObject>();
  Merged->Values = func::merge_maps_right(Left->Values, Right->Values);
  return Merged;
}

/** User Story: As a data json manifest consumer, I need to invoke set json object field through a stable signature so the data json manifest workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> SetJsonObjectField( const FSetJsonObjectFieldRequest &Request) */
TSharedPtr<FJsonObject> SetJsonObjectField(
    const FSetJsonObjectFieldRequest &Request) {
  TSharedPtr<FJsonObject> Next = MakeShared<FJsonObject>();
  Next->Values = Request.Object->Values;
  Next->SetField(Request.FieldName, Request.Value);
  return Next;
}

/** User Story: As a data json manifest consumer, I need to invoke load manifest field value through a stable signature so the data json manifest workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonValue>> LoadManifestFieldValue(const FString &ParentRelativePath, const FString &PartRelativePath) */
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

/** User Story: As a data json manifest consumer, I need to invoke load object manifest merge parts through a stable signature so the data json manifest workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadObjectManifestMergeParts(const FString &RelativePath, const TSharedPtr<FJsonObject> &ManifestParts) */
TSharedPtr<FJsonObject>
LoadObjectManifestMergeParts(const FString &RelativePath,
                             const TSharedPtr<FJsonObject> &ManifestParts) {
  const TArray<TSharedPtr<FJsonValue>> *MergeParts = nullptr;
  const TSharedPtr<FJsonObject> Empty = MakeShared<FJsonObject>();
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Parts =
      ManifestParts->TryGetArrayField(ManifestMergeFieldName(), MergeParts)
          ? func::from_nullable(MergeParts)
          : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
  return func::match(
      Parts,
      [&RelativePath, Empty](
          const TArray<TSharedPtr<FJsonValue>> &Values) {
        return func::fold_indexed(
            Values, static_cast<size_t>(Values.Num()), Empty,
            [&RelativePath](const TSharedPtr<FJsonObject> &Current,
                            const TSharedPtr<FJsonValue> &Part) {
              const func::Maybe<TSharedPtr<FJsonObject>> Object =
                  LoadObjectFromContent(
                      {ResolvePartPath(RelativePath, Part)});
              check(Object.hasValue);
              return MergeJsonObjects(Current, Object.value);
            });
      },
      []() { return MakeShared<FJsonObject>(); });
}

/** User Story: As a data json manifest consumer, I need to invoke load object manifest field part through a stable signature so the data json manifest workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadObjectManifestFieldPart(const FFieldPartRequest &Request) */
TSharedPtr<FJsonObject>
LoadObjectManifestFieldPart(const FFieldPartRequest &Request) {
  const TSharedPtr<FJsonValue> PartValue =
      Request.ManifestParts->TryGetField(Request.PartKey);
  const func::Maybe<TSharedPtr<FJsonValue>> Present =
      func::from_shared(PartValue);
  check(Present.hasValue);
  const func::Maybe<TSharedPtr<FJsonValue>> Value =
      LoadManifestFieldValue(Request.RelativePath,
                             Present.value->AsString());
  check(Value.hasValue);
  return SetJsonObjectField({Request.Current, Request.PartKey, Value.value});
}

/** User Story: As a data json manifest consumer, I need to invoke load object manifest field parts through a stable signature so the data json manifest workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadObjectManifestFieldParts(const FString &RelativePath, const TSharedPtr<FJsonObject> &ManifestParts) */
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

/** User Story: As a data json manifest consumer, I need to invoke load object manifest parts through a stable signature so the data json manifest workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> LoadObjectManifestParts(const FString &RelativePath, const TSharedPtr<FJsonObject> &Manifest) */
func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectManifestParts(const FString &RelativePath,
                        const TSharedPtr<FJsonObject> &Manifest) {
  const TSharedPtr<FJsonObject> Empty = MakeShared<FJsonObject>();
  return func::match(
      ReadPartValues(Manifest),
      [&RelativePath, Empty](
          const TArray<TSharedPtr<FJsonValue>> &Parts) {
        return func::just<TSharedPtr<FJsonObject>>(func::fold_indexed(
            Parts, static_cast<size_t>(Parts.Num()), Empty,
            [&RelativePath](const TSharedPtr<FJsonObject> &Current,
                            const TSharedPtr<FJsonValue> &Part) {
              const func::Maybe<TSharedPtr<FJsonObject>> Object =
                  LoadObjectFromContent(
                      {ResolvePartPath(RelativePath, Part)});
              check(Object.hasValue);
              return MergeJsonObjects(Current, Object.value);
            }));
      },
      [&RelativePath, &Manifest]() {
        return func::fmap(
            ReadPartObject(Manifest),
            [&RelativePath](const TSharedPtr<FJsonObject> &PartObject) {
              return LoadObjectManifestFieldParts(RelativePath, PartObject);
            });
      });
}

/** User Story: As a data json manifest consumer, I need to invoke load array manifest parts through a stable signature so the data json manifest workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> LoadArrayManifestParts(const FString &RelativePath, const TSharedPtr<FJsonObject> &Manifest) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
LoadArrayManifestParts(const FString &RelativePath,
                       const TSharedPtr<FJsonObject> &Manifest) {
  return func::fmap(
      ReadPartValues(Manifest),
      [&RelativePath](const TArray<TSharedPtr<FJsonValue>> &Parts) {
        return func::fold_indexed(
            Parts, static_cast<size_t>(Parts.Num()),
            TArray<TSharedPtr<FJsonValue>>(),
            [&RelativePath](
                const TArray<TSharedPtr<FJsonValue>> &Current,
                const TSharedPtr<FJsonValue> &Part) {
              const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Values =
                  LoadArrayFromContent(
                      {ResolvePartPath(RelativePath, Part)});
              check(Values.hasValue);
              return func::append_values(Current, Values.value);
            });
      });
}

} // namespace
} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
