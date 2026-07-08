#pragma once

#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/**
 * @brief Case reducer for RuntimeActions::TownspersonInteractionSourceObserved.
 */
FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action);

/**
 * @brief Reduces a UE-observed interaction source into a dialogue request.
 */
FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source);

/**
 * @brief Reduces runtime interaction data into dialogue reply input.
 */
FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request);

FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply,
    const ForbocAI::Game::Data::FUISettings &UISettings);

/**
 * @brief Reduces an interaction request into dialogue and UI payloads.
 */
FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FRuntimeTownspersonInteractionPayloadRequest &Request);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
