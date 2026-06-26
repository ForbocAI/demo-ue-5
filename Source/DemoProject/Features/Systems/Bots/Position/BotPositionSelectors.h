#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace BotPositionSelectors {

TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State);
func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id);
int32 SelectTotal(const FBotPositionState &State);

} // namespace BotPositionSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
