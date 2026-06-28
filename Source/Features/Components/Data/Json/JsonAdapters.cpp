#include "Features/Components/Data/Json/JsonAdapters.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonAdapters {
namespace {

/**
 * @brief Resolves a content-relative JSON path into a project content path.
 * @signature FString ResolveContentPath(const FJsonContentObjectRequest &Request)
 *
 * User story: As a data adapter author, content path assembly should be one
 * neutral function below every authored JSON domain.
 */
FString ResolveContentPath(const FJsonContentObjectRequest &Request) {
  return FPaths::ProjectContentDir() / Request.RelativePath;
}

/**
 * @brief Logs a missing JSON source file.
 * @signature void LogMissingSource(const FString &SourcePath)
 *
 * User story: As runtime data loading code, missing authored files should be
 * reported at the neutral JSON boundary before RTK actions are dispatched.
 */
void LogMissingSource(const FString &SourcePath) {
  UE_LOG(LogTemp, Error, TEXT("Data JSON missing: %s"), *SourcePath);
}

/**
 * @brief Logs an invalid JSON source file.
 * @signature void LogInvalidSource(const FString &SourcePath)
 *
 * User story: As runtime data loading code, invalid authored files should be
 * reported at the neutral JSON boundary before reducers receive payloads.
 */
void LogInvalidSource(const FString &SourcePath) {
  UE_LOG(LogTemp, Error, TEXT("Data JSON invalid: %s"), *SourcePath);
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
             : (LogMissingSource(SourcePath), func::nothing<FString>());
}

/**
 * @brief Parses a JSON object from one source string.
 * @signature func::Maybe<TSharedPtr<FJsonObject>> ParseObjectSource(const FString &Source, const FString &SourcePath)
 *
 * User story: As a JSON adapter, parse failures should remain explicit and not
 * become default objects in feature domains.
 */
func::Maybe<TSharedPtr<FJsonObject>>
ParseObjectSource(const FString &Source, const FString &SourcePath) {
  TSharedPtr<FJsonObject> Root;
  const TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(Source);
  return FJsonSerializer::Deserialize(Reader, Root) && Root.IsValid()
             ? func::just(Root)
             : (LogInvalidSource(SourcePath),
                func::nothing<TSharedPtr<FJsonObject>>());
}

} // namespace

FJsonFieldRequest Field(const TSharedPtr<FJsonObject> &Object,
                        const TCHAR *Name) {
  return {Object, FString(Name)};
}

FJsonFieldFactory FieldIn(const TSharedPtr<FJsonObject> &Object) {
  return [Object](const TCHAR *Name) { return Field(Object, Name); };
}

TSharedPtr<FJsonObject> ReadObjectValue(const FJsonFieldRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Value = ReadObject(Request);
  checkf(Value.hasValue, TEXT("Required JSON object missing: %s"),
         *Request.FieldName);
  return Value.value;
}

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request) {
  const FString SourcePath = ResolveContentPath(Request);
  return func::mbind(ReadSourceFile(SourcePath),
                     [&SourcePath](const FString &Source) {
                       return ParseObjectSource(Source, SourcePath);
                     });
}

TSharedPtr<FJsonObject>
LoadRequiredObjectFromContent(const FJsonContentObjectRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Root = LoadObjectFromContent(Request);
  checkf(Root.hasValue, TEXT("Required Data JSON missing or invalid: %s"),
         *Request.RelativePath);
  return Root.value;
}

FString ReadString(const FJsonFieldRequest &Request) {
  FString Value;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetStringField(Request.FieldName, Value);
  checkf(bRead, TEXT("Required JSON string missing: %s"),
         *Request.FieldName);
  return Value;
}

float ReadFloat(const FJsonFieldRequest &Request) {
  double Value = 0.0;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetNumberField(Request.FieldName, Value);
  checkf(bRead, TEXT("Required JSON number missing: %s"),
         *Request.FieldName);
  return static_cast<float>(Value);
}

int32 ReadInt(const FJsonFieldRequest &Request) {
  return static_cast<int32>(ReadFloat(Request));
}

bool ReadBool(const FJsonFieldRequest &Request) {
  bool Value = false;
  const bool bRead = Request.Object.IsValid() &&
                     Request.Object->TryGetBoolField(Request.FieldName, Value);
  checkf(bRead, TEXT("Required JSON bool missing: %s"), *Request.FieldName);
  return Value;
}

TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request) {
  const TArray<TSharedPtr<FJsonValue>> *Values = nullptr;
  const bool bRead =
      Request.Object.IsValid() &&
      Request.Object->TryGetArrayField(Request.FieldName, Values) &&
      Values != nullptr;
  checkf(bRead, TEXT("Required JSON array missing: %s"), *Request.FieldName);
  return *Values;
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request) {
  const TSharedPtr<FJsonObject> *Child = nullptr;
  const bool bHasChild =
      Request.Object.IsValid() &&
      Request.Object->TryGetObjectField(Request.FieldName, Child);
  return bHasChild && Child != nullptr && Child->IsValid()
             ? func::just(*Child)
             : func::nothing<TSharedPtr<FJsonObject>>();
}

FVector ReadVector(const FJsonFieldRequest &Request) {
  const TSharedPtr<FJsonObject> Object = ReadObjectValue(Request);
  return FVector(ReadFloat(Field(Object, TEXT("x"))),
                 ReadFloat(Field(Object, TEXT("y"))),
                 ReadFloat(Field(Object, TEXT("z"))));
}

FRotator ReadRotator(const FJsonFieldRequest &Request) {
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
} // namespace Demo
} // namespace ForbocAI
