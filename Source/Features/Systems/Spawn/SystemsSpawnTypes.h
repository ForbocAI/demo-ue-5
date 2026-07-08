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

inline bool operator==(const FPointPayload &Left,
                       const FPointPayload &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

inline bool operator!=(const FPointPayload &Left,
                       const FPointPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FPointSource &Left,
                       const FPointSource &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

inline bool operator!=(const FPointSource &Left,
                       const FPointSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpawnState &Left, const FSpawnState &Right) {
  return Left.PlayerSpawn == Right.PlayerSpawn;
}

inline bool operator!=(const FSpawnState &Left, const FSpawnState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
