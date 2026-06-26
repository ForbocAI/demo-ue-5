#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/MapTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace MapReducers {

FMapTerrainState ReduceTerrainLoaded(
    const FMapTerrainState &State,
    const rtk::Action<FMapTerrainLoadedPayload> &Action);
FMapLandmarkState ReduceLandmarksSeeded(
    const FMapLandmarkState &State,
    const rtk::Action<TArray<FMapLandmark>> &Action);
FMapSpawnState ReducePlayerSpawnAnchored(
    const FMapSpawnState &State,
    const rtk::Action<FMapSpawnPointPayload> &Action);
FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::Action<TArray<FTownspersonSeed>> &Action);
FMapHorseState ReduceHorsesSeeded(
    const FMapHorseState &State,
    const rtk::Action<TArray<FMapHorseRouteSeed>> &Action);
FNatureState ReduceNatureSeeded(
    const FNatureState &State,
    const rtk::Action<TArray<FNatureFeatureSeed>> &Action);

} // namespace MapReducers
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
