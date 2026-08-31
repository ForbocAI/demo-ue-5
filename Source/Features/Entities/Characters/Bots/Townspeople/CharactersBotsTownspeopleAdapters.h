#pragma once

#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Bots/Townspeople/CharactersBotsTownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspeopleAdapters {

using FProjectTownspersonEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FTownspersonSeed>;

/** User Story: As a townsperson entity consumer, I need authored records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) */
ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload);

} // namespace TownspeopleAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
