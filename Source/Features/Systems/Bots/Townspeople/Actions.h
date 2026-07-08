#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonActions {

const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded();
const rtk::ActionCreator<FTownspersonViewDefaultsRequest> &
ViewDefaultsRequested();
const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> &
InteractionOverlapObserved();

} // namespace TownspersonActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
