#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TownspersonSelectors {

TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State);
func::Maybe<FTownspersonSeed> SelectById(const FTownspersonState &State,
                                         const FString &Id);
TArray<FTownspersonSeed> SelectByInteractionIntent(
    const FTownspersonState &State, ETownspersonInteractionIntent Intent);
FTownspersonViewDefaults SelectViewDefaults(
    const FTownspersonViewDefaultsRequest &Request);
FTownspersonInteractionOverlapViewModel SelectInteractionOverlap(
    const FTownspersonInteractionOverlapRequest &Request);

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
