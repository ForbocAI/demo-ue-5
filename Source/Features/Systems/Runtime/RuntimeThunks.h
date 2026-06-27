#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeThunks {

rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn();
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();
rtk::ThunkAction<FDialogueReplyPayload, FRuntimeState>
RequestLocalDialogueReply(const FLocalDialogueReplyRequest &Request);
rtk::ThunkAction<FRuntimeTownspersonInteractionPayload, FRuntimeState>
RequestTownspersonInteraction(
    const FRuntimeTownspersonInteractionRequest &Request);

} // namespace RuntimeThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
