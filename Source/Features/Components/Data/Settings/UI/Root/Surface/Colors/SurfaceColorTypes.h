#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FSurfaceColorSettings {
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor ReplyColor;
};

/** User Story: As a UI settings consumer, I need to compare surface colors so conversation framing remains deterministic. @fn inline bool operator==(const FSurfaceColorSettings &Left, const FSurfaceColorSettings &Right) */
inline bool operator==(const FSurfaceColorSettings &Left,
                       const FSurfaceColorSettings &Right) {
  return Left.PanelColor == Right.PanelColor &&
         Left.TitleColor == Right.TitleColor &&
         Left.ReplyColor == Right.ReplyColor;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
