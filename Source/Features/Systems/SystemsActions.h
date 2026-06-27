#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/SystemsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsActions {

inline const rtk::ActionCreator<FSystemsPayload> &SystemsObserved() {
  static const func::Lazy<rtk::ActionCreator<FSystemsPayload>> Creator =
      func::lazy([]() -> rtk::ActionCreator<FSystemsPayload> {
        return rtk::createAction<FSystemsPayload>(
            TEXT("systems/observed"));
      });
  return func::eval(Creator);
}

} // namespace SystemsActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
