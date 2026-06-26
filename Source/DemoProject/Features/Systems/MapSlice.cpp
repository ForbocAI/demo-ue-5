#include "Features/Systems/MapSlice.h"

#include "Features/Systems/MapActions.h"
#include "Features/Systems/MapFactories.h"
#include "Features/Systems/MapReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapSlice {
namespace {

const rtk::Slice<FMapTerrainState> &TerrainSlice() {
  static const rtk::Slice<FMapTerrainState> Slice = []() {
    rtk::SliceBuilder<FMapTerrainState> Builder =
        rtk::sliceBuilder<FMapTerrainState>(
            TEXT("mapTerrain"), MapFactories::CreateTerrainState());
    Builder = rtk::addExtraCase(Builder, MapActions::TerrainLoaded(),
                                MapReducers::ReduceTerrainLoaded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

const rtk::Slice<FMapLandmarkState> &LandmarkSlice() {
  static const rtk::Slice<FMapLandmarkState> Slice = []() {
    rtk::SliceBuilder<FMapLandmarkState> Builder =
        rtk::sliceBuilder<FMapLandmarkState>(
            TEXT("mapLandmarks"), MapFactories::CreateLandmarkState());
    Builder = rtk::addExtraCase(Builder, MapActions::LandmarksSeeded(),
                                MapReducers::ReduceLandmarksSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

const rtk::Slice<FMapSpawnState> &SpawnSlice() {
  static const rtk::Slice<FMapSpawnState> Slice = []() {
    rtk::SliceBuilder<FMapSpawnState> Builder =
        rtk::sliceBuilder<FMapSpawnState>(
            TEXT("mapSpawn"), MapFactories::CreateSpawnState());
    Builder = rtk::addExtraCase(Builder, MapActions::PlayerSpawnAnchored(),
                                MapReducers::ReducePlayerSpawnAnchored);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

const rtk::Slice<FTownspersonState> &TownspersonSlice() {
  static const rtk::Slice<FTownspersonState> Slice = []() {
    rtk::SliceBuilder<FTownspersonState> Builder =
        rtk::sliceBuilder<FTownspersonState>(
            TEXT("mapTownspeople"), MapFactories::CreateTownspersonState());
    Builder = rtk::addExtraCase(Builder, MapActions::TownspeopleSeeded(),
                                MapReducers::ReduceTownspeopleSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

const rtk::Slice<FMapHorseState> &HorseSlice() {
  static const rtk::Slice<FMapHorseState> Slice = []() {
    rtk::SliceBuilder<FMapHorseState> Builder =
        rtk::sliceBuilder<FMapHorseState>(
            TEXT("mapHorses"), MapFactories::CreateHorseState());
    Builder = rtk::addExtraCase(Builder, MapActions::HorsesSeeded(),
                                MapReducers::ReduceHorsesSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

const rtk::Slice<FNatureState> &NatureSlice() {
  static const rtk::Slice<FNatureState> Slice = []() {
    rtk::SliceBuilder<FNatureState> Builder =
        rtk::sliceBuilder<FNatureState>(
            TEXT("mapNature"), MapFactories::CreateNatureState());
    Builder = rtk::addExtraCase(Builder, MapActions::NatureSeeded(),
                                MapReducers::ReduceNatureSeeded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

FMapState ReduceMapState(const FMapState &State,
                         const rtk::AnyAction &Action) {
  FMapState Next = State;
  Next.Terrain = TerrainSlice().Reducer(State.Terrain, Action);
  Next.Spawn = SpawnSlice().Reducer(State.Spawn, Action);
  Next.Landmarks = LandmarkSlice().Reducer(State.Landmarks, Action);
  Next.Townspeople =
      TownspersonSlice().Reducer(State.Townspeople, Action);
  Next.Horses = HorseSlice().Reducer(State.Horses, Action);
  Next.Nature = NatureSlice().Reducer(State.Nature, Action);
  return Next;
}

} // namespace

const rtk::Slice<FMapState> &GetSlice() {
  static const rtk::Slice<FMapState> Slice{TEXT("map"), ReduceMapState};
  return Slice;
}

rtk::EnhancedStore<FMapState> ConfigureStore() {
  return rtk::configureStore<FMapState>(
      GetSlice().Reducer, MapFactories::CreateInitialState());
}

} // namespace MapSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
