#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseActions {

const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded();

} // namespace HorseActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
