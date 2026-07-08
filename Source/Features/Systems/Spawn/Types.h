#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

struct FSpawnPointPayload {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FSpawnPointSource {
  FVector Location;
  FRotator Rotation;
  FString AnchorLabel;
};

struct FSpawnState {
  FSpawnPointPayload PlayerSpawn;
};

inline bool operator==(const FSpawnPointPayload &Left,
                       const FSpawnPointPayload &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

inline bool operator!=(const FSpawnPointPayload &Left,
                       const FSpawnPointPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpawnPointSource &Left,
                       const FSpawnPointSource &Right) {
  return Left.Location == Right.Location && Left.Rotation == Right.Rotation &&
         Left.AnchorLabel == Right.AnchorLabel;
}

inline bool operator!=(const FSpawnPointSource &Left,
                       const FSpawnPointSource &Right) {
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
