#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/BotTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotSelectors {

TArray<FBotEntity> SelectAll(const FBotState &State);
func::Maybe<FBotEntity> SelectById(const FBotState &State, const FString &Id);
int32 SelectTotal(const FBotState &State);

} // namespace BotSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
