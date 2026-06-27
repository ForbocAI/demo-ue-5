#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace HorseActions {

const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded();

} // namespace HorseActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
