#pragma once

#include "Core/rtk.hpp"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonReducers {

FTownspersonState ReduceTownspeopleSeeded(
    const FTownspersonState &State,
    const rtk::PayloadAction<TArray<FTownspersonSeed>> &Action);
FTownspersonState ReduceViewDefaultsRequested(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonViewDefaultsRequest> &Action);
FTownspersonState ReduceInteractionOverlapObserved(
    const FTownspersonState &State,
    const rtk::PayloadAction<FTownspersonInteractionOverlapRequest> &Action);
TArray<FTownspersonSeed> ReduceByInteractionIntent(
    const FTownspersonIntentProjectionRequest &Request);
FTownspersonViewDefaults ReduceViewDefaults(
    const FTownspersonViewDefaultsRequest &Request);
FTownspersonInteractionOverlapViewModel ReduceInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);

} // namespace TownspersonReducers
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
