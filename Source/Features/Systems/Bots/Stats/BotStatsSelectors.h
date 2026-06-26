#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/Bots/Stats/BotStatsTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotStatsSelectors {

TArray<FBotStatsComponent> SelectAll(const FBotStatsState &State);
func::Maybe<FBotStatsComponent> SelectById(const FBotStatsState &State,
                                           const FString &Id);
int32 SelectTotal(const FBotStatsState &State);

} // namespace BotStatsSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
