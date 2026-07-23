#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FDataLoadedPayload {
  FString RelativePath;
  bool bLoaded = false;
};


struct FDataState {
  func::Maybe<FString> Path = func::nothing<FString>();
  bool bLoadSucceeded = false;
  int32 LoadedObjectCount = 0;
};


/** User Story: As a components data state consumer, I need to compare values for equality through a stable signature so the components data state workflow remains explicit and composable. @fn inline bool operator==(const FDataLoadedPayload &Left, const FDataLoadedPayload &Right) */
inline bool operator==(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return Left.RelativePath == Right.RelativePath &&
         Left.bLoaded == Right.bLoaded;
}

/** User Story: As a components data state consumer, I need to compare values for inequality through a stable signature so the components data state workflow remains explicit and composable. @fn inline bool operator!=(const FDataLoadedPayload &Left, const FDataLoadedPayload &Right) */
inline bool operator!=(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a components data state consumer, I need to compare values for equality through a stable signature so the components data state workflow remains explicit and composable. @fn inline bool operator==(const FDataState &Left, const FDataState &Right) */
inline bool operator==(const FDataState &Left, const FDataState &Right) {
  return Left.Path.hasValue == Right.Path.hasValue &&
         (!Left.Path.hasValue ||
          Left.Path.value == Right.Path.value) &&
         Left.bLoadSucceeded == Right.bLoadSucceeded &&
         Left.LoadedObjectCount == Right.LoadedObjectCount;
}

/** User Story: As a components data state consumer, I need to compare values for inequality through a stable signature so the components data state workflow remains explicit and composable. @fn inline bool operator!=(const FDataState &Left, const FDataState &Right) */
inline bool operator!=(const FDataState &Left, const FDataState &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
