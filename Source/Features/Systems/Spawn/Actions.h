#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored();

} // namespace SpawnActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
