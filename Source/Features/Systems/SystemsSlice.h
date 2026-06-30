#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsAdapters.h"
#include "Features/Systems/SystemsReducers.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Features/Systems/SystemsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsSlice {

inline FSystemsState CreateInitialState() {
  return (func::pipe(FSystemsState{}) |
          [](FSystemsState State) -> FSystemsState {
            State.LastActionId = func::nothing<FString>();
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
} // namespace Demo
} // namespace ForbocAI
