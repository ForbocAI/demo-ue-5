#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeReducers {

/**
 * @brief Projects reducer-owned root state into the neutral ECS world.
 */
FRuntimeState ReduceRuntimeEcsProjected(const FRuntimeState &State);

/**
 * @brief Reduces a UE-observed interaction source into a dialogue request.
 */
FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source);

/**
 * @brief Reduces townsperson seed data into spawn data for views.
 */
FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request);
FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request);

/**
 * @brief Reduces runtime interaction data into dialogue thunk input.
 */
FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request);
FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply);

/**
 * @brief Reduces a dialogue reply into the combined runtime interaction payload.
 */
FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FDialogueReplyPayload &DialogueReply);
FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request);

/**
 * @brief Case reducer for RuntimeActions::TownspersonInteractionSourceObserved.
 */
FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action);
FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action);

/**
 * @brief Case reducer for RuntimeActions::RuntimeHydrated.
 */
inline FRuntimeState ReduceRuntimeHydrated(
    const FRuntimeState &State, const rtk::PayloadAction<FRuntimeState> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState) -> FRuntimeState {
            return ReduceRuntimeEcsProjected(Action.PayloadValue);
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
