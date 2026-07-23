#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeListeners {

/**
 * @fn rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware()
 * @brief Builds the runtime reactive-effects middleware.
 *
 * Reactive workflows that must observe *future* dispatched actions (rather than
 * drive one imperative request from a call site) belong in listener middleware,
 * not in views or reducers. This registers effects on the runtime bootstrap
 * async-thunk lifecycle so a rejected load is surfaced instead of swallowed,
 * and a fulfilled load is traceable, without any view or reducer owning that
 * reactive concern.
 *
 * @return rtk::Middleware<FRuntimeState> composed listener middleware for the
 * runtime store's middleware chain.
 * User Story: As a features systems consumer, I need to invoke create runtime listener middleware through a stable signature so the features systems workflow remains explicit and composable.
 */
rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware();

} // namespace RuntimeListeners
} // namespace Level
} // namespace Game
} // namespace ForbocAI
