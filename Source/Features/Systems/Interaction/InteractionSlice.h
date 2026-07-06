#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/InteractionActions.h"
#include "Features/Systems/Interaction/InteractionReducers.h"
#include "Features/Systems/Interaction/InteractionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace InteractionSlice {

/**
 * @brief Creates initial state for the interaction RTK slice.
 */
inline FInteractionState CreateInitialState() {
  return (func::pipe(FInteractionState{}) |
          [](FInteractionState State) -> FInteractionState {
            State.LastActionId = func::nothing<FString>();
            return State;
          })
      .val;
}

/**
 * @brief Returns the interaction slice binding actions to case reducers.
 */
inline const rtk::Slice<FInteractionState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FInteractionState>> Slice =
      func::lazy([]() -> rtk::Slice<FInteractionState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FInteractionState>(
            TEXT("systems/interaction"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FInteractionState> &Builder) {
              Builder.addCase(
                  InteractionActions::TownspersonCandidatesObserved(),
                  InteractionReducers::ReduceTownspersonCandidatesObserved);
            });
      });
  return func::eval(Slice);
}

inline rtk::AnyAction
TownspersonCandidatesObserved(
    const FInteractionCandidatesObserved &Payload) {
  return InteractionActions::TownspersonCandidatesObserved()(Payload);
}

} // namespace InteractionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
