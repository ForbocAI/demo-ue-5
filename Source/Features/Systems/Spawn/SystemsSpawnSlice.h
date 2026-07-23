#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Spawn/SystemsSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnReducers {

/** User Story: As a features systems spawn consumer, I need to invoke reduce player spawn anchored through a stable signature so the features systems spawn workflow remains explicit and composable. @fn FSpawnState ReducePlayerSpawnAnchored(const FSpawnState &State, const rtk::PayloadAction<FPointPayload> &Action) */
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

/** User Story: As a features systems spawn consumer, I need to invoke get slice through a stable signature so the features systems spawn workflow remains explicit and composable. @fn const rtk::Slice<FSpawnState> &GetSlice() */
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

/** User Story: As a features systems spawn consumer, I need to invoke create initial state through a stable signature so the features systems spawn workflow remains explicit and composable. @fn FSpawnState CreateInitialState() */
FSpawnState CreateInitialState();
/** User Story: As a features systems spawn consumer, I need to invoke spawn point payload through a stable signature so the features systems spawn workflow remains explicit and composable. @fn FPointPayload SpawnPointPayload(const FPointSource &Source) */
FPointPayload SpawnPointPayload(const FPointSource &Source);

} // namespace SpawnFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
