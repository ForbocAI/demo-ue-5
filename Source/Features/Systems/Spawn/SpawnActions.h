#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnActions {

/** User Story: As a features systems spawn consumer, I need to invoke player spawn anchored through a stable signature so the features systems spawn workflow remains explicit and composable. @fn const rtk::ActionCreator<FPointPayload> &PlayerSpawnAnchored() */
const rtk::ActionCreator<FPointPayload> &PlayerSpawnAnchored();

} // namespace SpawnActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
