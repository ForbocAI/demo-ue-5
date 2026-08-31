#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Entities/Characters/Bots/CharactersBotsTypes.h"
#include "Features/Entities/Characters/Bots/Horses/CharactersBotsHorsesTypes.h"
#include "Features/Entities/Characters/Bots/Townspeople/CharactersBotsTownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

struct FBotSeedBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

using FProjectBotEntityPayload =
    EntitiesAdapters::TProjectEntityPayload<FBotEntity>;

/** User Story: As a bot entity consumer, I need a stable logical key so every bot-related system projects into one ECS entity. @fn ecs::EntityKey BotEntityKey(const FString &Id) */
ecs::EntityKey BotEntityKey(const FString &Id);

/** User Story: As a bot entity consumer, I need bot records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) */
ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload);

/** User Story: As a entities characters bots consumer, I need to invoke build townsperson seed through a stable signature so the entities characters bots workflow remains explicit and composable. @fn TArray<FTownspersonSeed> BuildTownspersonSeed( const FBotSeedBuildRequest &Request) */
TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request);
/** User Story: As a entities characters bots consumer, I need to invoke build horse route seed through a stable signature so the entities characters bots workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> BuildHorseRouteSeed( const FBotSeedBuildRequest &Request) */
TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request);

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
