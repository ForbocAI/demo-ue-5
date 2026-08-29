#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FRoleSettings {
  FString PlayerRoleLabel;
  FString SystemRoleLabel;
  FString NpcRoleLabel;
  FString UnknownRoleLabel;
};

/** User Story: As a UI settings consumer, I need to compare role labels so conversation attribution remains deterministic. @fn inline bool operator==(const FRoleSettings &Left, const FRoleSettings &Right) */
inline bool operator==(const FRoleSettings &Left,
                       const FRoleSettings &Right) {
  return Left.PlayerRoleLabel == Right.PlayerRoleLabel &&
         Left.SystemRoleLabel == Right.SystemRoleLabel &&
         Left.NpcRoleLabel == Right.NpcRoleLabel &&
         Left.UnknownRoleLabel == Right.UnknownRoleLabel;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
