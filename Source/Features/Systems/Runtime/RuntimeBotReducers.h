#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Runtime/RuntimeInteractionTypes.h"
#include "Features/Systems/Runtime/RuntimeLevelViewTypes.h"
#include "Features/Systems/Runtime/RuntimeStateTypes.h"
#include "Features/Systems/Runtime/RuntimeViewSpawnTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceRuntimeTownspeopleSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);

FRuntimeState ReduceRuntimeHorsesSeeded(
    const FRuntimeState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
