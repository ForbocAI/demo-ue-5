#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Readiness/ReadinessTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorReadinessActions {

inline const rtk::ActionCreator<FBotOrchestratorReadinessPayload> &
ReadinessObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FBotOrchestratorReadinessPayload>>
      Creator = func::lazy(
          []() -> rtk::ActionCreator<FBotOrchestratorReadinessPayload> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotOrchestratorReadinessPayload>(
                TEXT("systems/bots/orchestrator/readiness/observed"));
          });
  return func::eval(Creator);
}

} // namespace BotOrchestratorReadinessActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
