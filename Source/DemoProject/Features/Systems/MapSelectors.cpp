#include "Features/Systems/MapSelectors.h"

#include "Features/Systems/MapAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapSelectors {

bool SelectTerrainLoaded(const FMapState &State) {
  return State.Terrain.bTerrainLoaded;
}

TArray<FMapLandmark>
SelectLandmarks(const FMapState &State) {
  return MapAdapters::LandmarkAdapter().getSelectors().selectAll(
      State.Landmarks.Landmarks);
}

func::Maybe<FMapLandmark>
SelectLandmarkById(const FMapState &State, const FString &Id) {
  return MapAdapters::LandmarkAdapter().getSelectors().selectById(
      State.Landmarks.Landmarks, Id);
}

FMapSpawnPointPayload
SelectPlayerSpawn(const FMapState &State) {
  return State.Spawn.PlayerSpawn;
}

TArray<FTownspersonSeed>
SelectTownspeople(const FMapState &State) {
  return MapAdapters::TownspersonAdapter().getSelectors().selectAll(
      State.Townspeople.Townspeople);
}

TArray<FMapHorseRouteSeed>
SelectHorses(const FMapState &State) {
  return MapAdapters::HorseAdapter().getSelectors().selectAll(
      State.Horses.Horses);
}

TArray<FNatureFeatureSeed>
SelectNatureFeatures(const FMapState &State) {
  return MapAdapters::NatureAdapter().getSelectors().selectAll(
      State.Nature.NatureFeatures);
}

} // namespace MapSelectors
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
