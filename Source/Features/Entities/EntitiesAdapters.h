#pragma once

#include "Core/ecs.hpp"

#include "Features/Entities/Characters/Player/CharactersPlayerTypes.h"
#include "Features/Systems/Bots/SystemsBotsTypes.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

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
  const FFeatureSeed &Feature;
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

/** User Story: As a features entities consumer, I need to invoke bot entity key through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::EntityKey BotEntityKey(const FString &Id) */
ecs::EntityKey BotEntityKey(const FString &Id);
/** User Story: As a features entities consumer, I need to invoke player entity key through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::EntityKey PlayerEntityKey() */
ecs::EntityKey PlayerEntityKey();
/** User Story: As a features entities consumer, I need to invoke landmark entity key through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::EntityKey LandmarkEntityKey(const FString &Id) */
ecs::EntityKey LandmarkEntityKey(const FString &Id);
/** User Story: As a features entities consumer, I need to invoke nature entity key through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::EntityKey NatureEntityKey(const FString &Id) */
ecs::EntityKey NatureEntityKey(const FString &Id);

/** User Story: As a features entities consumer, I need to invoke project landmark through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload) */
ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload);
/** User Story: As a features entities consumer, I need to invoke project nature feature through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) */
ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload);
/** User Story: As a features entities consumer, I need to invoke project townsperson through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) */
ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload);
/** User Story: As a features entities consumer, I need to invoke project horse through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) */
ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload);
/** User Story: As a features entities consumer, I need to invoke project bot through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) */
ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload);
/** User Story: As a features entities consumer, I need to invoke project player through a stable signature so the features entities workflow remains explicit and composable. @fn ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) */
ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload);

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
