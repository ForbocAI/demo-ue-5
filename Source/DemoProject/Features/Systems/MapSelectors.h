#pragma once

#include "Core/functional_core.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapSelectors {

bool SelectTerrainLoaded(const FMapState &State);
TArray<FMapLandmark>
SelectLandmarks(const FMapState &State);
func::Maybe<FMapLandmark>
SelectLandmarkById(const FMapState &State, const FString &Id);
FMapSpawnPointPayload
SelectPlayerSpawn(const FMapState &State);
TArray<FTownspersonSeed>
SelectTownspeople(const FMapState &State);
TArray<FMapHorseRouteSeed>
SelectHorses(const FMapState &State);
TArray<FNatureFeatureSeed>
SelectNatureFeatures(const FMapState &State);

} // namespace MapSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
