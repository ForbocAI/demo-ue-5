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
namespace Demo {
namespace Level {
namespace BotsAdapters {

TArray<FTownspersonSeed> Build1899TownspersonSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);
TArray<FHorseRouteSeed> Build1899HorseRouteSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry);

} // namespace BotsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
