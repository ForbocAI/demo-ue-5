#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EBotEntityKind : uint8 { Townsperson, Horse };
enum class EBotAlignment : uint8 { Friendly, Neutral };

struct FBotEntity {
  FString Id;
  FString DisplayName;
  EBotEntityKind Kind;
  EBotAlignment Alignment;
  bool bActive;
};

struct FBotEntitySource {
  FString Id;
  FString DisplayName;
  EBotEntityKind Kind;
  EBotAlignment Alignment;
  bool bActive;
};

struct FBotState {
  rtk::EntityState<FBotEntity> Items;
};

/** User Story: As a features systems bots consumer, I need to compare values for equality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator==(const FBotEntity &Left, const FBotEntity &Right) */
inline bool operator==(const FBotEntity &Left, const FBotEntity &Right) {
  return Left.Id == Right.Id && Left.DisplayName == Right.DisplayName &&
         Left.Kind == Right.Kind && Left.Alignment == Right.Alignment &&
         Left.bActive == Right.bActive;
}

/** User Story: As a features systems bots consumer, I need to compare values for inequality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator!=(const FBotEntity &Left, const FBotEntity &Right) */
inline bool operator!=(const FBotEntity &Left, const FBotEntity &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems bots consumer, I need to compare values for equality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator==(const FBotEntitySource &Left, const FBotEntitySource &Right) */
inline bool operator==(const FBotEntitySource &Left,
                       const FBotEntitySource &Right) {
  return Left.Id == Right.Id && Left.DisplayName == Right.DisplayName &&
         Left.Kind == Right.Kind && Left.Alignment == Right.Alignment &&
         Left.bActive == Right.bActive;
}

/** User Story: As a features systems bots consumer, I need to compare values for inequality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator!=(const FBotEntitySource &Left, const FBotEntitySource &Right) */
inline bool operator!=(const FBotEntitySource &Left,
                       const FBotEntitySource &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems bots consumer, I need to compare values for equality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator==(const FBotState &Left, const FBotState &Right) */
inline bool operator==(const FBotState &Left, const FBotState &Right) {
  return Left.Items == Right.Items;
}

/** User Story: As a features systems bots consumer, I need to compare values for inequality through a stable signature so the features systems bots workflow remains explicit and composable. @fn inline bool operator!=(const FBotState &Left, const FBotState &Right) */
inline bool operator!=(const FBotState &Left, const FBotState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
