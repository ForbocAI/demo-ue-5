#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotAISelectors {

TArray<FBotAIComponent> SelectAll(const FBotAIState &State);
func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State,
                                        const FString &Id);
int32 SelectTotal(const FBotAIState &State);

} // namespace BotAISelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
