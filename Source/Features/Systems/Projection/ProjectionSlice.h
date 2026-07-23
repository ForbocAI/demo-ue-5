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
 * @fn FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State)
 * @brief Projects reducer-owned root state into the neutral ECS world.
 * User Story: As a features systems projection consumer, I need to invoke reduce runtime projected through a stable signature so the features systems projection workflow remains explicit and composable.
 */
FRuntimeState ReduceRuntimeProjected(const FRuntimeState &State);

/**
 * @fn bool ShouldProjectRuntimeAction(const rtk::AnyAction &Action)
 * @brief Reports whether an action mutates state selected by ECS projection.
 * User Story: As a features systems projection consumer, I need to invoke should project runtime action through a stable signature so the features systems projection workflow remains explicit and composable.
 */
bool ShouldProjectRuntimeAction(const rtk::AnyAction &Action);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
