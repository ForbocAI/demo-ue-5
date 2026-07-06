#pragma once

#include "Core/rtk.hpp"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FJsonContentObjectRequest {
  FString RelativePath;
};


struct FJsonFieldRequest {
  TSharedPtr<FJsonObject> Object;
  FString FieldName;
};

/**
 * User story: As a data adapter author, I can validate authored array entries
 * without a feature system owning neutral JSON error-shaping logic.
 */
struct FJsonArrayValueObjectRequest {
  TSharedPtr<FJsonValue> Value;
  FString FieldName;
  int32 Index = 0;
};


inline bool operator==(const FJsonContentObjectRequest &Left,
                       const FJsonContentObjectRequest &Right) {
  return Left.RelativePath == Right.RelativePath;
}

inline bool operator!=(const FJsonContentObjectRequest &Left,
                       const FJsonContentObjectRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FJsonFieldRequest &Left,
                       const FJsonFieldRequest &Right) {
  return Left.Object == Right.Object && Left.FieldName == Right.FieldName;
}

inline bool operator!=(const FJsonFieldRequest &Left,
                       const FJsonFieldRequest &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
