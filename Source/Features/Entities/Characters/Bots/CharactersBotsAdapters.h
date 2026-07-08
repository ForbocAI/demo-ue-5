#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Bots/AI/AIAdapters.h"
#include "Features/Systems/Bots/SystemsBotsAdapters.h"
#include "Features/Systems/Bots/Goals/GoalsAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesAdapters.h"
#include "Features/Systems/Bots/Horses/HorsesTypes.h"
#include "Features/Systems/Bots/Position/PositionAdapters.h"
#include "Features/Systems/Bots/Stats/StatsAdapters.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleAdapters.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

struct FBotSeedBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request);
TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request);

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
