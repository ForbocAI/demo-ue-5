#pragma once

#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeFactories {

/**
 * @brief Creates the initial root state by composing feature initial states.
 */
FRuntimeState CreateInitialState();

} // namespace RuntimeFactories
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
