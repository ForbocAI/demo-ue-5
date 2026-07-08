#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalReducers {

struct FBotGoalsTownspeopleSeededRequest {
  FBotGoalState State;
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Game::Data::FBotSettings Settings;
};

struct FBotGoalsHorsesSeededRequest {
  FBotGoalState State;
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Game::Data::FBotSettings Settings;
};

FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action);
FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalAssignment> &Action);
FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalCompleted> &Action);
TMap<FString, FBotStrategicGoal> ReduceActiveGoalsById(
    const TArray<FBotGoalComponent> &Goals);
FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalsTownspeopleSeededRequest &Request);
FBotGoalState ReduceHorsesSeeded(
    const FBotGoalsHorsesSeededRequest &Request);

} // namespace BotGoalReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Goals/GoalsActions.h"
#include "Features/Systems/Bots/Goals/GoalsSelectors.h"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalSlice {

const rtk::Slice<FBotGoalState> &GetSlice();

} // namespace BotGoalSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/Source/Mapping/MappingAdapters.h"
#include "Features/Systems/Bots/Goals/GoalsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalFactories {

template <typename Seed>
using TBotGoalsFromSeedsRequest =
    BotSourceMappingAdapters::TSeedSettingsRequest<Seed, Data::FBotSettings>;

FBotGoalState CreateInitialState();
FBotStrategicGoal Goal(const FBotStrategicGoal &Source);
FBotGoalComponent Component(const FBotGoalComponent &Source);
TArray<FBotGoalComponent>
FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request);
TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotGoalFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
