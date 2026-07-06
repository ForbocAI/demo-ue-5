#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
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
  FRotator Rotation;
  FVector Scale;
};

struct FLandmarkSource {
  FString Id;
  FString Label;
  ELandmarkKind Kind;
  FVector Location;
  FRotator Rotation;
  FVector Scale;
};

struct FLandmarkState {
  rtk::EntityState<FLandmark> Items;
};

inline bool operator==(const FLandmark &Left, const FLandmark &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale;
}

inline bool operator!=(const FLandmark &Left, const FLandmark &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLandmarkSource &Left,
                       const FLandmarkSource &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale;
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
} // namespace Game
} // namespace ForbocAI
