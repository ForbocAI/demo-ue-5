#pragma once

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace PlayerAdapters {

using FProjectPlayerEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FPlayerState>;

/** User Story: As a player entity consumer, I need the stable local-player key through one domain-owned adapter so every projection addresses the same ECS entity. @fn ecs::EntityKey PlayerEntityKey() */
ecs::EntityKey PlayerEntityKey();

/** User Story: As a player entity consumer, I need player state projected through its domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) */
ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload);

} // namespace PlayerAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
