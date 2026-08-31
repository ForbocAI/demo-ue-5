#pragma once

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Bots/Horses/CharactersBotsHorsesTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace HorsesAdapters {

using FProjectHorseEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FHorseRouteSeed>;

/** User Story: As a horse entity consumer, I need route records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) */
ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload);

} // namespace HorsesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
