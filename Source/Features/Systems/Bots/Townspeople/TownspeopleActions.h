#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonActions {

/** User Story: As a systems bots townspeople consumer, I need to invoke townspeople seeded through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded() */
const rtk::ActionCreator<TArray<FTownspersonSeed>> &TownspeopleSeeded();
/** User Story: As a systems bots townspeople consumer, I need to invoke view defaults requested through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<FTownspersonViewDefaultsRequest> & ViewDefaultsRequested() */
const rtk::ActionCreator<FTownspersonViewDefaultsRequest> &
ViewDefaultsRequested();
/** User Story: As a systems bots townspeople consumer, I need to invoke interaction overlap observed through a stable signature so the systems bots townspeople workflow remains explicit and composable. @fn const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> & InteractionOverlapObserved() */
const rtk::ActionCreator<FTownspersonInteractionOverlapRequest> &
InteractionOverlapObserved();

} // namespace TownspersonActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
