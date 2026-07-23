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

/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goals seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalsSeeded( const FBotGoalState &State, const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action) */
FBotGoalState ReduceBotGoalsSeeded(
    const FBotGoalState &State,
    const rtk::PayloadAction<TArray<FBotGoalComponent>> &Action);
/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goal assigned through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalAssigned( const FBotGoalState &State, const rtk::PayloadAction<FBotGoalAssignment> &Action) */
FBotGoalState ReduceBotGoalAssigned(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalAssignment> &Action);
/** User Story: As a systems bots goals consumer, I need to invoke reduce bot goal completed through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceBotGoalCompleted( const FBotGoalState &State, const rtk::PayloadAction<FBotGoalCompleted> &Action) */
FBotGoalState ReduceBotGoalCompleted(
    const FBotGoalState &State,
    const rtk::PayloadAction<FBotGoalCompleted> &Action);
/** User Story: As a systems bots goals consumer, I need to invoke reduce active goals by id through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TMap<FString, FBotStrategicGoal> ReduceActiveGoalsById( const TArray<FBotGoalComponent> &Goals) */
TMap<FString, FBotStrategicGoal> ReduceActiveGoalsById(
    const TArray<FBotGoalComponent> &Goals);
/** User Story: As a systems bots goals consumer, I need to invoke reduce townspeople seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceTownspeopleSeeded( const FBotGoalsTownspeopleSeededRequest &Request) */
FBotGoalState ReduceTownspeopleSeeded(
    const FBotGoalsTownspeopleSeededRequest &Request);
/** User Story: As a systems bots goals consumer, I need to invoke reduce horses seeded through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState ReduceHorsesSeeded( const FBotGoalsHorsesSeededRequest &Request) */
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

/** User Story: As a systems bots goals consumer, I need to invoke get slice through a stable signature so the systems bots goals workflow remains explicit and composable. @fn const rtk::Slice<FBotGoalState> &GetSlice() */
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

/** User Story: As a systems bots goals consumer, I need to invoke create initial state through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalState CreateInitialState() */
FBotGoalState CreateInitialState();
/** User Story: As a systems bots goals consumer, I need to invoke goal through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotStrategicGoal Goal(const FBotStrategicGoal &Source) */
FBotStrategicGoal Goal(const FBotStrategicGoal &Source);
/** User Story: As a systems bots goals consumer, I need to invoke component through a stable signature so the systems bots goals workflow remains explicit and composable. @fn FBotGoalComponent Component(const FBotGoalComponent &Source) */
FBotGoalComponent Component(const FBotGoalComponent &Source);
/** User Story: As a systems bots goals consumer, I need to invoke from townspeople through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TArray<FBotGoalComponent> FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request) */
TArray<FBotGoalComponent>
FromTownspeople(const TBotGoalsFromSeedsRequest<FTownspersonSeed> &Request);
/** User Story: As a systems bots goals consumer, I need to invoke from horses through a stable signature so the systems bots goals workflow remains explicit and composable. @fn TArray<FBotGoalComponent> FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request) */
TArray<FBotGoalComponent>
FromHorses(const TBotGoalsFromSeedsRequest<FHorseRouteSeed> &Request);

} // namespace BotGoalFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
