#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapActions {

const rtk::ActionCreator<FMapTerrainLoadedPayload> &
TerrainLoaded();
const rtk::ActionCreator<TArray<FMapLandmark>> &LandmarksSeeded();
const rtk::ActionCreator<FMapSpawnPointPayload> &
PlayerSpawnAnchored();
const rtk::ActionCreator<TArray<FTownspersonSeed>> &
TownspeopleSeeded();
const rtk::ActionCreator<TArray<FMapHorseRouteSeed>> &HorsesSeeded();
const rtk::ActionCreator<TArray<FNatureFeatureSeed>> &
NatureSeeded();

} // namespace MapActions
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
