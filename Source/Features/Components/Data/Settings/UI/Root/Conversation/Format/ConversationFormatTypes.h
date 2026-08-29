#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FConversationFormatSettings {
  FString ConversationTitleFormat;
  FString PlayerLineFormat;
  FString NpcReplyFormat;
  FString ConnectionMessageFormat;
  FString UnboundDialogueError;
  FString PayloadIdFormat;
};

/** User Story: As a UI settings consumer, I need to compare conversation formats so rendered dialogue remains deterministic. @fn inline bool operator==(const FConversationFormatSettings &Left, const FConversationFormatSettings &Right) */
inline bool operator==(const FConversationFormatSettings &Left,
                       const FConversationFormatSettings &Right) {
  return Left.ConversationTitleFormat == Right.ConversationTitleFormat &&
         Left.PlayerLineFormat == Right.PlayerLineFormat &&
         Left.NpcReplyFormat == Right.NpcReplyFormat &&
         Left.ConnectionMessageFormat == Right.ConnectionMessageFormat &&
         Left.UnboundDialogueError == Right.UnboundDialogueError &&
         Left.PayloadIdFormat == Right.PayloadIdFormat;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
