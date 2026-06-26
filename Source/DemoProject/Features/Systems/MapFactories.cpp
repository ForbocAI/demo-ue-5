#include "Features/Systems/MapFactories.h"

#include "Features/Systems/MapAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapFactories {

FMapTerrainState CreateTerrainState() {
  FMapTerrainState State;
  State.bTerrainLoaded = false;
  State.GridSize = 0;
  State.MinElevationMeters = 0.0f;
  State.MaxElevationMeters = 0.0f;
  return State;
}

FMapSpawnState CreateSpawnState() {
  FMapSpawnState State;
  State.PlayerSpawn =
      SpawnPointPayload({FVector::ZeroVector, FRotator::ZeroRotator,
                         TEXT("unassigned")});
  return State;
}

FMapLandmarkState CreateLandmarkState() {
  FMapLandmarkState State;
  State.Landmarks = MapAdapters::LandmarkAdapter().getInitialState();
  return State;
}

FTownspersonState CreateTownspersonState() {
  FTownspersonState State;
  State.Townspeople =
      MapAdapters::TownspersonAdapter().getInitialState();
  return State;
}

FMapHorseState CreateHorseState() {
  FMapHorseState State;
  State.Horses = MapAdapters::HorseAdapter().getInitialState();
  return State;
}

FNatureState CreateNatureState() {
  FNatureState State;
  State.NatureFeatures =
      MapAdapters::NatureAdapter().getInitialState();
  return State;
}

FMapState CreateInitialState() {
  FMapState State;
  State.Terrain = CreateTerrainState();
  State.Spawn = CreateSpawnState();
  State.Landmarks = CreateLandmarkState();
  State.Townspeople = CreateTownspersonState();
  State.Horses = CreateHorseState();
  State.Nature = CreateNatureState();
  return State;
}

FMapLandmark Landmark(const FMapLandmarkSource &Source) {
  FMapLandmark Result;
  Result.Id = Source.Id;
  Result.Label = Source.Label;
  Result.Kind = Source.Kind;
  Result.Location = Source.Location;
  Result.Scale = Source.Scale;
  return Result;
}

FMapTerrainLoadedPayload
TerrainLoadedPayload(const FMapTerrainLoadedSource &Source) {
  FMapTerrainLoadedPayload Payload;
  Payload.TerrainSource = Source.TerrainSource;
  Payload.OrthoSource = Source.OrthoSource;
  Payload.GridSize = Source.GridSize;
  Payload.MinElevationMeters = Source.MinElevationMeters;
  Payload.MaxElevationMeters = Source.MaxElevationMeters;
  return Payload;
}

FMapSpawnPointPayload
SpawnPointPayload(const FMapSpawnPointSource &Source) {
  FMapSpawnPointPayload Payload;
  Payload.Location = Source.Location;
  Payload.Rotation = Source.Rotation;
  Payload.AnchorLabel = Source.AnchorLabel;
  return Payload;
}

} // namespace MapFactories
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
