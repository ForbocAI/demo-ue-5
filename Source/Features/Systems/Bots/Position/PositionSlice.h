#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionReducers {

/** User Story: As a systems bots position consumer, I need to invoke reduce bot positions seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionsSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action) */
FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action);
/** User Story: As a systems bots position consumer, I need to invoke reduce bot position upserted through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionUpserted( const FBotPositionState &State, const rtk::PayloadAction<FBotPositionComponent> &Action) */
FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action);
/** User Story: As a systems bots position consumer, I need to invoke reduce bot position moved through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceBotPositionMoved( const FBotPositionState &State, const rtk::PayloadAction<FBotPositionMoved> &Action) */
FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionMoved> &Action);
/** User Story: As a systems bots position consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceTownspeopleSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
/** User Story: As a systems bots position consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState ReduceHorsesSeeded( const FBotPositionState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);
/** User Story: As a systems bots position consumer, I need to invoke reduce initial patrol index through a stable signature so the systems bots position workflow remains explicit and composable. @fn int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute) */
int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute);
/** User Story: As a systems bots position consumer, I need to invoke reduce initial patrol location through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation( const FBotInitialPatrolLocationRequest &Request) */
FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request);
/** User Story: As a systems bots position consumer, I need to invoke reduce patrol advance through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPatrolAdvancePayload ReducePatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
ReducePatrolAdvance(const FBotPatrolAdvanceRequest &Request);

} // namespace BotPositionReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/Bots/Position/PositionSelectors.h"
#include "Features/Systems/Bots/Position/PositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionSlice {

/** User Story: As a systems bots position consumer, I need to invoke get slice through a stable signature so the systems bots position workflow remains explicit and composable. @fn const rtk::Slice<FBotPositionState> &GetSlice() */
const rtk::Slice<FBotPositionState> &GetSlice();

} // namespace BotPositionSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Position/PositionTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionFactories {

/** User Story: As a systems bots position consumer, I need to invoke create initial state through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionState CreateInitialState() */
FBotPositionState CreateInitialState();
/** User Story: As a systems bots position consumer, I need to invoke component through a stable signature so the systems bots position workflow remains explicit and composable. @fn FBotPositionComponent Component(const FBotPositionSource &Source) */
FBotPositionComponent Component(const FBotPositionSource &Source);
/** User Story: As a systems bots position consumer, I need to invoke from townspeople through a stable signature so the systems bots position workflow remains explicit and composable. @fn TArray<FBotPositionComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
/** User Story: As a systems bots position consumer, I need to invoke from horses through a stable signature so the systems bots position workflow remains explicit and composable. @fn TArray<FBotPositionComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotPositionFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
