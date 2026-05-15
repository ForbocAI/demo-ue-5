#pragma once

#include "NPC/NPCModule.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "GenericStateBlueprintLibrary.generated.h"

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
 * User Story: As a Blueprint developer, I need JSON state helpers
 * so I can read and write NPC state fields without C++ knowledge.
 */
UCLASS()
class DEMOPROJECT_API UGenericStateBlueprintLibrary
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

public:
  // ── Read Operations ──

  /**
   * Get a string field from JSON state data.
   * Returns empty string if field doesn't exist.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State String"))
  static FString GetStateString(const FString &JsonData,
                                 const FString &FieldName);

  /**
   * Get a float field from JSON state data.
   * Returns 0.0 if field doesn't exist.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Float"))
  static float GetStateFloat(const FString &JsonData,
                              const FString &FieldName);

  /**
   * Get an integer field from JSON state data.
   * Returns 0 if field doesn't exist.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Int"))
  static int32 GetStateInt(const FString &JsonData,
                            const FString &FieldName);

  /**
   * Get a boolean field from JSON state data.
   * Returns false if field doesn't exist.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Get State Bool"))
  static bool GetStateBool(const FString &JsonData,
                            const FString &FieldName);

  /**
   * Check if a field exists in the JSON state data.
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
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State String"))
  static FString SetStateString(const FString &JsonData,
                                 const FString &FieldName,
                                 const FString &Value);

  /**
   * Set a float field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Float"))
  static FString SetStateFloat(const FString &JsonData,
                                const FString &FieldName, float Value);

  /**
   * Set an integer field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Int"))
  static FString SetStateInt(const FString &JsonData,
                              const FString &FieldName, int32 Value);

  /**
   * Set a boolean field in JSON state data.
   * Returns a NEW JSON string with the field added/updated.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Set State Bool"))
  static FString SetStateBool(const FString &JsonData,
                               const FString &FieldName, bool Value);

  // ── Utility ──

  /**
   * Merge two JSON state strings. Fields in Override take precedence.
   * Returns a NEW JSON string.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Merge State"))
  static FString MergeState(const FString &BaseJson,
                             const FString &OverrideJson);

  /**
   * Create an empty JSON state object: "{}".
   */
  UFUNCTION(BlueprintCallable, BlueprintPure,
            Category = "ForbocAI|GenericState",
            meta = (DisplayName = "Empty State"))
  static FString EmptyState();

private:
  /** Parse JSON string into object. Returns nullptr on failure. */
  static TSharedPtr<FJsonObject> ParseJson(const FString &JsonData);

  /** Serialize JSON object back to string. */
  static FString SerializeJson(const TSharedPtr<FJsonObject> &JsonObject);
};
