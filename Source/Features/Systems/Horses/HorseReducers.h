#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseReducers {

FHorseState ReduceHorsesSeeded(
    const FHorseState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace HorseReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
