#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/SystemsLevelTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/GeometryTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

/**
 * @fn FLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources)
 * @brief Loads an authored level layout from split Content JSONs.
 *
 * LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources)
 *
 * User story: As a level author, invalid or missing authored layout data is
 * rejected at the adapter boundary instead of becoming reducer-owned state.
 * User Story: As a features systems level consumer, I need to invoke load layout seed through a stable signature so the features systems level workflow remains explicit and composable.
 */
FLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources);

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
