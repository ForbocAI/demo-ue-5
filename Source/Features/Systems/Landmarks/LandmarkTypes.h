#pragma once

#include "Core/rtk.hpp"
#include "CoreMinimal.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

enum class ELandmarkKind {
  Building,
  Road,
  Creek,
  TerrainMarker,
  Mine,
  Cemetery,
  Park
};

struct FLandmark {
  FString Id;
  FString Label;
  ELandmarkKind Kind;
  FVector Location;
  FVector Scale;
};

struct FLandmarkSource {
  FString Id;
  FString Label;
  ELandmarkKind Kind;
  FVector Location;
  FVector Scale;
};

struct FLandmarkState {
  rtk::EntityState<FLandmark> Items;
};

inline bool operator==(const FLandmark &Left, const FLandmark &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

inline bool operator!=(const FLandmark &Left, const FLandmark &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLandmarkSource &Left,
                       const FLandmarkSource &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Scale == Right.Scale;
}

inline bool operator!=(const FLandmarkSource &Left,
                       const FLandmarkSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
