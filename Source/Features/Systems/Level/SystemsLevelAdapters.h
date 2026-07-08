#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"
#include "Features/Components/Data/Settings/Level/SettingsLevelTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

/**
 * @brief Loads an authored level layout from split Content JSONs.
 *
 * @signature func::Maybe<FLevelLayoutSeed>
 * LoadLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources)
 *
 * User story: As a level author, invalid or missing authored layout data is
 * rejected at the adapter boundary instead of becoming reducer-owned state.
 */
FLevelLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources);

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
