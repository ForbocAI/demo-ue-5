#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FProjectionLabelSettings {
  FString RootReducerMillisecondsLabel;
  FString CombinedReducerMillisecondsLabel;
  FString EcsProjectionMillisecondsLabel;
  FString ProjectedEntityCountLabel;
  FString ProjectedComponentTypeCountLabel;
};

/** User Story: As a stats label consumer, I need to compare projection labels so immutable settings transitions remain explicit. @fn inline bool operator==(const FProjectionLabelSettings &Left, const FProjectionLabelSettings &Right) */
inline bool operator==(const FProjectionLabelSettings &Left,
                       const FProjectionLabelSettings &Right) {
  return Left.RootReducerMillisecondsLabel ==
             Right.RootReducerMillisecondsLabel &&
         Left.CombinedReducerMillisecondsLabel ==
             Right.CombinedReducerMillisecondsLabel &&
         Left.EcsProjectionMillisecondsLabel ==
             Right.EcsProjectionMillisecondsLabel &&
         Left.ProjectedEntityCountLabel == Right.ProjectedEntityCountLabel &&
         Left.ProjectedComponentTypeCountLabel ==
             Right.ProjectedComponentTypeCountLabel;
}

/** User Story: As a stats label consumer, I need to compare projection labels for inequality so immutable settings transitions remain explicit. @fn inline bool operator!=(const FProjectionLabelSettings &Left, const FProjectionLabelSettings &Right) */
inline bool operator!=(const FProjectionLabelSettings &Left,
                       const FProjectionLabelSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
