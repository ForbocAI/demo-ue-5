#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Dialogue/DialogueTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueThunks {

const rtk::AsyncThunkConfig<FDialogueReplyPayload, FLocalDialogueReplyRequest,
                            FDialogueState> &
RequestLocalReply();

} // namespace DialogueThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
