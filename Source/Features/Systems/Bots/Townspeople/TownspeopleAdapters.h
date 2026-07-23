#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonAdapters {

/** User Story: As a systems bots townspeople consumer, I need to invoke townsperson adapter through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::EntityAdapter<FTownspersonSeed> &TownspersonAdapter() */
const rtk::EntityAdapter<FTownspersonSeed> &TownspersonAdapter();

} // namespace TownspersonAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
