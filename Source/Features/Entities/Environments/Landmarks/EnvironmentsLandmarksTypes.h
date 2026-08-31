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

/** User Story: As a landmark entity consumer, I need value equality so authored landmarks and ECS projections remain deterministic. @fn inline bool operator==(const FLandmark &Left, const FLandmark &Right) */
inline bool operator==(const FLandmark &Left, const FLandmark &Right) {
  return Left.Id == Right.Id && Left.Label == Right.Label &&
         Left.Kind == Right.Kind && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale;
}

/** User Story: As a landmark entity consumer, I need value inequality so landmark changes remain explicit. @fn inline bool operator!=(const FLandmark &Left, const FLandmark &Right) */
inline bool operator!=(const FLandmark &Left, const FLandmark &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
