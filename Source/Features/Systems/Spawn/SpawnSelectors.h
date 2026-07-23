#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnSelectors {

/** User Story: As a features systems spawn consumer, I need to invoke select player spawn through a stable signature so the features systems spawn workflow remains explicit and composable. @fn FPointPayload SelectPlayerSpawn(const FSpawnState &State) */
FPointPayload SelectPlayerSpawn(const FSpawnState &State);

} // namespace SpawnSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
