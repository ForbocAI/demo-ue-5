#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotPositionSelectors {

TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State);
func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id);
int32 SelectTotal(const FBotPositionState &State);

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
