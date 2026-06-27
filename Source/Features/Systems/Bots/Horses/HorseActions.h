#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseActions {

const rtk::ActionCreator<TArray<FHorseRouteSeed>> &HorsesSeeded();

} // namespace HorseActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
