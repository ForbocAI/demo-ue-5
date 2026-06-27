#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/AI/BotAITypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotAISelectors {

TArray<FBotAIComponent> SelectAll(const FBotAIState &State);
func::Maybe<FBotAIComponent> SelectById(const FBotAIState &State,
                                        const FString &Id);
int32 SelectTotal(const FBotAIState &State);

} // namespace BotAISelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
