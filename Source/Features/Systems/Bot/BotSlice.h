#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"
#include "Features/Systems/Level/View/ViewTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"
#include "Features/Systems/View/Spawn/ViewSpawnTypes.h"

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
