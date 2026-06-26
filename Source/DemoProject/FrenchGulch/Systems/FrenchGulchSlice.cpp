#include "FrenchGulch/Systems/FrenchGulchSlice.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

namespace FrenchGulchFactory {

FFrenchGulchMapState CreateInitialState() {
  FFrenchGulchMapState State;
  State.bTerrainLoaded = false;
  State.GridSize = 0;
  State.MinElevationMeters = 0.0f;
  State.MaxElevationMeters = 0.0f;
  State.PlayerSpawn = SpawnPointPayload(
      FVector::ZeroVector, FRotator::ZeroRotator, TEXT("unassigned"));
  State.Landmarks = FrenchGulchSlice::LandmarkAdapter().getInitialState();
  return State;
}

FFrenchGulchLandmark Landmark(const FString &Id, const FString &Label,
                              EFrenchGulchLandmarkKind Kind,
                              const FVector &Location,
                              const FVector &Scale) {
  FFrenchGulchLandmark Result;
  Result.Id = Id;
  Result.Label = Label;
  Result.Kind = Kind;
  Result.Location = Location;
  Result.Scale = Scale;
  return Result;
}

FFrenchGulchTerrainLoadedPayload TerrainLoadedPayload(
    const FString &TerrainSource, const FString &OrthoSource, int32 GridSize,
    float MinElevationMeters, float MaxElevationMeters) {
  FFrenchGulchTerrainLoadedPayload Payload;
  Payload.TerrainSource = TerrainSource;
  Payload.OrthoSource = OrthoSource;
  Payload.GridSize = GridSize;
  Payload.MinElevationMeters = MinElevationMeters;
  Payload.MaxElevationMeters = MaxElevationMeters;
  return Payload;
}

FFrenchGulchSpawnPointPayload SpawnPointPayload(const FVector &Location,
                                                const FRotator &Rotation,
                                                const FString &AnchorLabel) {
  FFrenchGulchSpawnPointPayload Payload;
  Payload.Location = Location;
  Payload.Rotation = Rotation;
  Payload.AnchorLabel = AnchorLabel;
  return Payload;
}

} // namespace FrenchGulchFactory

namespace FrenchGulchReducers {

FFrenchGulchMapState
ReduceTerrainLoaded(const FFrenchGulchMapState &State,
                    const rtk::Action<FFrenchGulchTerrainLoadedPayload> &Action) {
  FFrenchGulchMapState Next = State;
  Next.bTerrainLoaded = true;
  Next.TerrainSource = Action.PayloadValue.TerrainSource;
  Next.OrthoSource = Action.PayloadValue.OrthoSource;
  Next.GridSize = Action.PayloadValue.GridSize;
  Next.MinElevationMeters = Action.PayloadValue.MinElevationMeters;
  Next.MaxElevationMeters = Action.PayloadValue.MaxElevationMeters;
  return Next;
}

FFrenchGulchMapState
ReduceLandmarksSeeded(const FFrenchGulchMapState &State,
                      const rtk::Action<TArray<FFrenchGulchLandmark>> &Action) {
  FFrenchGulchMapState Next = State;
  Next.Landmarks =
      FrenchGulchSlice::LandmarkAdapter().setAll(State.Landmarks,
                                                 Action.PayloadValue);
  return Next;
}

FFrenchGulchMapState
ReducePlayerSpawnAnchored(
    const FFrenchGulchMapState &State,
    const rtk::Action<FFrenchGulchSpawnPointPayload> &Action) {
  FFrenchGulchMapState Next = State;
  Next.PlayerSpawn = Action.PayloadValue;
  return Next;
}

} // namespace FrenchGulchReducers

namespace FrenchGulchSlice {

const rtk::EntityAdapterOps<FFrenchGulchLandmark> &LandmarkAdapter() {
  static const rtk::EntityAdapterOps<FFrenchGulchLandmark> Adapter =
      rtk::createEntityAdapter<FFrenchGulchLandmark>(
          [](const FFrenchGulchLandmark &Landmark) -> FString {
            return Landmark.Id;
          });
  return Adapter;
}

const rtk::ActionCreator<FFrenchGulchTerrainLoadedPayload> &
TerrainLoadedAction() {
  static const rtk::ActionCreator<FFrenchGulchTerrainLoadedPayload> Creator =
      rtk::createAction<FFrenchGulchTerrainLoadedPayload>(
          TEXT("frenchGulch/terrainLoaded"));
  return Creator;
}

const rtk::ActionCreator<TArray<FFrenchGulchLandmark>> &
LandmarksSeededAction() {
  static const rtk::ActionCreator<TArray<FFrenchGulchLandmark>> Creator =
      rtk::createAction<TArray<FFrenchGulchLandmark>>(
          TEXT("frenchGulch/landmarksSeeded"));
  return Creator;
}

const rtk::ActionCreator<FFrenchGulchSpawnPointPayload> &
PlayerSpawnAnchoredAction() {
  static const rtk::ActionCreator<FFrenchGulchSpawnPointPayload> Creator =
      rtk::createAction<FFrenchGulchSpawnPointPayload>(
          TEXT("frenchGulch/playerSpawnAnchored"));
  return Creator;
}

const rtk::Slice<FFrenchGulchMapState> &GetSlice() {
  static const rtk::Slice<FFrenchGulchMapState> Slice = []() {
    rtk::SliceBuilder<FFrenchGulchMapState> Builder =
        rtk::sliceBuilder<FFrenchGulchMapState>(
            TEXT("frenchGulch"), FrenchGulchFactory::CreateInitialState());

    rtk::createCase<FFrenchGulchTerrainLoadedPayload>(
        Builder, TEXT("terrainLoaded"),
        FrenchGulchReducers::ReduceTerrainLoaded);
    rtk::createCase<TArray<FFrenchGulchLandmark>>(
        Builder, TEXT("landmarksSeeded"),
        FrenchGulchReducers::ReduceLandmarksSeeded);
    rtk::createCase<FFrenchGulchSpawnPointPayload>(
        Builder, TEXT("playerSpawnAnchored"),
        FrenchGulchReducers::ReducePlayerSpawnAnchored);

    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

rtk::EnhancedStore<FFrenchGulchMapState> ConfigureStore() {
  return rtk::configureStore<FFrenchGulchMapState>(
      GetSlice().Reducer, FrenchGulchFactory::CreateInitialState());
}

} // namespace FrenchGulchSlice

namespace FrenchGulchSelectors {

bool SelectTerrainLoaded(const FFrenchGulchMapState &State) {
  return State.bTerrainLoaded;
}

TArray<FFrenchGulchLandmark>
SelectLandmarks(const FFrenchGulchMapState &State) {
  return FrenchGulchSlice::LandmarkAdapter().getSelectors().selectAll(
      State.Landmarks);
}

func::Maybe<FFrenchGulchLandmark>
SelectLandmarkById(const FFrenchGulchMapState &State, const FString &Id) {
  return FrenchGulchSlice::LandmarkAdapter().getSelectors().selectById(
      State.Landmarks, Id);
}

FFrenchGulchSpawnPointPayload
SelectPlayerSpawn(const FFrenchGulchMapState &State) {
  return State.PlayerSpawn;
}

} // namespace FrenchGulchSelectors

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
