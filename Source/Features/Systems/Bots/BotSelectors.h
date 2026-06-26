#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/Bots/BotTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotSelectors {

TArray<FBotEntity> SelectAll(const FBotState &State);
func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id);
int32 SelectTotal(const FBotState &State);

} // namespace BotSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
