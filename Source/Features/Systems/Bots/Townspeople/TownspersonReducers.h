#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);

} // namespace TownspersonReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
