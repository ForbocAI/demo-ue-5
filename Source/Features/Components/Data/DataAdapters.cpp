#include "Features/Components/Data/DataAdapters.h"

#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace DataAdapters {

func::Maybe<TSharedPtr<FJsonObject>>
LoadObjectFromContent(const FJsonContentObjectRequest &Request) {
  const FString SourcePath = FPaths::ProjectContentDir() / Request.RelativePath;
  FString Source;
  if (!FFileHelper::LoadFileToString(Source, *SourcePath)) {
    UE_LOG(LogTemp, Error, TEXT("Data JSON missing: %s"), *SourcePath);
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  TSharedPtr<FJsonObject> Root;
  const TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(Source);
  if (!FJsonSerializer::Deserialize(Reader, Root) || !Root.IsValid()) {
    UE_LOG(LogTemp, Error, TEXT("Data JSON invalid: %s"), *SourcePath);
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  return func::just(Root);
}

FString ReadString(const FJsonFieldRequest &Request) {
  return Request.Object.IsValid() ? Request.Object->GetStringField(Request.FieldName)
                                  : FString();
}

float ReadFloat(const FJsonFieldRequest &Request) {
  return Request.Object.IsValid()
             ? static_cast<float>(Request.Object->GetNumberField(Request.FieldName))
             : 0.0f;
}

bool ReadBool(const FJsonFieldRequest &Request) {
  return Request.Object.IsValid() &&
         Request.Object->GetBoolField(Request.FieldName);
}

TArray<TSharedPtr<FJsonValue>> ReadArray(const FJsonFieldRequest &Request) {
  return Request.Object.IsValid()
             ? Request.Object->GetArrayField(Request.FieldName)
             : TArray<TSharedPtr<FJsonValue>>();
}

func::Maybe<TSharedPtr<FJsonObject>>
ReadObject(const FJsonFieldRequest &Request) {
  if (!Request.Object.IsValid()) {
    return func::nothing<TSharedPtr<FJsonObject>>();
  }

  const TSharedPtr<FJsonObject> Child =
      Request.Object->GetObjectField(Request.FieldName);
  return Child.IsValid() ? func::just(Child)
                         : func::nothing<TSharedPtr<FJsonObject>>();
}

} // namespace DataAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
