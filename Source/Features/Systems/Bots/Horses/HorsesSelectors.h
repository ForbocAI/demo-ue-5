#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Horses/HorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSelectors {

TArray<FHorseRouteSeed> SelectAll(const FHorseState &State);

} // namespace HorseSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
