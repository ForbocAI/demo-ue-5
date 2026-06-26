#include "Features/Systems/MapActions.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapActions {

const rtk::ActionCreator<FMapTerrainLoadedPayload> &
TerrainLoaded() {
  static const rtk::ActionCreator<FMapTerrainLoadedPayload> Creator =
      rtk::createAction<FMapTerrainLoadedPayload>(
          TEXT("map/terrainLoaded"));
  return Creator;
}

const rtk::ActionCreator<TArray<FMapLandmark>> &LandmarksSeeded() {
  static const rtk::ActionCreator<TArray<FMapLandmark>> Creator =
      rtk::createAction<TArray<FMapLandmark>>(
          TEXT("map/landmarksSeeded"));
  return Creator;
}

const rtk::ActionCreator<FMapSpawnPointPayload> &
PlayerSpawnAnchored() {
  static const rtk::ActionCreator<FMapSpawnPointPayload> Creator =
      rtk::createAction<FMapSpawnPointPayload>(
          TEXT("map/playerSpawnAnchored"));
  return Creator;
}

const rtk::ActionCreator<TArray<FTownspersonSeed>> &
TownspeopleSeeded() {
  static const rtk::ActionCreator<TArray<FTownspersonSeed>> Creator =
      rtk::createAction<TArray<FTownspersonSeed>>(
          TEXT("map/townspeopleSeeded"));
  return Creator;
}

const rtk::ActionCreator<TArray<FMapHorseRouteSeed>> &HorsesSeeded() {
  static const rtk::ActionCreator<TArray<FMapHorseRouteSeed>> Creator =
      rtk::createAction<TArray<FMapHorseRouteSeed>>(
          TEXT("map/horsesSeeded"));
  return Creator;
}

const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &
NatureSeeded() {
  static const rtk::ActionCreator<TArray<FNatureFeatureSeed>>
      Creator = rtk::createAction<TArray<FNatureFeatureSeed>>(
          TEXT("map/natureSeeded"));
  return Creator;
}

} // namespace MapActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
