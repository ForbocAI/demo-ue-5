#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeActions {

inline const rtk::ActionCreator<FRuntimeState> &RuntimeHydrated() {
  static const func::Lazy<rtk::ActionCreator<FRuntimeState>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FRuntimeState> {
        return rtk::createAction<FRuntimeState>(TEXT("runtime/hydrated"));
      });
  return func::eval(Creator);
}

inline const rtk::ActionCreator<FRuntimeTownspersonInteractionSource> &
TownspersonInteractionSourceObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FRuntimeTownspersonInteractionSource>>
      Creator =
          func::lazy([]()
                         -> rtk::ActionCreator<
                             FRuntimeTownspersonInteractionSource> {
            return rtk::createAction<FRuntimeTownspersonInteractionSource>(
                TEXT("runtime/townspersonInteractionSourceObserved"));
          });
  return func::eval(Creator);
}

} // namespace RuntimeActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
