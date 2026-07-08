#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TownspersonSelectors {

TArray<FTownspersonSeed> SelectAll(const FTownspersonState &State);
func::Maybe<FTownspersonSeed> SelectById(const FTownspersonState &State,
                                         const FString &Id);
const TArray<FTownspersonSeed> &
SelectGeneralTownspeople(const FTownspersonState &State);
const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FTownspersonState &State);
const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FTownspersonState &State);
const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FTownspersonState &State);
const FTownspersonViewDefaults &
SelectViewDefaults(const FTownspersonState &State);

} // namespace TownspersonSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
