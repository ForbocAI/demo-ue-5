#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/Types.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/Actions.h"
#include "Features/Systems/Bots/AI/Selectors.h"
#include "Features/Systems/Bots/AI/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAISlice {

const rtk::Slice<FBotAIState> &GetSlice();

} // namespace BotAISlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/AI/Types.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotAIFactories {

FBotAIState CreateInitialState();
FBotAIComponent Component(const FBotAISource &Source);
TArray<FBotAIComponent> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotAIComponent> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotAIFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
