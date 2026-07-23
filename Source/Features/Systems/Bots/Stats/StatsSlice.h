#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsReducers {

struct FBotStatsTownspeopleSeededRequest {
  FBotStatsState State;
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Game::Data::FBotSettings Settings;
};

struct FBotStatsHorsesSeededRequest {
  FBotStatsState State;
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Game::Data::FBotSettings Settings;
};

/** User Story: As a systems bots stats consumer, I need to invoke reduce bot stats seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceBotStatsSeeded( const FBotStatsState &State, const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action) */
FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action);
/** User Story: As a systems bots stats consumer, I need to invoke reduce bot stats updated through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceBotStatsUpdated( const FBotStatsState &State, const rtk::PayloadAction<FBotStatsUpdate> &Action) */
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action);
/** User Story: As a systems bots stats consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceTownspeopleSeeded( const FBotStatsTownspeopleSeededRequest &Request) */
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsTownspeopleSeededRequest &Request);
/** User Story: As a systems bots stats consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState ReduceHorsesSeeded( const FBotStatsHorsesSeededRequest &Request) */
FBotStatsState ReduceHorsesSeeded(
    const FBotStatsHorsesSeededRequest &Request);

} // namespace BotStatsReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Stats/StatsActions.h"
#include "Features/Systems/Bots/Stats/BotsStatsSelectors.h"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsSlice {

/** User Story: As a systems bots stats consumer, I need to invoke get slice through a stable signature so the systems bots stats workflow remains explicit and composable. @fn const rtk::Slice<FBotStatsState> &GetSlice() */
const rtk::Slice<FBotStatsState> &GetSlice();

} // namespace BotStatsSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"
#include "Features/Systems/Bots/Stats/BotsStatsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsFactories {

template <typename Seed>
using TBotStatsFromSeedsRequest =
    BotSourceMappingAdapters::TSeedSettingsRequest<Seed, Data::FBotSettings>;

/** User Story: As a systems bots stats consumer, I need to invoke create initial state through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsState CreateInitialState() */
FBotStatsState CreateInitialState();
/** User Story: As a systems bots stats consumer, I need to invoke component through a stable signature so the systems bots stats workflow remains explicit and composable. @fn FBotStatsComponent Component(const FBotStatsSource &Source) */
FBotStatsComponent Component(const FBotStatsSource &Source);
/** User Story: As a systems bots stats consumer, I need to invoke from townspeople through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request) */
TArray<FBotStatsComponent>
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request);
/** User Story: As a systems bots stats consumer, I need to invoke from horses through a stable signature so the systems bots stats workflow remains explicit and composable. @fn TArray<FBotStatsComponent> FromHorses(const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request) */
TArray<FBotStatsComponent>
FromHorses(const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotStatsFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
