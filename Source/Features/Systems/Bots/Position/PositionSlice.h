#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Position/PositionTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionReducers {

FBotPositionState ReduceBotPositionsSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FBotPositionComponent>> &Action);
FBotPositionState ReduceBotPositionUpserted(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionComponent> &Action);
FBotPositionState ReduceBotPositionMoved(
    const FBotPositionState &State,
    const rtk::PayloadAction<FBotPositionMoved> &Action);
FBotPositionState ReduceTownspeopleSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotPositionState ReduceHorsesSeeded(
    const FBotPositionState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);
int32 ReduceInitialPatrolIndex(const TArray<FVector> &PatrolRoute);
FBotInitialPatrolLocationPayload ReduceInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request);
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

FBotPositionState CreateInitialState();
FBotPositionComponent Component(const FBotPositionSource &Source);
TArray<FBotPositionComponent>
FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotPositionComponent>
FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotPositionFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
