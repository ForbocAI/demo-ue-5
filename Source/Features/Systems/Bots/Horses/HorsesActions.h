#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseActions {

/** User Story: As a systems bots horses consumer, I need to invoke horses seeded through a stable signature so the systems bots horses workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded() */
const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded();

} // namespace HorseActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
