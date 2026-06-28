#pragma once

#include "Core/rtk.hpp"

#include "Features/Systems/Level/LevelTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace LevelAdapters {

/**
 * @brief Loads an authored runtime layout from Content JSON.
 *
 * @signature func::Maybe<FLevelRuntimeLayoutSeed>
 * LoadRuntimeLayoutSeed(const FString &RelativeJsonPath)
 *
 * User story: As a level author, invalid or missing authored layout data is
 * rejected at the adapter boundary instead of becoming reducer-owned state.
 */
FLevelRuntimeLayoutSeed LoadRuntimeLayoutSeed(const FString &RelativeJsonPath);

} // namespace LevelAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
