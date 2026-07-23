#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionActions {

/** User Story: As a systems bots position consumer, I need to invoke bot positions seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<TArray<FBotPositionComponent>> & BotPositionsSeeded() */
const rtk::ActionCreator<TArray<FBotPositionComponent>> &
BotPositionsSeeded();
/** User Story: As a systems bots position consumer, I need to invoke bot position upserted through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted() */
const rtk::ActionCreator<FBotPositionComponent> &BotPositionUpserted();
/** User Story: As a systems bots position consumer, I need to invoke bot position moved through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved() */
const rtk::ActionCreator<FBotPositionMoved> &BotPositionMoved();
/** User Story: As a systems bots position consumer, I need to invoke initial patrol observed through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotInitialPatrolLocationRequest> & InitialPatrolObserved() */
const rtk::ActionCreator<FBotInitialPatrolLocationRequest> &
InitialPatrolObserved();
/** User Story: As a systems bots position consumer, I need to invoke patrol advance observed through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::ActionCreator<FBotPatrolAdvanceRequest> &PatrolAdvanceObserved() */
const rtk::ActionCreator<FBotPatrolAdvanceRequest> &PatrolAdvanceObserved();

} // namespace BotPositionActions
} // namespace Level
} // namespace Game
} // namespace ForbocAI
