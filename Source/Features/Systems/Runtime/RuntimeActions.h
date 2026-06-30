#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeActions {

/**
 * @brief RTK action creator for replacing the runtime root state.
 */
inline const rtk::ActionCreator<FRuntimeState> &RuntimeHydrated() {
  static const func::Lazy<rtk::ActionCreator<FRuntimeState>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRuntimeState> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FRuntimeState>(TEXT("runtime/hydrated"));
      });
  return func::eval(Creator);
}

/**
 * @brief RTK action creator for observed townsperson dialogue source data.
 */
inline const rtk::ActionCreator<FRuntimeTownspersonInteractionSource> &
TownspersonInteractionSourceObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FRuntimeTownspersonInteractionSource>>
      Creator =
          func::lazy([]()
                         -> rtk::ActionCreator<
                             FRuntimeTownspersonInteractionSource> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FRuntimeTownspersonInteractionSource>(
                TEXT("runtime/townspersonInteractionSourceObserved"));
          });
  return func::eval(Creator);
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
