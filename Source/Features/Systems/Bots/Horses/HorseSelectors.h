#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace HorseSelectors {

TArray<FHorseRouteSeed> SelectAll(const FHorseState &State);

} // namespace HorseSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
