#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Behavior/BehaviorTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotCoreSelectors {

/**
 * @fn inline bool SelectReady(const FBotCoreState &State)
 * @brief Selects whether bot behavior systems are ready.
 * @return True when the bot behavior slice has reached ready state.
 *
 * User story: As orchestration code, readiness should be selector-derived RTK
 * data before bot systems decide what ECS work may run.
 * User Story: As a systems bots behavior consumer, I need to invoke select ready through a stable signature so the systems bots behavior workflow remains explicit and composable.
 */
inline bool SelectReady(const FBotCoreState &State) {
  return (func::pipe(State.bReady) | [](bool bReady) -> bool {
            return bReady;
          })
      .val;
}

/**
 * @fn inline func::Maybe<FString> SelectActionId( const FBotCoreState &State)
 * @brief Selects the bot behavior action id if one exists.
 * @return A Maybe containing the action id, or Nothing before action flow.
 *
 * User story: As runtime diagnostics, action lineage should stay as optional
 * FP data exposed through selectors rather than direct state access.
 * User Story: As a systems bots behavior consumer, I need to invoke select action id through a stable signature so the systems bots behavior workflow remains explicit and composable.
 */
inline func::Maybe<FString> SelectActionId(
    const FBotCoreState &State) {
  return State.ActionId;
}

} // namespace BotCoreSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
