#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

/**
 * @brief Loads an authored runtime layout from split Content JSONs.
 *
 * @signature func::Maybe<FLevelRuntimeLayoutSeed>
 * LoadRuntimeLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources)
 *
 * User story: As a level author, invalid or missing authored layout data is
 * rejected at the adapter boundary instead of becoming reducer-owned state.
 */
FLevelRuntimeLayoutSeed LoadRuntimeLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources);

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
