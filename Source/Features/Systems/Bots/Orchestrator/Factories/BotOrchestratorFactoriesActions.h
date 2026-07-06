#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Orchestrator/Factories/BotOrchestratorFactoriesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotOrchestratorFactoriesActions {

inline const rtk::ActionCreator<FBotOrchestratorFactoriesPayload> &
FactoriesObserved() {
  static const func::Lazy<
      rtk::ActionCreator<FBotOrchestratorFactoriesPayload>>
      Creator = func::lazy(
          []() -> rtk::ActionCreator<FBotOrchestratorFactoriesPayload> {
            // RTK guidance: action type strings are reducer/action metadata, not JSON-authored runtime data.
            return rtk::createAction<FBotOrchestratorFactoriesPayload>(
                TEXT("systems/bots/orchestrator/factories/observed"));
          });
  return func::eval(Creator);
}

} // namespace BotOrchestratorFactoriesActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
