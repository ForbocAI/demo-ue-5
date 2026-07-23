#pragma once

#include "Core/rtk.hpp"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"


namespace ForbocAI {
namespace Game {
namespace Data {

// --- Request structs (data contracts) ------------------------------------

struct FContentObjectRequest {
  FString RelativePath;
};

struct FContentArrayRequest {
  FString RelativePath;
};

struct FFieldRequest {
  TSharedPtr<FJsonObject> Object;
  FString FieldName;
};

/**
 * User story: As a data adapter author, I can validate authored array entries
 * without a feature system owning neutral JSON error-shaping logic.
 */
struct FArrayValueObjectRequest {
  TSharedPtr<FJsonValue> Value;
  FString FieldName;
  int32 Index = 0;
};

// --- Equality operators --------------------------------------------------

/** User Story: As a components data json consumer, I need to compare values for equality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator==(const FContentObjectRequest &Left, const FContentObjectRequest &Right) */
inline bool operator==(const FContentObjectRequest &Left,
                       const FContentObjectRequest &Right) {
  return Left.RelativePath == Right.RelativePath;
}

/** User Story: As a components data json consumer, I need to compare values for inequality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator!=(const FContentObjectRequest &Left, const FContentObjectRequest &Right) */
inline bool operator!=(const FContentObjectRequest &Left,
                       const FContentObjectRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a components data json consumer, I need to compare values for equality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator==(const FContentArrayRequest &Left, const FContentArrayRequest &Right) */
inline bool operator==(const FContentArrayRequest &Left,
                       const FContentArrayRequest &Right) {
  return Left.RelativePath == Right.RelativePath;
}

/** User Story: As a components data json consumer, I need to compare values for inequality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator!=(const FContentArrayRequest &Left, const FContentArrayRequest &Right) */
inline bool operator!=(const FContentArrayRequest &Left,
                       const FContentArrayRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a components data json consumer, I need to compare values for equality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator==(const FFieldRequest &Left, const FFieldRequest &Right) */
inline bool operator==(const FFieldRequest &Left,
                       const FFieldRequest &Right) {
  return Left.Object == Right.Object && Left.FieldName == Right.FieldName;
}

/** User Story: As a components data json consumer, I need to compare values for inequality through a stable signature so the components data json workflow remains explicit and composable. @fn inline bool operator!=(const FFieldRequest &Left, const FFieldRequest &Right) */
inline bool operator!=(const FFieldRequest &Left,
                       const FFieldRequest &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI

// --- Macro expansion primitives (declaration infrastructure) -------------
// Shared by JsonSettingsAdapters and JsonValueAdapters macro families.
// Owns arity expansion so declaration macros can be variadic without
// digit-suffixed helper families.

#define JSON_EMPTY()
#define JSON_DEFER(Id) Id JSON_EMPTY()
#define JSON_OBSTRUCT(...) __VA_ARGS__ JSON_DEFER(JSON_EMPTY)()
#define JSON_EXPAND(...)                                                     \
  JSON_EXPAND_MORE(                                                          \
      JSON_EXPAND_MORE(                                                      \
          JSON_EXPAND_MORE(                                                  \
              JSON_EXPAND_MORE(                                              \
                  JSON_EXPAND_MORE(                                          \
                      JSON_EXPAND_MORE(                                      \
                          JSON_EXPAND_MORE(                                  \
                              JSON_EXPAND_MORE(__VA_ARGS__))))))))
#define JSON_EXPAND_MORE(...)                                                \
  JSON_EXPAND_DEEP(                                                          \
      JSON_EXPAND_DEEP(                                                      \
          JSON_EXPAND_DEEP(                                                  \
              JSON_EXPAND_DEEP(                                              \
                  JSON_EXPAND_DEEP(                                          \
                      JSON_EXPAND_DEEP(                                      \
                          JSON_EXPAND_DEEP(                                  \
                              JSON_EXPAND_DEEP(__VA_ARGS__))))))))
#define JSON_EXPAND_DEEP(...)                                                \
  JSON_EXPAND_AGAIN(                                                         \
      JSON_EXPAND_AGAIN(                                                     \
          JSON_EXPAND_AGAIN(                                                 \
              JSON_EXPAND_AGAIN(                                             \
                  JSON_EXPAND_AGAIN(                                         \
                      JSON_EXPAND_AGAIN(                                     \
                          JSON_EXPAND_AGAIN(                                 \
                              JSON_EXPAND_AGAIN(__VA_ARGS__))))))))
#define JSON_EXPAND_AGAIN(...) __VA_ARGS__
