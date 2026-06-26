#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded();

} // namespace TownspersonActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
