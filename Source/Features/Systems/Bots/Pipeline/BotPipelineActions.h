#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Pipeline/BotPipelineTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPipelineActions {

inline const rtk::ActionCreator<FBotPipelinePayload> &
PipelineObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotPipelinePayload>>
      Creator = func::lazy([]() -> rtk::ActionCreator<FBotPipelinePayload> {
        return rtk::createAction<FBotPipelinePayload>(
            TEXT("systems/bots/pipeline/observed"));
      });
  return func::eval(Creator);
}

} // namespace BotPipelineActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
