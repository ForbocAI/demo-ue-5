#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotOrchestratorActions {

inline const rtk::ActionCreator<FBotOrchestratorPayload> &
OrchestratorObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotOrchestratorPayload>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FBotOrchestratorPayload> {
            return rtk::createAction<FBotOrchestratorPayload>(
                TEXT("systems/bots/orchestrator/observed"));
          });
  return func::eval(Creator);
}

} // namespace BotOrchestratorActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
