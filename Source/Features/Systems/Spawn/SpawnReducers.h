#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::Action<FSpawnPointPayload> &Action);

} // namespace SpawnReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
