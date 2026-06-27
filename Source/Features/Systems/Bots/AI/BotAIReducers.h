#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAIReducers {

FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FBotAIComponent>> &Action);
FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::PayloadAction<FBotAIUpdated> &Action);
FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotAIReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
