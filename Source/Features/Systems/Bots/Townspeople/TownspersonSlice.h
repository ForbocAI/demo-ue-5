#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspersonActions.h"
#include "Features/Systems/Bots/Townspeople/TownspersonFactories.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice();

} // namespace TownspersonSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
