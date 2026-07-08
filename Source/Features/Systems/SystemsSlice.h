#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

#include "Features/Systems/Interaction/Conversation/InteractionConversationSlice.h"
#include "Features/Systems/Projection/ProjectionSlice.h"
#include "Features/Systems/View/ViewSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeAction(const FRuntimeState &State,
                                  const rtk::AnyAction &Action);

/**
 * @brief Case reducer for RuntimeActions::RuntimeHydrated.
 */
inline FRuntimeState ReduceRuntimeHydrated(
    const FRuntimeState &State, const rtk::PayloadAction<FRuntimeState> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState) -> FRuntimeState {
            return ReduceRuntimeProjected(Action.PayloadValue);
          })
      .val;
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSlice {

/**
 * @brief Returns the root RTK slice composed into Store::ConfigureStore.
 */
const rtk::Slice<FRuntimeState> &GetSlice();

/**
 * @brief Public thunk re-export for player spawn loading.
 */
rtk::ThunkAction<FPointPayload, FRuntimeState> RequestPlayerSpawn();

/**
 * @brief Public thunk re-export for runtime level view payload loading.
 */
rtk::ThunkAction<FRuntimeLevelViewPayload, FRuntimeState>
RequestLevelViewPayload();

} // namespace RuntimeSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeFactories {

/**
 * @brief Creates the initial root state by composing feature initial states.
 */
FRuntimeState CreateInitialState();

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
