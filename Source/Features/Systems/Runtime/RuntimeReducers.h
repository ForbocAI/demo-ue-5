#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeEcsProjected(const FRuntimeState &State);
FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source);
FRuntimeTownspersonViewSpawn ReduceTownspersonViewSpawn(
    const FRuntimeTownspersonViewSpawnRequest &Request);
FRuntimeHorseViewSpawn
ReduceHorseViewSpawn(const FRuntimeHorseViewSpawnRequest &Request);
FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request);
FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply);
FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FDialogueReplyPayload &DialogueReply);
FRuntimeLevelViewPayload ReduceLevelViewPayload(
    const FRuntimeState &State, const FRuntimeLevelViewPayloadRequest &Request);
FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action);
FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action);

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
