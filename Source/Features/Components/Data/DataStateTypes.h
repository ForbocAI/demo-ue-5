#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FDataLoadedPayload {
  FString RelativePath;
  bool bLoaded = false;
};


struct FDataState {
  func::Maybe<FString> LastPath = func::nothing<FString>();
  bool bLastLoadSucceeded = false;
  int32 LoadedObjectCount = 0;
};


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
