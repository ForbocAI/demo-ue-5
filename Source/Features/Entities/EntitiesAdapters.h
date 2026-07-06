#pragma once

#include "Core/ecs.hpp"

#include "Features/Entities/Characters/Player/PlayerTypes.h"
#include "Features/Systems/Bots/BotTypes.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace EntitiesAdapters {

struct FProjectLandmarkEntityPayload {
  ecs::FWorld World;
  const FLandmark &Landmark;
};

struct FProjectNatureFeatureEntityPayload {
  ecs::FWorld World;
  const FNatureFeatureSeed &Feature;
};

struct FProjectTownspersonEntityPayload {
  ecs::FWorld World;
  const FTownspersonSeed &Townsperson;
};

struct FProjectHorseEntityPayload {
  ecs::FWorld World;
  const FHorseRouteSeed &Horse;
};

struct FProjectBotEntityPayload {
  ecs::FWorld World;
  const FBotEntity &Bot;
};

struct FProjectPlayerEntityPayload {
  ecs::FWorld World;
  const FPlayerState &Player;
};

ecs::EntityKey BotEntityKey(const FString &Id);
ecs::EntityKey PlayerEntityKey();
ecs::EntityKey LandmarkEntityKey(const FString &Id);
ecs::EntityKey NatureEntityKey(const FString &Id);

ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload);
ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload);
ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload);
ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload);
ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload);
ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload);

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
