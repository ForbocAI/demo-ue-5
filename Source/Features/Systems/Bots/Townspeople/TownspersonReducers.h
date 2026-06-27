#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
