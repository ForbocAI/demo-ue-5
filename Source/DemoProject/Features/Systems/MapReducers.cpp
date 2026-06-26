#include "Features/Systems/MapReducers.h"

#include "Features/Systems/MapAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapReducers {

FMapTerrainState
ReduceTerrainLoaded(const FMapTerrainState &State,
                    const rtk::Action<FMapTerrainLoadedPayload> &Action) {
  FMapTerrainState Next = State;
  Next.bTerrainLoaded = true;
  Next.TerrainSource = Action.PayloadValue.TerrainSource;
  Next.OrthoSource = Action.PayloadValue.OrthoSource;
  Next.GridSize = Action.PayloadValue.GridSize;
  Next.MinElevationMeters = Action.PayloadValue.MinElevationMeters;
  Next.MaxElevationMeters = Action.PayloadValue.MaxElevationMeters;
  return Next;
}

FMapLandmarkState
ReduceLandmarksSeeded(const FMapLandmarkState &State,
                      const rtk::Action<TArray<FMapLandmark>> &Action) {
  FMapLandmarkState Next = State;
  Next.Landmarks =
      MapAdapters::LandmarkAdapter().setAll(State.Landmarks,
                                            Action.PayloadValue);
  return Next;
}

FMapSpawnState
ReducePlayerSpawnAnchored(
    const FMapSpawnState &State,
    const rtk::Action<FMapSpawnPointPayload> &Action) {
  FMapSpawnState Next = State;
  Next.PlayerSpawn = Action.PayloadValue;
  return Next;
}

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action) {
  FTownspersonState Next = State;
  Next.Townspeople = MapAdapters::TownspersonAdapter().setAll(
      State.Townspeople, Action.PayloadValue);
  return Next;
}

FMapHorseState
ReduceHorsesSeeded(const FMapHorseState &State,
                   const rtk::Action<TArray<FMapHorseRouteSeed>> &Action) {
  FMapHorseState Next = State;
  Next.Horses =
      MapAdapters::HorseAdapter().setAll(State.Horses, Action.PayloadValue);
  return Next;
}

FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::Action<TArray<FNatureFeatureSeed>> &Action) {
  FNatureState Next = State;
  Next.NatureFeatures = MapAdapters::NatureAdapter().setAll(
      State.NatureFeatures, Action.PayloadValue);
  return Next;
}

} // namespace MapReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
