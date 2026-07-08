#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Types.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotReducers {

FBotState ReduceBotsSeeded(const FBotState &State,
                           const rtk::PayloadAction<TArray<FBotEntity>> &Action);
FBotState ReduceBotUpserted(const FBotState &State,
                            const rtk::PayloadAction<FBotEntity> &Action);
FBotState ReduceTownspeopleSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FBotState ReduceHorsesSeeded(
    const FBotState &State,
    const rtk::PayloadAction<TArray<FHorseRouteSeed>> &Action);

} // namespace BotReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/AI/Slice.h"
#include "Features/Systems/Bots/Actions.h"
#include "Features/Systems/Bots/Selectors.h"
#include "Features/Systems/Bots/Types.h"
#include "Features/Systems/Bots/Goals/Slice.h"
#include "Features/Systems/Bots/Position/Slice.h"
#include "Features/Systems/Bots/Stats/Slice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSlice {

const rtk::Slice<FBotState> &GetSlice();

} // namespace BotSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Bots/Types.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotFactories {

FBotState CreateInitialState();
FBotEntity Bot(const FBotEntitySource &Source);
TArray<FBotEntity> FromTownspeople(const TArray<FTownspersonSeed> &Seeds);
TArray<FBotEntity> FromHorses(const TArray<FHorseRouteSeed> &Seeds);

} // namespace BotFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
