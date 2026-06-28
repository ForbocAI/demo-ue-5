#pragma once

#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses one authored runtime block seed object.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, block JSON is validated into ECS seed
 * data before RTK reducers derive display-ready spawn state.
 */
func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
