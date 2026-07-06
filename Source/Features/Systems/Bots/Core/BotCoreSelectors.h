#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Core/BotCoreTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotCoreSelectors {

/**
 * @brief Selects whether bot core systems are ready.
 * @signature inline bool SelectReady(const FBotCoreState &State)
 * @return True when the bot core slice has reached ready state.
 *
 * User story: As orchestration code, readiness should be selector-derived RTK
 * data before bot systems decide what ECS work may run.
 */
inline bool SelectReady(const FBotCoreState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/**
 * @brief Selects the last bot core action id if one exists.
 * @signature inline func::Maybe<FString> SelectLastActionId(const FBotCoreState &State)
 * @return A Maybe containing the last action id, or Nothing before action flow.
 *
 * User story: As runtime diagnostics, action lineage should stay as optional
 * FP data exposed through selectors rather than direct state access.
 */
inline func::Maybe<FString> SelectLastActionId(
    const FBotCoreState &State) {
  return State.LastActionId;
}

} // namespace BotCoreSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
