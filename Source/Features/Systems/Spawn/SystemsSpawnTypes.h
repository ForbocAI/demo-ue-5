#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FPointPayload {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FPointSource {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FSpawnState {
  FPointPayload PlayerSpawn;
};

/** User Story: As a features systems spawn consumer, I need to compare values for equality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator==(const FPointPayload &Left, const FPointPayload &Right) */
inline bool operator==(const FPointPayload &Left,
                       const FPointPayload &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

/** User Story: As a features systems spawn consumer, I need to compare values for inequality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator!=(const FPointPayload &Left, const FPointPayload &Right) */
inline bool operator!=(const FPointPayload &Left,
                       const FPointPayload &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems spawn consumer, I need to compare values for equality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator==(const FPointSource &Left, const FPointSource &Right) */
inline bool operator==(const FPointSource &Left,
                       const FPointSource &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

/** User Story: As a features systems spawn consumer, I need to compare values for inequality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator!=(const FPointSource &Left, const FPointSource &Right) */
inline bool operator!=(const FPointSource &Left,
                       const FPointSource &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems spawn consumer, I need to compare values for equality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator==(const FSpawnState &Left, const FSpawnState &Right) */
inline bool operator==(const FSpawnState &Left, const FSpawnState &Right) {
  return Left.PlayerSpawn == Right.PlayerSpawn;
}

/** User Story: As a features systems spawn consumer, I need to compare values for inequality through a stable signature so the features systems spawn workflow remains explicit and composable. @fn inline bool operator!=(const FSpawnState &Left, const FSpawnState &Right) */
inline bool operator!=(const FSpawnState &Left, const FSpawnState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
