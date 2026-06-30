#include "Blueprints/GenericStateBlueprintLibrary.h"

#include "Core/functional_core.hpp"

TSharedPtr<FJsonObject>
UGenericStateBlueprintLibrary::ParseJson(const FString &JsonData) {
  TSharedPtr<FJsonObject> JsonObject;
  TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(JsonData);
  const bool bParsed = FJsonSerializer::Deserialize(Reader, JsonObject);
  check(bParsed);
  check(JsonObject.IsValid());
  return JsonObject;
}

FString UGenericStateBlueprintLibrary::SerializeJson(
    const TSharedPtr<FJsonObject> &JsonObject) {
  FString Output;
  check(JsonObject.IsValid());

  TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
  FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
  return Output;
}

// ── Read Operations ──

FString UGenericStateBlueprintLibrary::GetStateString(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  check(Json->HasField(FieldName));
  return Json->GetStringField(FieldName);
}

float UGenericStateBlueprintLibrary::GetStateFloat(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  check(Json->HasField(FieldName));
  return Json->GetNumberField(FieldName);
}

int32 UGenericStateBlueprintLibrary::GetStateInt(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  check(Json->HasField(FieldName));
  return (int32)Json->GetNumberField(FieldName);
}

bool UGenericStateBlueprintLibrary::GetStateBool(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  check(Json->HasField(FieldName));
  return Json->GetBoolField(FieldName);
}

bool UGenericStateBlueprintLibrary::HasStateField(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  return Json->HasField(FieldName);
}

// ── Write Operations ──

FString UGenericStateBlueprintLibrary::SetStateString(
    const FString &JsonData, const FString &FieldName,
    const FString &Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  Json->SetStringField(FieldName, Value);
  return SerializeJson(Json);
}

FString UGenericStateBlueprintLibrary::SetStateFloat(
    const FString &JsonData, const FString &FieldName, float Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  Json->SetNumberField(FieldName, Value);
  return SerializeJson(Json);
}

FString UGenericStateBlueprintLibrary::SetStateInt(
    const FString &JsonData, const FString &FieldName, int32 Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  Json->SetNumberField(FieldName, (double)Value);
  return SerializeJson(Json);
}

FString UGenericStateBlueprintLibrary::SetStateBool(
    const FString &JsonData, const FString &FieldName, bool Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  check(Json.IsValid());
  Json->SetBoolField(FieldName, Value);
  return SerializeJson(Json);
}

// ── Utility ──

FString UGenericStateBlueprintLibrary::MergeState(
    const FString &BaseJson, const FString &OverrideJson) {
  TSharedPtr<FJsonObject> Base = ParseJson(BaseJson);
  TSharedPtr<FJsonObject> Override = ParseJson(OverrideJson);
  check(Base.IsValid());
  check(Override.IsValid());

  TArray<FString> OverrideKeys;
  Override->Values.GetKeys(OverrideKeys);
  func::for_each_indexed(
      OverrideKeys, static_cast<size_t>(OverrideKeys.Num()),
      [&Base, &Override](const FString &Key) {
        Base->SetField(Key, Override->TryGetField(Key));
      });
  return SerializeJson(Base);
}
