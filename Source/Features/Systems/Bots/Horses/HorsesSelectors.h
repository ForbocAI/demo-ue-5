#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSelectors {

/** User Story: As a systems bots horses consumer, I need to invoke select all through a stable signature so the systems bots horses workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> SelectAll(const FHorseState &State) */
TArray<FHorseRouteSeed> SelectAll(const FHorseState &State);

} // namespace HorseSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
