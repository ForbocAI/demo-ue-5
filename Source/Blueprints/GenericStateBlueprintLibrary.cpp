#include "Blueprints/GenericStateBlueprintLibrary.h"

#include "Algo/Transform.h"
#include "Core/fp.hpp"
#include "Features/Components/Data/Json/Read/ReadAdapters.h"

namespace {

namespace Json = ForbocAI::Game::Data::JsonAdapters;
using FFieldRequest = ForbocAI::Game::Data::FFieldRequest;

template <typename Value> struct TStateJsonFieldDeclaration {
  TFunction<Value(const FFieldRequest &)> Read;
  TFunction<void(const FFieldRequest &, const Value &)> Write;
};

template <typename Value> struct TStateJsonFieldRegistry;

struct FStateJsonMergeRequest {
  TSharedPtr<FJsonObject> Target;
  TSharedPtr<FJsonObject> Source;
};

/** User Story: As a blueprints consumer, I need to invoke has json field through a stable signature so the blueprints workflow remains explicit and composable. @fn bool HasJsonField(const FFieldRequest &Request) */
bool HasJsonField(const FFieldRequest &Request);
/** User Story: As a blueprints consumer, I need to invoke write string json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteStringJsonField(const FFieldRequest &Request, const FString &Value) */
void WriteStringJsonField(const FFieldRequest &Request,
                          const FString &Value);
/** User Story: As a blueprints consumer, I need to invoke write float json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteFloatJsonField(const FFieldRequest &Request, const float &Value) */
void WriteFloatJsonField(const FFieldRequest &Request,
                         const float &Value);
/** User Story: As a blueprints consumer, I need to invoke write int json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteIntJsonField(const FFieldRequest &Request, const int32 &Value) */
void WriteIntJsonField(const FFieldRequest &Request, const int32 &Value);
/** User Story: As a blueprints consumer, I need to invoke write bool json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteBoolJsonField(const FFieldRequest &Request, const bool &Value) */
void WriteBoolJsonField(const FFieldRequest &Request, const bool &Value);

template <> struct TStateJsonFieldRegistry<FString> {
  /** User Story: As a blueprints consumer, I need to invoke declaration through a stable signature so the blueprints workflow remains explicit and composable. @fn static const TStateJsonFieldDeclaration<FString> &Declaration() */
  static const TStateJsonFieldDeclaration<FString> &Declaration() {
    static const TStateJsonFieldDeclaration<FString> RegisteredDeclaration = {
        Json::ReadString, WriteStringJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<float> {
  /** User Story: As a blueprints consumer, I need to invoke declaration through a stable signature so the blueprints workflow remains explicit and composable. @fn static const TStateJsonFieldDeclaration<float> &Declaration() */
  static const TStateJsonFieldDeclaration<float> &Declaration() {
    static const TStateJsonFieldDeclaration<float> RegisteredDeclaration = {
        Json::ReadFloat, WriteFloatJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<int32> {
  /** User Story: As a blueprints consumer, I need to invoke declaration through a stable signature so the blueprints workflow remains explicit and composable. @fn static const TStateJsonFieldDeclaration<int32> &Declaration() */
  static const TStateJsonFieldDeclaration<int32> &Declaration() {
    static const TStateJsonFieldDeclaration<int32> RegisteredDeclaration = {
        Json::ReadInt, WriteIntJsonField};
    return RegisteredDeclaration;
  }
};

template <> struct TStateJsonFieldRegistry<bool> {
  /** User Story: As a blueprints consumer, I need to invoke declaration through a stable signature so the blueprints workflow remains explicit and composable. @fn static const TStateJsonFieldDeclaration<bool> &Declaration() */
  static const TStateJsonFieldDeclaration<bool> &Declaration() {
    static const TStateJsonFieldDeclaration<bool> RegisteredDeclaration = {
        Json::ReadBool, WriteBoolJsonField};
    return RegisteredDeclaration;
  }
};

/** User Story: As a blueprints consumer, I need to invoke parse state json through a stable signature so the blueprints workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> ParseStateJson(const FString &JsonData) */
TSharedPtr<FJsonObject> ParseStateJson(const FString &JsonData) {
  TSharedPtr<FJsonObject> JsonObject;
  TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonData);
  const bool bParsed = FJsonSerializer::Deserialize(Reader, JsonObject);
  check(bParsed);
  check(JsonObject.IsValid());
  return JsonObject;
}

/** User Story: As a blueprints consumer, I need to invoke serialize state json through a stable signature so the blueprints workflow remains explicit and composable. @fn FString SerializeStateJson(const TSharedPtr<FJsonObject> &JsonObject) */
FString SerializeStateJson(const TSharedPtr<FJsonObject> &JsonObject) {
  FString Output;
  check(JsonObject.IsValid());

  TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
  FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
  return Output;
}

/** User Story: As a blueprints consumer, I need to invoke state json field through a stable signature so the blueprints workflow remains explicit and composable. @fn FFieldRequest StateJsonField(const FString &JsonData, const FString &FieldName) */
FFieldRequest StateJsonField(const FString &JsonData,
                                 const FString &FieldName) {
  return {ParseStateJson(JsonData), FieldName};
}

/** User Story: As a blueprints consumer, I need to invoke state json field through a stable signature so the blueprints workflow remains explicit and composable. @fn FFieldRequest StateJsonField(const FGenericStateFieldRequest &Request) */
FFieldRequest StateJsonField(const FGenericStateFieldRequest &Request) {
  return StateJsonField(Request.JsonData, Request.FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke read state json field through a stable signature so the blueprints workflow remains explicit and composable. @fn template <typename Value> Value ReadStateJsonField(const FString &JsonData, const FString &FieldName) */
template <typename Value>
Value ReadStateJsonField(const FString &JsonData,
                         const FString &FieldName) {
  return TStateJsonFieldRegistry<Value>::Declaration().Read(
      StateJsonField(JsonData, FieldName));
}

/** User Story: As a blueprints consumer, I need to invoke write state json field through a stable signature so the blueprints workflow remains explicit and composable. @fn template <typename Value> FString WriteStateJsonField(const FGenericStateFieldRequest &Request, const Value &ValueData) */
template <typename Value>
FString WriteStateJsonField(const FGenericStateFieldRequest &Request,
                            const Value &ValueData) {
  const TSharedPtr<FJsonObject> Json = ParseStateJson(Request.JsonData);
  TStateJsonFieldRegistry<Value>::Declaration().Write({Json, Request.FieldName},
                                                      ValueData);
  return SerializeStateJson(Json);
}

/** User Story: As a blueprints consumer, I need to invoke has state json field through a stable signature so the blueprints workflow remains explicit and composable. @fn bool HasStateJsonField(const FString &JsonData, const FString &FieldName) */
bool HasStateJsonField(const FString &JsonData, const FString &FieldName) {
  return HasJsonField(StateJsonField(JsonData, FieldName));
}

/** User Story: As a blueprints consumer, I need to invoke has json field through a stable signature so the blueprints workflow remains explicit and composable. @fn bool HasJsonField(const FFieldRequest &Request) */
bool HasJsonField(const FFieldRequest &Request) {
  check(Request.Object.IsValid());
  return Request.Object->HasField(Request.FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke write string json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteStringJsonField(const FFieldRequest &Request, const FString &Value) */
void WriteStringJsonField(const FFieldRequest &Request,
                          const FString &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetStringField(Request.FieldName, Value);
}

/** User Story: As a blueprints consumer, I need to invoke write float json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteFloatJsonField(const FFieldRequest &Request, const float &Value) */
void WriteFloatJsonField(const FFieldRequest &Request,
                         const float &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetNumberField(Request.FieldName, Value);
}

/** User Story: As a blueprints consumer, I need to invoke write int json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteIntJsonField(const FFieldRequest &Request, const int32 &Value) */
void WriteIntJsonField(const FFieldRequest &Request,
                       const int32 &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetNumberField(Request.FieldName, static_cast<double>(Value));
}

/** User Story: As a blueprints consumer, I need to invoke write bool json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void WriteBoolJsonField(const FFieldRequest &Request, const bool &Value) */
void WriteBoolJsonField(const FFieldRequest &Request,
                        const bool &Value) {
  check(Request.Object.IsValid());
  Request.Object->SetBoolField(Request.FieldName, Value);
}

/** User Story: As a blueprints consumer, I need to invoke json field names through a stable signature so the blueprints workflow remains explicit and composable. @fn TArray<FString> JsonFieldNames(const TSharedPtr<FJsonObject> &JsonObject) */
TArray<FString> JsonFieldNames(const TSharedPtr<FJsonObject> &JsonObject) {
  check(JsonObject.IsValid());
  TArray<FString> FieldNames;
  FieldNames.Reserve(JsonObject->Values.Num());
  Algo::Transform(JsonObject->Values, FieldNames, [](const auto &Field) {
    return FString(Field.Key.Len(), *Field.Key);
  });
  return FieldNames;
}

/** User Story: As a blueprints consumer, I need to invoke copy json field through a stable signature so the blueprints workflow remains explicit and composable. @fn void CopyJsonField(const FStateJsonMergeRequest &Request, const FString &FieldName) */
void CopyJsonField(const FStateJsonMergeRequest &Request,
                   const FString &FieldName) {
  Request.Target->SetField(FieldName, Request.Source->TryGetField(FieldName));
}

/** User Story: As a blueprints consumer, I need to invoke merge json objects through a stable signature so the blueprints workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> MergeJsonObjects( const FStateJsonMergeRequest &Request) */
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

/** User Story: As a blueprints consumer, I need to invoke merge state json through a stable signature so the blueprints workflow remains explicit and composable. @fn FString MergeStateJson(const FString &BaseJson, const FString &OverrideJson) */
FString MergeStateJson(const FString &BaseJson, const FString &OverrideJson) {
  return SerializeStateJson(
      MergeJsonObjects({ParseStateJson(BaseJson), ParseStateJson(OverrideJson)}));
}

} // namespace

/** User Story: As a blueprints consumer, I need to invoke parse json through a stable signature so the blueprints workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> UGenericStateBlueprintLibrary::ParseJson(const FString &JsonData) */
TSharedPtr<FJsonObject>
UGenericStateBlueprintLibrary::ParseJson(const FString &JsonData) {
  return ParseStateJson(JsonData);
}

/** User Story: As a blueprints consumer, I need to invoke serialize json through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::SerializeJson( const TSharedPtr<FJsonObject> &JsonObject) */
FString UGenericStateBlueprintLibrary::SerializeJson(
    const TSharedPtr<FJsonObject> &JsonObject) {
  return SerializeStateJson(JsonObject);
}

// ── Read Operations ──

/** User Story: As a blueprints consumer, I need to invoke get state string through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::GetStateString( const FString &JsonData, const FString &FieldName) */
FString UGenericStateBlueprintLibrary::GetStateString(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<FString>(JsonData, FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke get state float through a stable signature so the blueprints workflow remains explicit and composable. @fn float UGenericStateBlueprintLibrary::GetStateFloat( const FString &JsonData, const FString &FieldName) */
float UGenericStateBlueprintLibrary::GetStateFloat(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<float>(JsonData, FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke get state int through a stable signature so the blueprints workflow remains explicit and composable. @fn int32 UGenericStateBlueprintLibrary::GetStateInt( const FString &JsonData, const FString &FieldName) */
int32 UGenericStateBlueprintLibrary::GetStateInt(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<int32>(JsonData, FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke get state bool through a stable signature so the blueprints workflow remains explicit and composable. @fn bool UGenericStateBlueprintLibrary::GetStateBool( const FString &JsonData, const FString &FieldName) */
bool UGenericStateBlueprintLibrary::GetStateBool(
    const FString &JsonData, const FString &FieldName) {
  return ReadStateJsonField<bool>(JsonData, FieldName);
}

/** User Story: As a blueprints consumer, I need to invoke has state field through a stable signature so the blueprints workflow remains explicit and composable. @fn bool UGenericStateBlueprintLibrary::HasStateField( const FString &JsonData, const FString &FieldName) */
bool UGenericStateBlueprintLibrary::HasStateField(
    const FString &JsonData, const FString &FieldName) {
  return HasStateJsonField(JsonData, FieldName);
}

// ── Write Operations ──

/** User Story: As a blueprints consumer, I need to invoke set state string through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::SetStateString( const FGenericStateFieldRequest &Request, const FString &Value) */
FString UGenericStateBlueprintLibrary::SetStateString(
    const FGenericStateFieldRequest &Request,
    const FString &Value) {
  return WriteStateJsonField<FString>(Request, Value);
}

/** User Story: As a blueprints consumer, I need to invoke set state float through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::SetStateFloat( const FGenericStateFieldRequest &Request, float Value) */
FString UGenericStateBlueprintLibrary::SetStateFloat(
    const FGenericStateFieldRequest &Request, float Value) {
  return WriteStateJsonField<float>(Request, Value);
}

/** User Story: As a blueprints consumer, I need to invoke set state int through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::SetStateInt( const FGenericStateFieldRequest &Request, int32 Value) */
FString UGenericStateBlueprintLibrary::SetStateInt(
    const FGenericStateFieldRequest &Request, int32 Value) {
  return WriteStateJsonField<int32>(Request, Value);
}

/** User Story: As a blueprints consumer, I need to invoke set state bool through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::SetStateBool( const FGenericStateFieldRequest &Request, bool Value) */
FString UGenericStateBlueprintLibrary::SetStateBool(
    const FGenericStateFieldRequest &Request, bool Value) {
  return WriteStateJsonField<bool>(Request, Value);
}

// ── Utility ──

/** User Story: As a blueprints consumer, I need to invoke merge state through a stable signature so the blueprints workflow remains explicit and composable. @fn FString UGenericStateBlueprintLibrary::MergeState( const FString &BaseJson, const FString &OverrideJson) */
FString UGenericStateBlueprintLibrary::MergeState(
    const FString &BaseJson, const FString &OverrideJson) {
  return MergeStateJson(BaseJson, OverrideJson);
}
