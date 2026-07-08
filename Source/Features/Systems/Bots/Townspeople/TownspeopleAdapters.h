#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonAdapters {

const rtk::EntityAdapter<FTownspersonSeed> &TownspersonAdapter();

} // namespace TownspersonAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
