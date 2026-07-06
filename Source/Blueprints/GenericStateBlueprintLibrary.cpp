#include "Blueprints/GenericStateBlueprintLibrary.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/Data/Json/JsonAdapters.h"

namespace {

namespace Json = ForbocAI::Game::Data::JsonAdapters;
using FJsonFieldRequest = ForbocAI::Game::Data::FJsonFieldRequest;

template <typename Value> struct TStateJsonFieldDeclaration {
  TFunction<Value(const FJsonFieldRequest &)> Read;
  TFunction<void(const FJsonFieldRequest &, const Value &)> Write;
};

template <typename Value> struct TStateJsonFieldRegistry;

struct FStateJsonMergeRequest {
  TSharedPtr<FJsonObject> Target;
  TSharedPtr<FJsonObject> Source;
};

bool HasJsonField(const FJsonFieldRequest &Request);
void WriteStringJsonField(const FJsonFieldRequest &Request,
                          const FString &Value);
void WriteFloatJsonField(const FJsonFieldRequest &Request,
                         const float &Value);
void WriteIntJsonField(const FJsonFieldRequest &Request, const int32 &Value);
void WriteBoolJsonField(const FJsonFieldRequest &Request, const bool &Value);

template <> struct TStateJsonFieldRegistry<FString> {
  static const TStateJsonFieldDeclaration<FString> &Declaration() {
    static const TStateJsonFieldDeclaration<FString> RegisteredDeclaration = {
        Json::ReadString, WriteStringJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<float> {
  static const TStateJsonFieldDeclaration<float> &Declaration() {
    static const TStateJsonFieldDeclaration<float> RegisteredDeclaration = {
        Json::ReadFloat, WriteFloatJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<int32> {
  static const TStateJsonFieldDeclaration<int32> &Declaration() {
    static const TStateJsonFieldDeclaration<int32> RegisteredDeclaration = {
        Json::ReadInt, WriteIntJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<bool> {
  static const TStateJsonFieldDeclaration<bool> &Declaration() {
    static const TStateJsonFieldDeclaration<bool> RegisteredDeclaration = {
        Json::ReadBool, WriteBoolJsonField};
    return RegisteredDeclaration;
  }
};

TSharedPtr<FJsonObject> ParseStateJson(const FString &JsonData) {
  TSharedPtr<FJsonObject> JsonObject;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonData);
  const bool bParsed = FJsonSerializer::Deserialize(Reader, JsonObject);
  check(bParsed);
  check(JsonObject.IsValid());
  return JsonObject;
}

FString SerializeStateJson(const TSharedPtr<FJsonObject> &JsonObject) {
  FString Output;
  check(JsonObject.IsValid());

  TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
  FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
  return Output;
}

FJsonFieldRequest StateJsonField(const FString &JsonData,
                                 const FString &FieldName) {
  return {ParseStateJson(JsonData), FieldName};
}

template <typename Value>
Value ReadStateJsonField(const FString &JsonData,
                         const FString &FieldName) {
  return TStateJsonFieldRegistry<Value>::Declaration().Read(
      StateJsonField(JsonData, FieldName));
}

template <typename Value>
FString WriteStateJsonField(const FString &JsonData,
                            const FString &FieldName,
                            const Value &ValueData) {
  const TSharedPtr<FJsonObject> Json = ParseStateJson(JsonData);
  TStateJsonFieldRegistry<Value>::Declaration().Write({Json, FieldName},
                                                      ValueData);
  return SerializeStateJson(Json);
}

bool HasStateJsonField(const FString &JsonData, const FString &FieldName) {
  return HasJsonField(StateJsonField(JsonData, FieldName));
}

bool HasJsonField(const FJsonFieldRequest &Request) {
  check(Request.Object.IsValid());
  return Request.Object->HasField(Request.FieldName);
}

void WriteStringJsonField(const FJsonFieldRequest &Request,
                          const FString &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetStringField(Request.FieldName, Value);
}

void WriteFloatJsonField(const FJsonFieldRequest &Request,
                         const float &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetNumberField(Request.FieldName, Value);
}

void WriteIntJsonField(const FJsonFieldRequest &Request,
                       const int32 &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetNumberField(Request.FieldName, static_cast<double>(Value));
}

void WriteBoolJsonField(const FJsonFieldRequest &Request,
                        const bool &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetBoolField(Request.FieldName, Value);
}

TArray<FString> JsonFieldNames(const TSharedPtr<FJsonObject> &JsonObject) {
  check(JsonObject.IsValid());
  return func::map_keys<FString, TSharedPtr<FJsonValue>>(JsonObject->Values);
}

void CopyJsonField(const FStateJsonMergeRequest &Request,
                   const FString &FieldName) {
  Request.Target->SetField(FieldName, Request.Source->TryGetField(FieldName));
}

TSharedPtr<FJsonObject> MergeJsonObjects(
    const FStateJsonMergeRequest &Request) {
  check(Request.Target.IsValid());
  check(Request.Source.IsValid());
  func::for_each_array<FString>(
      JsonFieldNames(Request.Source),
      [Request](const FString &FieldName) {
        CopyJsonField(Request, FieldName);
      });
  return Request.Target;
}

FString MergeStateJson(const FString &BaseJson, const FString &OverrideJson) {
  return SerializeStateJson(
      MergeJsonObjects({ParseStateJson(BaseJson), ParseStateJson(OverrideJson)}));
}

} // namespace

TSharedPtr<FJsonObject>
UGenericStateBlueprintLibrary::ParseJson(const FString &JsonData) {
  return ParseStateJson(JsonData);
}

FString UGenericStateBlueprintLibrary::SerializeJson(
    const TSharedPtr<FJsonObject> &JsonObject) {
  return SerializeStateJson(JsonObject);
}

// ── Read Operations ──

FString UGenericStateBlueprintLibrary::GetStateString(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<FString>(JsonData, FieldName);
}

float UGenericStateBlueprintLibrary::GetStateFloat(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<float>(JsonData, FieldName);
}

int32 UGenericStateBlueprintLibrary::GetStateInt(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<int32>(JsonData, FieldName);
}

bool UGenericStateBlueprintLibrary::GetStateBool(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<bool>(JsonData, FieldName);
}

bool UGenericStateBlueprintLibrary::HasStateField(
    const FString &JsonData, const FString &FieldName) {
  return HasStateJsonField(JsonData, FieldName);
}

// ── Write Operations ──

FString UGenericStateBlueprintLibrary::SetStateString(
    const FString &JsonData, const FString &FieldName,
    const FString &Value) {
  return WriteStateJsonField<FString>(JsonData, FieldName, Value);
}

FString UGenericStateBlueprintLibrary::SetStateFloat(
    const FString &JsonData, const FString &FieldName, float Value) {
  return WriteStateJsonField<float>(JsonData, FieldName, Value);
}

FString UGenericStateBlueprintLibrary::SetStateInt(
    const FString &JsonData, const FString &FieldName, int32 Value) {
  return WriteStateJsonField<int32>(JsonData, FieldName, Value);
}

FString UGenericStateBlueprintLibrary::SetStateBool(
    const FString &JsonData, const FString &FieldName, bool Value) {
  return WriteStateJsonField<bool>(JsonData, FieldName, Value);
}

// ── Utility ──

FString UGenericStateBlueprintLibrary::MergeState(
    const FString &BaseJson, const FString &OverrideJson) {
  return MergeStateJson(BaseJson, OverrideJson);
}
