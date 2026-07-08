#pragma once

#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses authored level layout leaf arrays.
 *
 * @signature func::Maybe<FLayoutSeed> LayoutFromJson(const FLeaves &Leaves)
 *
 * User story: As a Level thunk author, atomic JSON leaves compose into one
 * layout seed before entering unidirectional RTK flow.
 */
func::Maybe<FLayoutSeed>
LayoutFromJson(const FLeaves &Leaves);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
