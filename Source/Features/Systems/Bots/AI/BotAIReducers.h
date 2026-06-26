#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"
#include "Features/Systems/Horses/HorseTypes.h"
#include "Features/Systems/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAIReducers {

FBotAIState ReduceBotAISeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FBotAIComponent>> &Action);
FBotAIState ReduceBotAIUpdated(const FBotAIState &State,
                               const rtk::Action<FBotAIUpdated> &Action);
FBotAIState ReduceTownspeopleSeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FBotAIState ReduceHorsesSeeded(
    const FBotAIState &State,
    const rtk::Action<TArray<FHorseRouteSeed>> &Action);

} // namespace BotAIReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
