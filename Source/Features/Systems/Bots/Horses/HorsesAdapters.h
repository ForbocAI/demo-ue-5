#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseAdapters {

/** User Story: As a systems bots horses consumer, I need to invoke horse adapter through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::EntityAdapter<FHorseRouteSeed> &HorseAdapter() */
const rtk::EntityAdapter<FHorseRouteSeed> &HorseAdapter();

} // namespace HorseAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
