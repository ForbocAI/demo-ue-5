#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FBotPositionComponent {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPositionSource {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPositionMoved {
  FString Id;
  FLevelLocalPoint LocalLocation;
  FVector WorldLocation;
  bool bHasWorldLocation;
  bool bFacingRight;
};

struct FBotPositionState {
  rtk::EntityState<FBotPositionComponent> Items;
};

inline bool operator==(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionComponent &Left,
                       const FBotPositionComponent &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionSource &Left,
                       const FBotPositionSource &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return Left.Id == Right.Id && Left.LocalLocation == Right.LocalLocation &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.bHasWorldLocation == Right.bHasWorldLocation &&
         Left.bFacingRight == Right.bFacingRight;
}

inline bool operator!=(const FBotPositionMoved &Left,
                       const FBotPositionMoved &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return Left.Items == Right.Items;
}

inline bool operator!=(const FBotPositionState &Left,
                       const FBotPositionState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
