#include "Features/Components/Data/Json/Read/ReadAdapters.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Features/Components/Data/Json/Manifest/ManifestAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {
namespace {

/**
 * @fn FString ResolveContentPath(const FString &RelativePath)
 * @brief Resolves a content-relative JSON path into a project content path.
 *
 * User story: As a data adapter author, content path assembly should be one
 * neutral function below every authored JSON domain.
 * User Story: As a data json read consumer, I need to invoke resolve content path through a stable signature so the data json read workflow remains explicit and composable.
 */
FString ResolveContentPath(const FString &RelativePath) {
  return FPaths::ProjectContentDir() / RelativePath;
}

/** User Story: As a data json read consumer, I need to invoke resolve content path through a stable signature so the data json read workflow remains explicit and composable. @fn FString ResolveContentPath(const FContentObjectRequest &Request) */
FString ResolveContentPath(const FContentObjectRequest &Request) {
  return ResolveContentPath(Request.RelativePath);
}

/** User Story: As a data json read consumer, I need to invoke resolve content path through a stable signature so the data json read workflow remains explicit and composable. @fn FString ResolveContentPath(const FContentArrayRequest &Request) */
FString ResolveContentPath(const FContentArrayRequest &Request) {
  return ResolveContentPath(Request.RelativePath);
}

/**
 * @fn func::Maybe<FString> ReadSourceFile(const FString &SourcePath)
 * @brief Reads a text file as a Maybe string.
 *
 * User story: As a JSON adapter, file IO should become an explicit Maybe value
 * so parse execution composes through functional helpers.
 * User Story: As a data json read consumer, I need to invoke read source file through a stable signature so the data json read workflow remains explicit and composable.
 */
func::Maybe<FString> ReadSourceFile(const FString &SourcePath) {
  FString Source;
  return FFileHelper::LoadFileToString(Source, *SourcePath)
             ? func::just(Source)
             : func::nothing<FString>();
}

/**
 * @fn func::Maybe<TSharedPtr<FJsonObject>> ParseObjectSource(const FString &Source)
 * @brief Parses a JSON object from one source string.
 *
 * User story: As a JSON adapter, parse failures should remain explicit and not
 * become default objects in feature domains.
 * User Story: As a data json read consumer, I need to invoke parse object source through a stable signature so the data json read workflow remains explicit and composable.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ParseObjectSource(const FString &Source) {
  TSharedPtr<FJsonObject> Root;
  const TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(Source);
  return FJsonSerializer::Deserialize(Reader, Root) && Root.IsValid()
             ? func::just(Root)
             : func::nothing<TSharedPtr<FJsonObject>>();
}

/** User Story: As a data json read consumer, I need to invoke parse array source through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> ParseArraySource(const FString &Source) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
ParseArraySource(const FString &Source) {
  TArray<TSharedPtr<FJsonValue>> Root;
  const TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(Source);
  return FJsonSerializer::Deserialize(Reader, Root)
             ? func::just(Root)
             : func::nothing<TArray<TSharedPtr<FJsonValue>>>();
}


} // namespace

/** User Story: As a data json read consumer, I need to invoke field through a stable signature so the data json read workflow remains explicit and composable. @fn FFieldRequest Field(const TSharedPtr<FJsonObject> &Object, const TCHAR *Name) */
FFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name) {
  return {Object, FString(Name)};
}

/** User Story: As a data json read consumer, I need to invoke field in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object) */
FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object) {
  return [Object](const TCHAR *Name) { return Field(Object, Name); };
}

/** User Story: As a data json read consumer, I need to invoke read object value through a stable signature so the data json read workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> ReadObjectValue(const FFieldRequest &Request) */
TSharedPtr<FJsonObject> ReadObjectValue(const FFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value = ReadObject(Request);
  check(Value.hasValue);
  return Value.value;
}

/** User Story: As a data json read consumer, I need to invoke load object from content through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> LoadObjectFromContent(const FContentObjectRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FContentObjectRequest &Request) {
  const FString SourcePath = ResolveContentPath(Request);
  return func::mbind(ReadSourceFile(SourcePath),
                     [&Request](const FString &Source) {
                       return func::mbind(
                           ParseObjectSource(Source),
                           [&Request](const TSharedPtr<FJsonObject> &Object) {
                             const func::Maybe<TSharedPtr<FJsonObject>> Parts =
                                 LoadObjectManifestParts(Request.RelativePath,
                                                         Object);
                             return Parts.hasValue ? Parts
                                                   : func::just(Object);
                           });
                     });
}

/** User Story: As a data json read consumer, I need to invoke load required object from content through a stable signature so the data json read workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadRequiredObjectFromContent(const FContentObjectRequest &Request) */
TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FContentObjectRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Root = LoadObjectFromContent(Request);
  check(Root.hasValue);
  return Root.value;
}

/** User Story: As a data json read consumer, I need to invoke load array from content through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TArray<TSharedPtr<FJsonValue>>> LoadArrayFromContent(const FContentArrayRequest &Request) */
func::Maybe<TArray<TSharedPtr<FJsonValue>>>
LoadArrayFromContent(const FContentArrayRequest &Request) {
  const FString SourcePath = ResolveContentPath(Request);
  return func::mbind(
      ReadSourceFile(SourcePath), [&Request](const FString &Source) {
        const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Array =
            ParseArraySource(Source);
        return Array.hasValue
                   ? Array
                   : func::mbind(
                         ParseObjectSource(Source),
                         [&Request](const TSharedPtr<FJsonObject> &Object) {
                           return LoadArrayManifestParts(Request.RelativePath,
                                                         Object);
                         });
      });
}

/** User Story: As a data json read consumer, I need to invoke load required array from content through a stable signature so the data json read workflow remains explicit and composable. @fn TArray<TSharedPtr<FJsonValue>> LoadRequiredArrayFromContent(const FContentArrayRequest &Request) */
TArray<TSharedPtr<FJsonValue>>
LoadRequiredArrayFromContent(const FContentArrayRequest &Request) {
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Root =
      LoadArrayFromContent(Request);
  check(Root.hasValue);
  return Root.value;
}

/** User Story: As a data json read consumer, I need to invoke read string through a stable signature so the data json read workflow remains explicit and composable. @fn FString ReadString(const FFieldRequest &Request) */
FString ReadString(const FFieldRequest &Request) {
  FString Value;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetStringField(Request.FieldName, Value);
  check(bRead);
  return Value;
}

/** User Story: As a data json read consumer, I need to invoke read float through a stable signature so the data json read workflow remains explicit and composable. @fn float ReadFloat(const FFieldRequest &Request) */
float ReadFloat(const FFieldRequest &Request) {
  double Value = 0.0;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetNumberField(Request.FieldName, Value);
  check(bRead);
  return static_cast<float>(Value);
}

/** User Story: As a data json read consumer, I need to invoke read int through a stable signature so the data json read workflow remains explicit and composable. @fn int32 ReadInt(const FFieldRequest &Request) */
int32 ReadInt(const FFieldRequest &Request) {
  return static_cast<int32>(ReadFloat(Request));
}

/** User Story: As a data json read consumer, I need to invoke read bool through a stable signature so the data json read workflow remains explicit and composable. @fn bool ReadBool(const FFieldRequest &Request) */
bool ReadBool(const FFieldRequest &Request) {
  bool Value = false;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetBoolField(Request.FieldName, Value);
  check(bRead);
  return Value;
}

/** User Story: As a data json read consumer, I need to invoke read array through a stable signature so the data json read workflow remains explicit and composable. @fn TArray<TSharedPtr<FJsonValue>> ReadArray(const FFieldRequest &Request) */
TArray<TSharedPtr<FJsonValue>> ReadArray(const FFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  const bool bRead =
      Request.Object.IsValid() &&
      Request.Object->TryGetArrayField(Request.FieldName, Values) &&
      Values != nullptr;
  check(bRead);
  return *Values;
}

/** User Story: As a data json read consumer, I need to invoke read object through a stable signature so the data json read workflow remains explicit and composable. @fn func::Maybe<TSharedPtr<FJsonObject>> ReadObject(const FFieldRequest &Request) */
func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FFieldRequest &Request) {
  const TSharedPtr<FJsonObject> *Child = nullptr;
  const bool bHasChild =
      Request.Object.IsValid() &&
      Request.Object->TryGetObjectField(Request.FieldName, Child);
  return bHasChild && Child != nullptr && Child->IsValid()
             ? func::just(*Child)
             : func::nothing<TSharedPtr<FJsonObject>>();
}

/** User Story: As a data json read consumer, I need to invoke read vector through a stable signature so the data json read workflow remains explicit and composable. @fn FVector ReadVector(const FFieldRequest &Request) */
FVector ReadVector(const FFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FVector(ReadFloat(Field(Object, TEXT("x"))),
                 ReadFloat(Field(Object, TEXT("y"))),
                 ReadFloat(Field(Object, TEXT("z"))));
}

/** User Story: As a data json read consumer, I need to invoke read rotator through a stable signature so the data json read workflow remains explicit and composable. @fn FRotator ReadRotator(const FFieldRequest &Request) */
FRotator ReadRotator(const FFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FRotator(ReadFloat(Field(Object, TEXT("pitch"))),
                  ReadFloat(Field(Object, TEXT("yaw"))),
                  ReadFloat(Field(Object, TEXT("roll"))));
}

/** User Story: As a data json read consumer, I need to invoke string in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object) */
FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadString(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke float in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object) */
FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadFloat(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke int in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object) */
FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadInt(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke bool in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object) */
FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadBool(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke array in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object) */
FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadArray(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke object in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object) */
FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadObject(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke object value in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object) */
FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) {
    return ReadObjectValue(FieldFor(Name));
  };
}

/** User Story: As a data json read consumer, I need to invoke vector in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object) */
FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadVector(FieldFor(Name)); };
}

/** User Story: As a data json read consumer, I need to invoke rotator in through a stable signature so the data json read workflow remains explicit and composable. @fn FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object) */
FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadRotator(FieldFor(Name)); };
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
