#include "Blueprints/GenericStateBlueprintLibrary.h"

TSharedPtr<FJsonObject>
UGenericStateBlueprintLibrary::ParseJson(const FString &JsonData) {
  TSharedPtr<FJsonObject> JsonObject;
  TSharedRef<TJsonReader<>> Reader =
      TJsonReaderFactory<>::Create(JsonData);
  FJsonSerializer::Deserialize(Reader, JsonObject);
  return JsonObject;
}

FString UGenericStateBlueprintLibrary::SerializeJson(
    const TSharedPtr<FJsonObject> &JsonObject) {
  FString Output;
  return !JsonObject.IsValid()
             ? FString(TEXT("{}"))
             : (TSharedRef<TJsonWriter<>> Writer =
                    TJsonWriterFactory<>::Create(&Output),
                FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer),
                Output);
}

// ── Read Operations ──

FString UGenericStateBlueprintLibrary::GetStateString(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  return (!Json.IsValid() || !Json->HasField(FieldName))
             ? FString()
             : Json->GetStringField(FieldName);
}

float UGenericStateBlueprintLibrary::GetStateFloat(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  return (!Json.IsValid() || !Json->HasField(FieldName))
             ? 0.0f
             : Json->GetNumberField(FieldName);
}

int32 UGenericStateBlueprintLibrary::GetStateInt(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  return (!Json.IsValid() || !Json->HasField(FieldName))
             ? 0
             : (int32)Json->GetNumberField(FieldName);
}

bool UGenericStateBlueprintLibrary::GetStateBool(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  return (!Json.IsValid() || !Json->HasField(FieldName))
             ? false
             : Json->GetBoolField(FieldName);
}

bool UGenericStateBlueprintLibrary::HasStateField(
    const FString &JsonData, const FString &FieldName) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  return Json.IsValid() && Json->HasField(FieldName);
}

// ── Write Operations ──

FString UGenericStateBlueprintLibrary::SetStateString(
    const FString &JsonData, const FString &FieldName,
    const FString &Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  TSharedPtr<FJsonObject> Target =
      Json.IsValid() ? Json : MakeShared<FJsonObject>();
  Target->SetStringField(FieldName, Value);
  return SerializeJson(Target);
}

FString UGenericStateBlueprintLibrary::SetStateFloat(
    const FString &JsonData, const FString &FieldName, float Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  TSharedPtr<FJsonObject> Target =
      Json.IsValid() ? Json : MakeShared<FJsonObject>();
  Target->SetNumberField(FieldName, Value);
  return SerializeJson(Target);
}

FString UGenericStateBlueprintLibrary::SetStateInt(
    const FString &JsonData, const FString &FieldName, int32 Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  TSharedPtr<FJsonObject> Target =
      Json.IsValid() ? Json : MakeShared<FJsonObject>();
  Target->SetNumberField(FieldName, (double)Value);
  return SerializeJson(Target);
}

FString UGenericStateBlueprintLibrary::SetStateBool(
    const FString &JsonData, const FString &FieldName, bool Value) {
  TSharedPtr<FJsonObject> Json = ParseJson(JsonData);
  TSharedPtr<FJsonObject> Target =
      Json.IsValid() ? Json : MakeShared<FJsonObject>();
  Target->SetBoolField(FieldName, Value);
  return SerializeJson(Target);
}

// ── Utility ──

FString UGenericStateBlueprintLibrary::MergeState(
    const FString &BaseJson, const FString &OverrideJson) {
  TSharedPtr<FJsonObject> Base = ParseJson(BaseJson);
  TSharedPtr<FJsonObject> Override = ParseJson(OverrideJson);

  TSharedPtr<FJsonObject> Merged =
      Base.IsValid() ? Base : MakeShared<FJsonObject>();

  return !Override.IsValid()
             ? SerializeJson(Merged)
             : [&]() -> FString {
    // Recursively merge override fields into base
    const auto MergeFieldsRecursive =
        [&Merged, &Override](const TArray<FString> &Keys, int32 Idx,
                              const auto &Self) -> void {
      return Idx >= Keys.Num()
                 ? void()
                 : (Merged->SetField(Keys[Idx],
                                      Override->TryGetField(Keys[Idx])),
                    Self(Keys, Idx + 1, Self));
    };

    TArray<FString> OverrideKeys;
    Override->Values.GetKeys(OverrideKeys);
    MergeFieldsRecursive(OverrideKeys, 0, MergeFieldsRecursive);

    return SerializeJson(Merged);
  }();
}

FString UGenericStateBlueprintLibrary::EmptyState() {
  return TEXT("{}");
}
