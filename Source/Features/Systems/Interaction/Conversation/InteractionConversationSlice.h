#pragma once

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/**
 * @fn FRuntimeState ReduceTownspersonInteractionSourceObserved( const FRuntimeState &State, const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action)
 * @brief Case reducer for RuntimeActions::TownspersonInteractionSourceObserved.
 * User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction source observed through a stable signature so the systems interaction conversation workflow remains explicit and composable.
 */
FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action);

/**
 * @fn FRuntimeTownspersonInteractionRequest ReduceTownspersonInteractionRequest( const FRuntimeTownspersonInteractionSource &Source)
 * @brief Reduces a UE-observed interaction source into a dialogue request.
 * User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction request through a stable signature so the systems interaction conversation workflow remains explicit and composable.
 */
FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source);

/**
 * @fn FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest( const FRuntimeTownspersonInteractionRequest &Request)
 * @brief Reduces runtime interaction data into dialogue reply input.
 * User Story: As a systems interaction conversation consumer, I need to invoke reduce local dialogue reply request through a stable signature so the systems interaction conversation workflow remains explicit and composable.
 */
FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request);

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce conversation presented payload through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FUIPayload ReduceConversationPresentedPayload( const FReplyPayload &DialogueReply, const ForbocAI::Game::Data::FUISettings &UISettings) */
FUIPayload ReduceConversationPresentedPayload(
    const FReplyPayload &DialogueReply,
    const ForbocAI::Game::Data::FUISettings &UISettings);

/**
 * @fn FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload( const FRuntimeTownspersonInteractionPayloadRequest &Request)
 * @brief Reduces an interaction request into dialogue and UI payloads.
 * User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction payload through a stable signature so the systems interaction conversation workflow remains explicit and composable.
 */
FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FRuntimeTownspersonInteractionPayloadRequest &Request);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
