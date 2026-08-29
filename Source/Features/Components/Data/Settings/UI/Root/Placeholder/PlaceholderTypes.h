#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FPlaceholderSettings {
  FString PlaceholderTitle;
  FString PlaceholderPlayerLine;
  FString PlaceholderNpcReply;
};

/** User Story: As a UI settings consumer, I need to compare placeholders so unbound conversation presentation remains deterministic. @fn inline bool operator==(const FPlaceholderSettings &Left, const FPlaceholderSettings &Right) */
inline bool operator==(const FPlaceholderSettings &Left,
                       const FPlaceholderSettings &Right) {
  return Left.PlaceholderTitle == Right.PlaceholderTitle &&
         Left.PlaceholderPlayerLine == Right.PlaceholderPlayerLine &&
         Left.PlaceholderNpcReply == Right.PlaceholderNpcReply;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
