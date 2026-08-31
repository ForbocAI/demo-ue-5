#pragma once

#include "Features/Components/Data/Settings/ForbocAI/Protocol/Command/ProtocolCommandTypes.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/Presentation/ProtocolPresentationTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FForbocAINpcCommandSettings {
  FForbocAICommandSettings Create;
  FForbocAICommandSettings Process;
};

struct FForbocAINpcPresentationSettings
    : FForbocAIFlowPresentationSettings {
  FString ActionLinePrefix;
};

struct FForbocAINpcSettings {
  FForbocAINpcCommandSettings Commands;
  FForbocAINpcPresentationSettings Presentation;
};

/** User Story: As the NPC protocol state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAINpcCommandSettings &Left, const FForbocAINpcCommandSettings &Right) */
inline bool operator==(const FForbocAINpcCommandSettings &Left,
                       const FForbocAINpcCommandSettings &Right) {
  return Left.Create == Right.Create && Left.Process == Right.Process;
}

/** User Story: As the NPC protocol state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAINpcCommandSettings &Left, const FForbocAINpcCommandSettings &Right) */
inline bool operator!=(const FForbocAINpcCommandSettings &Left,
                       const FForbocAINpcCommandSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the NPC presentation state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAINpcPresentationSettings &Left, const FForbocAINpcPresentationSettings &Right) */
inline bool operator==(const FForbocAINpcPresentationSettings &Left,
                       const FForbocAINpcPresentationSettings &Right) {
  return static_cast<const FForbocAIFlowPresentationSettings &>(Left) ==
             static_cast<const FForbocAIFlowPresentationSettings &>(Right) &&
         Left.ActionLinePrefix == Right.ActionLinePrefix;
}

/** User Story: As the NPC presentation state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAINpcPresentationSettings &Left, const FForbocAINpcPresentationSettings &Right) */
inline bool operator!=(const FForbocAINpcPresentationSettings &Left,
                       const FForbocAINpcPresentationSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the NPC protocol state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAINpcSettings &Left, const FForbocAINpcSettings &Right) */
inline bool operator==(const FForbocAINpcSettings &Left,
                       const FForbocAINpcSettings &Right) {
  return Left.Commands == Right.Commands &&
         Left.Presentation == Right.Presentation;
}

/** User Story: As the NPC protocol state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAINpcSettings &Left, const FForbocAINpcSettings &Right) */
inline bool operator!=(const FForbocAINpcSettings &Left,
                       const FForbocAINpcSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
