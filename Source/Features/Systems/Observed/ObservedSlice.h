#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/ObservedTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsReducers {

inline FSystemsState
ReduceSystemsObserved(const FSystemsState &State,
                      const rtk::PayloadAction<FSystemsPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FSystemsState Next) -> FSystemsState {
            Next.ActionId = func::just(Action.PayloadValue.Id);
            Next.bReady = true;
            return Next;
          })
      .val;
}

} // namespace SystemsReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Observed/ObservedActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/Observed/ObservedSelectors.h"
#include "Features/Systems/Observed/ObservedTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsSlice {

inline FSystemsState CreateInitialState() {
  return (func::pipe(FSystemsState{}) |
          [](FSystemsState State) -> FSystemsState {
            State.ActionId = func::nothing<FString>();
            State.bReady = false;
            return State;
          })
      .val;
}

inline const rtk::Slice<FSystemsState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSystemsState>> Slice =
      func::lazy([]() -> rtk::Slice<FSystemsState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FSystemsState>(
            TEXT("systems"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FSystemsState> &Builder) {
              Builder.addCase(SystemsActions::SystemsObserved(),
                              SystemsReducers::ReduceSystemsObserved);
            });
      });
  return func::eval(Slice);
}

} // namespace SystemsSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
