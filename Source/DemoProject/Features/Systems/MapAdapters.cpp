#include "Features/Systems/MapAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapAdapters {

const rtk::EntityAdapterOps<FMapLandmark> &LandmarkAdapter() {
  static const rtk::EntityAdapterOps<FMapLandmark> Adapter =
      rtk::createEntityAdapter<FMapLandmark>(
          [](const FMapLandmark &Landmark) -> FString {
            return Landmark.Id;
          });
  return Adapter;
}

const rtk::EntityAdapterOps<FTownspersonSeed> &
TownspersonAdapter() {
  static const rtk::EntityAdapterOps<FTownspersonSeed> Adapter =
      rtk::createEntityAdapter<FTownspersonSeed>(
          [](const FTownspersonSeed &Townsperson) -> FString {
            return Townsperson.Id;
          });
  return Adapter;
}

const rtk::EntityAdapterOps<FMapHorseRouteSeed> &HorseAdapter() {
  static const rtk::EntityAdapterOps<FMapHorseRouteSeed> Adapter =
      rtk::createEntityAdapter<FMapHorseRouteSeed>(
          [](const FMapHorseRouteSeed &Horse) -> FString {
            return Horse.Id;
          });
  return Adapter;
}

const rtk::EntityAdapterOps<FNatureFeatureSeed> &NatureAdapter() {
  static const rtk::EntityAdapterOps<FNatureFeatureSeed> Adapter =
      rtk::createEntityAdapter<FNatureFeatureSeed>(
          [](const FNatureFeatureSeed &Feature) -> FString {
            return Feature.Id;
          });
  return Adapter;
}

} // namespace MapAdapters
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
