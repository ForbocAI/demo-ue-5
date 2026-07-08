#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Interaction/Conversation/Types.h"
#include "Features/Systems/Level/View/Types.h"
#include "Features/Systems/State/Types.h"
#include "Features/Systems/View/Spawn/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Store {

/**
 * @brief Builds the single RTK-style runtime store for the UE runtime.
 *
 * @return EnhancedStore rooted at FRuntimeState and RuntimeSlice.
 *
 * Architecture: This is the Redux Toolkit `configureStore` boundary for the
 * project. It composes the root reducer and initial state once; functional-core
 * helpers stay below it as pure transforms and never replace store semantics.
 *
 * User story: As a game developer, I can reason about one authoritative store
 * so UE events flow through actions, reducers, selectors, and views in one
 * direction.
 */
rtk::EnhancedStore<FRuntimeState> ConfigureStore();

/**
 * @brief Returns the process-local runtime store singleton.
 *
 * @return Mutable RTK store used by UE view/effect boundaries to dispatch
 * actions or thunks and read selector inputs.
 *
 * Side effects: Lazily initializes the store on first access.
 */
rtk::EnhancedStore<FRuntimeState> &GetStore();

} // namespace Store
} // namespace Level
} // namespace Game
} // namespace ForbocAI
