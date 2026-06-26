#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/Runtime/RuntimeTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace RuntimeSelectors {

bool SelectTerrainLoaded(const FRuntimeState &State);
TArray<FLandmark> SelectLandmarks(const FRuntimeState &State);
func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id);
FSpawnPointPayload SelectPlayerSpawn(const FRuntimeState &State);
TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State);
TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State);
TArray<FNatureFeatureSeed> SelectNatureFeatures(const FRuntimeState &State);
TArray<FBotEntity> SelectBots(const FRuntimeState &State);
func::Maybe<FBotEntity> SelectBotById(const FRuntimeState &State,
                                      const FString &Id);
TArray<FBotStatsComponent> SelectBotStats(const FRuntimeState &State);
func::Maybe<FBotStatsComponent>
SelectBotStatsById(const FRuntimeState &State, const FString &Id);
TArray<FBotPositionComponent> SelectBotPositions(
    const FRuntimeState &State);
func::Maybe<FBotPositionComponent>
SelectBotPositionById(const FRuntimeState &State, const FString &Id);
TArray<FBotAIComponent> SelectBotAI(const FRuntimeState &State);
func::Maybe<FBotAIComponent> SelectBotAIById(const FRuntimeState &State,
                                             const FString &Id);
TArray<FBotGoalComponent> SelectBotGoals(const FRuntimeState &State);
func::Maybe<FBotGoalComponent> SelectBotGoalById(
    const FRuntimeState &State, const FString &Id);
func::Maybe<FBotStrategicGoal> SelectBotActiveGoalById(
    const FRuntimeState &State, const FString &Id);

} // namespace RuntimeSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
