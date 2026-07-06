#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseReducers {

FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace HorseReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
