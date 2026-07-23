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

/** User Story: As a features systems landmarks consumer, I need to compare values for equality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator==(const FLandmark &Left, const FLandmark &Right) */
inline bool operator==(const FLandmark &Left, const FLandmark &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale;
}

/** User Story: As a features systems landmarks consumer, I need to compare values for inequality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator!=(const FLandmark &Left, const FLandmark &Right) */
inline bool operator!=(const FLandmark &Left, const FLandmark &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems landmarks consumer, I need to compare values for equality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator==(const FLandmarkSource &Left, const FLandmarkSource &Right) */
inline bool operator==(const FLandmarkSource &Left,
                       const FLandmarkSource &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale;
}

/** User Story: As a features systems landmarks consumer, I need to compare values for inequality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator!=(const FLandmarkSource &Left, const FLandmarkSource &Right) */
inline bool operator!=(const FLandmarkSource &Left,
                       const FLandmarkSource &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems landmarks consumer, I need to compare values for equality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator==(const FLandmarkState &Left, const FLandmarkState &Right) */
inline bool operator==(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems landmarks consumer, I need to compare values for inequality through a stable signature so the features systems landmarks workflow remains explicit and composable. @fn inline bool operator!=(const FLandmarkState &Left, const FLandmarkState &Right) */
inline bool operator!=(const FLandmarkState &Left,
                       const FLandmarkState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
