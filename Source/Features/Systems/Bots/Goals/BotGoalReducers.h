#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/Goals/BotGoalTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotGoalReducers {

struct FBotGoalsTownspeopleSeededRequest {
  FBotGoalState State;
  TArray<FTownspersonSeed> Seeds;
  ForbocAI::Game::Data::FBotRuntimeSettings RuntimeSettings;
};

struct FBotGoalsHorsesSeededRequest {
  FBotGoalState State;
  TArray<FHorseRouteSeed> Seeds;
  ForbocAI::Game::Data::FBotRuntimeSettings RuntimeSettings;
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
