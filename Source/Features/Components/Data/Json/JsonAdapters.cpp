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
 * @brief Resolves a content-relative JSON path into a project content path.
 * @signature FString ResolveContentPath(const FContentObjectRequest &Request)
 *
 * User story: As a data adapter author, content path assembly should be one
 * neutral function below every authored JSON domain.
 */
FString ResolveContentPath(const FString &RelativePath) {
  return FPaths::ProjectContentDir() / RelativePath;
}

FString ResolveContentPath(const FContentObjectRequest &Request) {
  return ResolveContentPath(Request.RelativePath);
}

FString ResolveContentPath(const FContentArrayRequest &Request) {
  return ResolveContentPath(Request.RelativePath);
}

/**
 * @brief Reads a text file as a Maybe string.
 * @signature func::Maybe<FString> ReadSourceFile(const FString &SourcePath)
 *
 * User story: As a JSON adapter, file IO should become an explicit Maybe value
 * so parse execution composes through functional helpers.
 */
func::Maybe<FString> ReadSourceFile(const FString &SourcePath) {
  FString Source;
  return FFileHelper::LoadFileToString(Source, *SourcePath)
             ? func::just(Source)
             : func::nothing<FString>();
}

/**
 * @brief Parses a JSON object from one source string.
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ParseObjectSource(const FString &Source, const FString &SourcePath)
 *
 * User story: As a JSON adapter, parse failures should remain explicit and not
 * become default objects in feature domains.
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

FFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name) {
  return {Object, FString(Name)};
}

FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object) {
  return [Object](const TCHAR *Name) { return Field(Object, Name); };
}

TSharedPtr<FJsonObject> ReadObjectValue(const FFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value = ReadObject(Request);
  check(Value.hasValue);
  return Value.value;
}

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

TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FContentObjectRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Root = LoadObjectFromContent(Request);
  check(Root.hasValue);
  return Root.value;
}

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

TArray<TSharedPtr<FJsonValue>>
LoadRequiredArrayFromContent(const FContentArrayRequest &Request) {
  const func::Maybe<TArray<TSharedPtr<FJsonValue>>> Root =
      LoadArrayFromContent(Request);
  check(Root.hasValue);
  return Root.value;
}

FString ReadString(const FFieldRequest &Request) {
  FString Value;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetStringField(Request.FieldName, Value);
  check(bRead);
  return Value;
}

float ReadFloat(const FFieldRequest &Request) {
  double Value = 0.0;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetNumberField(Request.FieldName, Value);
  check(bRead);
  return static_cast<float>(Value);
}

int32 ReadInt(const FFieldRequest &Request) {
  return static_cast<int32>(ReadFloat(Request));
}

bool ReadBool(const FFieldRequest &Request) {
  bool Value = false;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetBoolField(Request.FieldName, Value);
  check(bRead);
  return Value;
}

TArray<TSharedPtr<FJsonValue>> ReadArray(const FFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  const bool bRead =
      Request.Object.IsValid() &&
      Request.Object->TryGetArrayField(Request.FieldName, Values) &&
      Values != nullptr;
  check(bRead);
  return *Values;
}

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

FVector ReadVector(const FFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FVector(ReadFloat(Field(Object, TEXT("x"))),
                 ReadFloat(Field(Object, TEXT("y"))),
                 ReadFloat(Field(Object, TEXT("z"))));
}

FRotator ReadRotator(const FFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FRotator(ReadFloat(Field(Object, TEXT("pitch"))),
                  ReadFloat(Field(Object, TEXT("yaw"))),
                  ReadFloat(Field(Object, TEXT("roll"))));
}

FJsonStringReader StringIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadString(FieldFor(Name)); };
}

FJsonFloatReader FloatIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadFloat(FieldFor(Name)); };
}

FJsonIntReader IntIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadInt(FieldFor(Name)); };
}

FJsonBoolReader BoolIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadBool(FieldFor(Name)); };
}

FJsonArrayReader ArrayIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadArray(FieldFor(Name)); };
}

FJsonObjectReader ObjectIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadObject(FieldFor(Name)); };
}

FJsonObjectValueReader ObjectValueIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) {
    return ReadObjectValue(FieldFor(Name));
  };
}

FJsonVectorReader VectorIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadVector(FieldFor(Name)); };
}

FJsonRotatorReader RotatorIn(const TSharedPtr<FJsonObject> &Object) {
  const FJsonFieldFactory FieldFor = FieldIn(Object);
  return [FieldFor](const TCHAR *Name) { return ReadRotator(FieldFor(Name)); };
}

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
