#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Horses/HorseActions.h"
#include "Features/Systems/Horses/HorseFactories.h"
#include "Features/Systems/Horses/HorseSelectors.h"
#include "Features/Systems/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice();

} // namespace HorseSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
