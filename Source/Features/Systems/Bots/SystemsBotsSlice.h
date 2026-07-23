#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotReducers {

/** User Story: As a features systems bots consumer, I need to invoke reduce bots seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceBotsSeeded(const FBotState &State, const rtk::PayloadAction<TArray<FBotEntity>> &Action) */
FBotState ReduceBotsSeeded(const FBotState &State,
                           const rtk::PayloadAction<TArray<FBotEntity>> &Action);
/** User Story: As a features systems bots consumer, I need to invoke reduce bot upserted through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceBotUpserted(const FBotState &State, const rtk::PayloadAction<FBotEntity> &Action) */
FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action);
/** User Story: As a features systems bots consumer, I need to invoke reduce townspeople seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceTownspeopleSeeded( const FBotState &State, const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action) */
FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
/** User Story: As a features systems bots consumer, I need to invoke reduce horses seeded through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState ReduceHorsesSeeded( const FBotState &State, const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action) */
FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/AISlice.h"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/SystemsBotsSelectors.h"
#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Goals/GoalsSlice.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Stats/StatsSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSlice {

/** User Story: As a features systems bots consumer, I need to invoke get slice through a stable signature so the features systems bots workflow remains explicit and composable. @fn const rtk::Slice<FBotState> &GetSlice() */
const rtk::Slice<FBotState> &GetSlice();

} // namespace BotSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotFactories {

/** User Story: As a features systems bots consumer, I need to invoke create initial state through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotState CreateInitialState() */
FBotState CreateInitialState();
/** User Story: As a features systems bots consumer, I need to invoke bot through a stable signature so the features systems bots workflow remains explicit and composable. @fn FBotEntity Bot(const FBotEntitySource &Source) */
FBotEntity Bot(const FBotEntitySource &Source);
/** User Story: As a features systems bots consumer, I need to invoke from townspeople through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds) */
TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
/** User Story: As a features systems bots consumer, I need to invoke from horses through a stable signature so the features systems bots workflow remains explicit and composable. @fn TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds) */
TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
