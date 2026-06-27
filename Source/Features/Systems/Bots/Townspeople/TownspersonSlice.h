#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Townspeople/TownspersonActions.h"
#include "Features/Systems/Townspeople/TownspersonFactories.h"
#include "Features/Systems/Townspeople/TownspersonSelectors.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonSlice {

const rtk::Slice<FTownspersonState> &GetSlice();

} // namespace TownspersonSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
