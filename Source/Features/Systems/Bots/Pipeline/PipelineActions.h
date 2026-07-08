#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotsPipelineTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPipelineActions {

inline const rtk::ActionCreator<FBotPipelinePayload> &
PipelineObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotPipelinePayload>>
      Creator = func::lazy([]() -> rtk::ActionCreator<FBotPipelinePayload> {
        // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createAction<FBotPipelinePayload>(
            TEXT("systems/bots/pipeline/observed"));
      });
  return func::eval(Creator);
}

} // namespace BotPipelineActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
