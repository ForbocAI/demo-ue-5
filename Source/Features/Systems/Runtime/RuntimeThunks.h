#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeThunks {

/**
 * @brief RTK thunk that loads runtime data and resolves the player spawn.
 */
rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @brief RTK thunk that seeds data and resolves the full runtime level view
 * payload.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

/**
 * @brief RTK thunk that resolves a local dialogue reply.
 */
rtk::ThunkAction<FDialogueReplyPayload, FRuntimeState>
RequestLocalDialogueReply(const FLocalDialogueReplyRequest &Request);

/**
 * @brief RTK thunk that composes dialogue and UI effects for interaction.
 */
rtk::ThunkAction<FRuntimeTownspersonInteractionPayload, FRuntimeState>
RequestTownspersonInteraction(
    const FRuntimeTownspersonInteractionRequest &Request);

} // namespace RuntimeThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
