#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded();
const rtk::ActionCreator<FTownspersonViewDefaultsRequest> &
ViewDefaultsRequested();
const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> &
InteractionOverlapObserved();

} // namespace TownspersonActions
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
