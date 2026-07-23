#pragma once

#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeFactories {

/** User Story: As a features systems initialization consumer, I need to invoke create initial state through a stable signature so the features systems initialization workflow remains explicit and composable. @fn FRuntimeState CreateInitialState() */
FRuntimeState CreateInitialState();

} // namespace RuntimeFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
