#pragma once

#include "Core/rtk.hpp"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FJsonContentObjectRequest {
  FString RelativePath;
};

struct FJsonFieldRequest {
  TSharedPtr<FJsonObject> Object;
  FString FieldName;
};

template <typename Output> struct TJsonValueMapRequest {
  TArray<TSharedPtr<FJsonValue>> Values;
  TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue;
};

template <typename Output> struct TJsonValueMapStep {
  TArray<TSharedPtr<FJsonValue>> Values;
  TFunction<Output(const TSharedPtr<FJsonObject> &)> MapValue;
  int32 Index = 0;
  TArray<Output> Acc;
};

struct FDataLoadedPayload {
  FString RelativePath;
  bool bLoaded = false;
};

struct FDataState {
  func::Maybe<FString> LastPath = func::nothing<FString>();
  bool bLastLoadSucceeded = false;
  int32 LoadedObjectCount = 0;
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

inline bool operator==(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return Left.RelativePath == Right.RelativePath &&
         Left.bLoaded == Right.bLoaded;
}

inline bool operator!=(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDataState &Left, const FDataState &Right) {
  return Left.LastPath.hasValue == Right.LastPath.hasValue &&
         (!Left.LastPath.hasValue ||
          Left.LastPath.value == Right.LastPath.value) &&
         Left.bLastLoadSucceeded == Right.bLastLoadSucceeded &&
         Left.LoadedObjectCount == Right.LoadedObjectCount;
}

inline bool operator!=(const FDataState &Left, const FDataState &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
