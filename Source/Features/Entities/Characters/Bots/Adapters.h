#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/Types.h"
#include "Features/Systems/Bots/AI/Adapters.h"
#include "Features/Systems/Bots/Adapters.h"
#include "Features/Systems/Bots/Goals/Adapters.h"
#include "Features/Systems/Bots/Horses/Adapters.h"
#include "Features/Systems/Bots/Horses/Types.h"
#include "Features/Systems/Bots/Position/Adapters.h"
#include "Features/Systems/Bots/Stats/Adapters.h"
#include "Features/Systems/Bots/Townspeople/Adapters.h"
#include "Features/Systems/Bots/Townspeople/Types.h"

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
