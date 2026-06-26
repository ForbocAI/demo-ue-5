#pragma once

#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapFactories {

FMapTerrainState CreateTerrainState();
FMapSpawnState CreateSpawnState();
FMapLandmarkState CreateLandmarkState();
FTownspersonState CreateTownspersonState();
FMapHorseState CreateHorseState();
FNatureState CreateNatureState();
FMapState CreateInitialState();
FMapLandmark Landmark(const FMapLandmarkSource &Source);
FMapTerrainLoadedPayload
TerrainLoadedPayload(const FMapTerrainLoadedSource &Source);
FMapSpawnPointPayload
SpawnPointPayload(const FMapSpawnPointSource &Source);

} // namespace MapFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
