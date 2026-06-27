#pragma once

#include "Core/ecs.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace EntitiesAdapters {

ecs::EntityKey BotEntityKey(const FString &Id);
ecs::EntityKey PlayerEntityKey();
ecs::EntityKey LandmarkEntityKey(const FString &Id);
ecs::EntityKey NatureEntityKey(const FString &Id);

ecs::FWorld ProjectLandmark(ecs::FWorld World, const FLandmark &Landmark);
ecs::FWorld ProjectNatureFeature(ecs::FWorld World,
                                 const FNatureFeatureSeed &Feature);
ecs::FWorld ProjectTownsperson(ecs::FWorld World,
                               const FTownspersonSeed &Townsperson);
ecs::FWorld ProjectHorse(ecs::FWorld World, const FHorseRouteSeed &Horse);
ecs::FWorld ProjectBot(ecs::FWorld World, const FBotEntity &Bot);
ecs::FWorld ProjectPlayer(ecs::FWorld World, const FPlayerState &Player);

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
