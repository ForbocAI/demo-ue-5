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

FBotStatsState ReduceBotStatsSeeded(
    const FBotStatsState &State,
    const rtk::PayloadAction<TArray<FBotStatsComponent>> &Action);
FBotStatsState ReduceBotStatsUpdated(
    const FBotStatsState &State,
    const rtk::PayloadAction<FBotStatsUpdate> &Action);
FBotStatsState ReduceTownspeopleSeeded(
    const FBotStatsTownspeopleSeededRequest &Request);
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

FBotStatsState CreateInitialState();
FBotStatsComponent Component(const FBotStatsSource &Source);
TArray<FBotStatsComponent>
FromTownspeople(const TBotStatsFromSeedsRequest<FTownspersonSeed> &Request);
TArray<FBotStatsComponent>
FromHorses(const TBotStatsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotStatsFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
