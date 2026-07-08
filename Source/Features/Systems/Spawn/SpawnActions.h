#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored();

} // namespace SpawnActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
