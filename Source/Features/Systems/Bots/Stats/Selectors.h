#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Stats/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotStatsSelectors {

TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State);
func::Maybe<FBotStatsComponent> SelectById(const FBotStatsState &State,
                                           const FString &Id);
int32 SelectTotal(const FBotStatsState &State);

} // namespace BotStatsSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
