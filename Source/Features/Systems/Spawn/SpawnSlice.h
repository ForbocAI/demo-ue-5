#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::PayloadAction<FPointPayload> &Action);

} // namespace SpawnReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnSlice {

const rtk::Slice<FSpawnState> &GetSlice();

} // namespace SpawnSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnFactories {

FSpawnState CreateInitialState();
FPointPayload SpawnPointPayload(const FPointSource &Source);

} // namespace SpawnFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
