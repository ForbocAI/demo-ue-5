#pragma once

#include "Features/Systems/Runtime/RuntimeTypes.h"

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
