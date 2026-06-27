#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Position/BotPositionTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotPositionSelectors {

TArray<FBotPositionComponent> SelectAll(const FBotPositionState &State);
func::Maybe<FBotPositionComponent>
SelectById(const FBotPositionState &State, const FString &Id);
int32 SelectTotal(const FBotPositionState &State);
int32 SelectInitialPatrolIndex(const TArray<FVector> &PatrolRoute);
FBotInitialPatrolLocationPayload SelectInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request);
FBotPatrolAdvancePayload
SelectPatrolAdvance(const FBotPatrolAdvanceRequest &Request);

} // namespace BotPositionSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
