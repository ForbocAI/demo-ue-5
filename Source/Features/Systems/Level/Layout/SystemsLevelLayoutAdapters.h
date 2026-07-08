#pragma once

#include "Features/Systems/Level/Layout/SystemsLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses authored level layout leaf arrays.
 *
 * @signature func::Maybe<FLevelLayoutSeed> LayoutFromJson(const FLevelLayoutLeaves &Leaves)
 *
 * User story: As a Level thunk author, atomic JSON leaves compose into one
 * layout seed before entering unidirectional RTK flow.
 */
func::Maybe<FLevelLayoutSeed>
LayoutFromJson(const FLevelLayoutLeaves &Leaves);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
