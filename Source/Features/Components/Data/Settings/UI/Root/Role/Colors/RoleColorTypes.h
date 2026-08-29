#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRoleColorSettings {
  FLinearColor PlayerColor;
  FLinearColor SystemColor;
  FLinearColor NpcColor;
  FLinearColor UnknownColor;
};

/** User Story: As a UI settings consumer, I need to compare role colors so speaker presentation remains deterministic. @fn inline bool operator==(const FRoleColorSettings &Left, const FRoleColorSettings &Right) */
inline bool operator==(const FRoleColorSettings &Left,
                       const FRoleColorSettings &Right) {
  return Left.PlayerColor == Right.PlayerColor &&
         Left.SystemColor == Right.SystemColor &&
         Left.NpcColor == Right.NpcColor &&
         Left.UnknownColor == Right.UnknownColor;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
