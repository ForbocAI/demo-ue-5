#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeSlice {

/**
 * @brief Returns the root RTK slice composed into Store::ConfigureStore.
 */
const rtk::Slice<FRuntimeState> &GetSlice();

/**
 * @brief Public thunk re-export for player spawn loading.
 */
rtk::ThunkAction<FSpawnPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @brief Public thunk re-export for runtime level view payload loading.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

/**
 * @brief Public thunk re-export for deterministic local dialogue replies.
 */
rtk::ThunkAction<FDialogueReplyPayload, FRuntimeState>
RequestLocalDialogueReply(const FLocalDialogueReplyRequest &Request);

/**
 * @brief Public thunk re-export for the townsperson interaction workflow.
 */
rtk::ThunkAction<FRuntimeTownspersonInteractionPayload, FRuntimeState>
RequestTownspersonInteraction(
    const FRuntimeTownspersonInteractionRequest &Request);

} // namespace RuntimeSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
