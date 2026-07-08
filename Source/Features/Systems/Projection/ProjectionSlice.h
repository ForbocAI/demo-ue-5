#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/Presentation/LevelPresentationTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/Level/Presentation/Spawn/PresentationSpawnTypes.h"

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
