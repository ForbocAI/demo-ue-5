#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GenericStateBlueprintLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGenericStateFieldRequest {
  GENERATED_BODY()

  UPROPERTY(BlueprintReadWrite, EditAnywhere,
            Category = "ForbocAI|GenericState")
  FString JsonData;

  UPROPERTY(BlueprintReadWrite, EditAnywhere,
            Category = "ForbocAI|GenericState")
  FString FieldName;
};

/**
 * UGenericStateBlueprintLibrary — Blueprint-callable helpers for
 * manipulating FAgentState.JsonData from Blueprints.
 *
 * FAgentState stores arbitrary game state as a JSON string.
 * These helpers let Blueprint users read, write, and merge
 * JSON fields without touching C++.
 *
 * All functions are pure (stateless) — they return new strings
 * rather than mutating inputs.
 *
 * Architecture: These helpers are neutral Blueprint primitives below gameplay
 * feature domains. They are pure JSON transforms, not RTK actions, reducers,
 * selectors, thunks, adapters, or stores.
 *
 * User Story: As a Blueprint developer, I need JSON state helpers
 * so I can read and write NPC state fields without C++ knowledge.
 */
UCLASS()
class FORBOCAIDEMO_API UGenericStateBlueprintLibrary
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

public:
  // ── Read Operations ──

  /**
   * Get a required string field from JSON state data.
   * User Story: As a blueprints consumer, I need to invoke get state string through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString GetStateString(const FString &JsonData, const FString &FieldName)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State String"))
  static FString GetStateString(const FString &JsonData,
                                 const FString &FieldName);

  /**
   * Get a required float field from JSON state data.
   * User Story: As a blueprints consumer, I need to invoke get state float through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static float GetStateFloat(const FString &JsonData, const FString &FieldName)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Float"))
  static float GetStateFloat(const FString &JsonData,
                              const FString &FieldName);

  /**
   * Get a required integer field from JSON state data.
   * User Story: As a blueprints consumer, I need to invoke get state int through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static int32 GetStateInt(const FString &JsonData, const FString &FieldName)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Int"))
  static int32 GetStateInt(const FString &JsonData,
                            const FString &FieldName);

  /**
   * Get a required boolean field from JSON state data.
   * User Story: As a blueprints consumer, I need to invoke get state bool through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static bool GetStateBool(const FString &JsonData, const FString &FieldName)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Bool"))
  static bool GetStateBool(const FString &JsonData,
                            const FString &FieldName);

  /**
   * Check if a field exists in the JSON state data.
   * User Story: As a blueprints consumer, I need to invoke has state field through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static bool HasStateField(const FString &JsonData, const FString &FieldName)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Has State Field"))
  static bool HasStateField(const FString &JsonData,
                             const FString &FieldName);

  // ── Write Operations (return new JSON string — immutable) ──

  /**
   * Set a string field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   * User Story: As a blueprints consumer, I need to invoke set state string through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString SetStateString(const FGenericStateFieldRequest &Request, const FString &Value)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State String"))
  static FString SetStateString(const FGenericStateFieldRequest &Request,
                                 const FString &Value);

  /**
   * Set a float field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   * User Story: As a blueprints consumer, I need to invoke set state float through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString SetStateFloat(const FGenericStateFieldRequest &Request, float Value)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Float"))
  static FString SetStateFloat(const FGenericStateFieldRequest &Request,
                               float Value);

  /**
   * Set an integer field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   * User Story: As a blueprints consumer, I need to invoke set state int through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString SetStateInt(const FGenericStateFieldRequest &Request, int32 Value)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Int"))
  static FString SetStateInt(const FGenericStateFieldRequest &Request,
                             int32 Value);

  /**
   * Set a boolean field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   * User Story: As a blueprints consumer, I need to invoke set state bool through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString SetStateBool(const FGenericStateFieldRequest &Request, bool Value)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Bool"))
  static FString SetStateBool(const FGenericStateFieldRequest &Request,
                              bool Value);

  // ── Utility ──

  /**
   * Merge two JSON state strings. Fields in Override take precedence.
   * Returns a NEW JSON string.
   * User Story: As a blueprints consumer, I need to invoke merge state through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString MergeState(const FString &BaseJson, const FString &OverrideJson)
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Merge State"))
  static FString MergeState(const FString &BaseJson,
                             const FString &OverrideJson);

private:
  /**
   * Parse JSON string into object. Returns nullptr on failure.
   * User Story: As a blueprints consumer, I need to invoke parse json through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static TSharedPtr<FJsonObject> ParseJson(const FString &JsonData)
   */
  static TSharedPtr<FJsonObject> ParseJson(const FString &JsonData);

  /**
   * Serialize JSON object back to string.
   * User Story: As a blueprints consumer, I need to invoke serialize json through a stable signature so the blueprints workflow remains explicit and composable.
   * @fn static FString SerializeJson(const TSharedPtr<FJsonObject> &JsonObject)
   */
  static FString SerializeJson(const TSharedPtr<FJsonObject> &JsonObject);
};
