#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpawnActions {

const rtk::ActionCreator<FSpawnPointPayload> &PlayerSpawnAnchored();

} // namespace SpawnActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
