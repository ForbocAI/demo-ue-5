#pragma once

#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses the authored runtime layout root object.
 *
 * @signature func::Maybe<FLevelRuntimeLayoutSeed> LayoutFromJson(const FLevelRuntimeLayoutRoots &Roots)
 *
 * User story: As a Runtime thunk author, a full layout seed can be loaded from
 * JSON and passed into unidirectional RTK flow without view-side logic.
 */
func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const FLevelRuntimeLayoutRoots &Roots);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
