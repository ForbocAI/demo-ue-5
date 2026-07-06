#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Bots/AI/BotAIAdapters.h"
#include "Features/Systems/Bots/BotAdapters.h"
#include "Features/Systems/Bots/Goals/BotGoalAdapters.h"
#include "Features/Systems/Bots/Horses/HorseAdapters.h"
#include "Features/Systems/Bots/Horses/HorseTypes.h"
#include "Features/Systems/Bots/Position/BotPositionAdapters.h"
#include "Features/Systems/Bots/Stats/BotStatsAdapters.h"
#include "Features/Systems/Bots/Townspeople/TownspersonAdapters.h"
#include "Features/Systems/Bots/Townspeople/TownspersonTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

struct FBotSeedBuildRequest {
  FString RelativeJsonPath;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request);
TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request);

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
