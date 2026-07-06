#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/BotOrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorActions {

inline const rtk::ActionCreator<FBotOrchestratorPayload> &
OrchestratorObserved() {
  static const func::Lazy<rtk::ActionCreator<FBotOrchestratorPayload>>
      Creator =
          func::lazy([]() -> rtk::ActionCreator<FBotOrchestratorPayload> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotOrchestratorPayload>(
                TEXT("systems/bots/orchestrator/observed"));
          });
  return func::eval(Creator);
}

} // namespace BotOrchestratorActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
