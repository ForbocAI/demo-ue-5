#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Horses/HorseActions.h"
#include "Features/Systems/Bots/Horses/HorseFactories.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorseSlice {

const rtk::Slice<FHorseState> &GetSlice();

} // namespace HorseSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
