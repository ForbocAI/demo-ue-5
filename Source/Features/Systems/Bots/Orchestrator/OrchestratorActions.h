#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/OrchestratorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorActions {

/** User Story: As a systems bots orchestrator consumer, I need to invoke orchestrator observed through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn inline const rtk::ActionCreator<FBotOrchestratorPayload> & OrchestratorObserved() */
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
