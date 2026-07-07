#pragma once

#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

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
