#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SpawnTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::PayloadAction<FSpawnPointPayload> &Action);

} // namespace SpawnReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
