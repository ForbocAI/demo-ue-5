#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FConversationLayoutSettings {
  float PanelPadding;
  float TitleSize;
  float BodySize;
};

/** User Story: As a UI settings consumer, I need to compare conversation layout so rendered spacing remains deterministic. @fn inline bool operator==(const FConversationLayoutSettings &Left, const FConversationLayoutSettings &Right) */
inline bool operator==(const FConversationLayoutSettings &Left,
                       const FConversationLayoutSettings &Right) {
  return FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.TitleSize, Right.TitleSize) &&
         FMath::IsNearlyEqual(Left.BodySize, Right.BodySize);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
