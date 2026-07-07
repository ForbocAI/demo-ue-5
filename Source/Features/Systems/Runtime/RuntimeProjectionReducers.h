#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/**
 * @brief Projects reducer-owned root state into the neutral ECS world.
 */
FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State);

/**
 * @brief Reports whether an action mutates state selected by ECS projection.
 */
bool ShouldProjectRuntimeAction(const rtk::AnyAction &Action);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
