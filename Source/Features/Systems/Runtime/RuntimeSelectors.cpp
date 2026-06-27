#include "Features/Systems/Runtime/RuntimeSelectors.h"

#include "Features/Systems/Bots/AI/BotAISelectors.h"
#include "Features/Systems/Bots/BotSelectors.h"
#include "Features/Systems/Bots/Goals/BotGoalSelectors.h"
#include "Features/Systems/Bots/Position/BotPositionSelectors.h"
#include "Features/Systems/Bots/Stats/BotStatsSelectors.h"
#include "Features/Systems/Bots/Horses/HorseSelectors.h"
#include "Features/Systems/Landmarks/LandmarkSelectors.h"
#include "Features/Systems/Nature/NatureSelectors.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeSelectors {

bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
}

TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) {
  return LandmarkSelectors::SelectAll(State.Landmarks);
}

func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id) {
  return LandmarkSelectors::SelectById(State.Landmarks, Id);
}

FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State) {
  return SpawnSelectors::SelectPlayerSpawn(State.Spawn);
}

TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectAll(State.Townspeople);
}

func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id) {
  return TownspersonSelectors::SelectById(State.Townspeople, Id);
}

TArray<FTownspersonSeed> SelectTownspeopleByInteractionIntent(
    const FRuntimeState &State, ETownspersonInteractionIntent Intent) {
  return TownspersonSelectors::SelectByInteractionIntent(State.Townspeople,
                                                        Intent);
}

TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

TArray<FBotEntity> SelectBots(const FRuntimeState &State) {
  return BotSelectors::SelectAll(State.Bots);
}

func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id) {
  return BotSelectors::SelectById(State.Bots, Id);
}

TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State) {
  return BotStatsSelectors::SelectAll(State.BotStats);
}

func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id) {
  return BotStatsSelectors::SelectById(State.BotStats, Id);
}

TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State) {
  return BotPositionSelectors::SelectAll(State.BotPosition);
}

func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id) {
  return BotPositionSelectors::SelectById(State.BotPosition, Id);
}

TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State) {
  return BotAISelectors::SelectAll(State.BotAI);
}

func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id) {
  return BotAISelectors::SelectById(State.BotAI, Id);
}

TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State) {
  return BotGoalSelectors::SelectAll(State.BotGoals);
}

func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectById(State.BotGoals, Id);
}

func::Maybe<FBotStrategicGoal> SelectBotActiveGoalById(
    const FRuntimeState &State, const FString &Id) {
  return BotGoalSelectors::SelectActiveGoalById(State.BotGoals, Id);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
