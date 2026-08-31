#pragma once

#include "Features/Components/Data/Settings/ForbocAI/Protocol/Command/ProtocolCommandTypes.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/Ghost/GhostTypes.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/NPC/NPCTypes.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/Presentation/ProtocolPresentationTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FProtocolSettings {
  FForbocAIErrorSettings Errors;
  FLogSettings Logging;
  FForbocAINpcSettings Npc;
  FForbocAIGhostSettings Ghost;
};

/** User Story: As the protocol state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FProtocolSettings &Left, const FProtocolSettings &Right) */
inline bool operator==(const FProtocolSettings &Left,
                       const FProtocolSettings &Right) {
  return Left.Errors == Right.Errors && Left.Logging == Right.Logging &&
         Left.Npc == Right.Npc && Left.Ghost == Right.Ghost;
}

/** User Story: As the protocol state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FProtocolSettings &Left, const FProtocolSettings &Right) */
inline bool operator!=(const FProtocolSettings &Left,
                       const FProtocolSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
