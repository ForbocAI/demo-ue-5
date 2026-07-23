#pragma once

#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @fn func::Maybe<FLayoutSeed> LayoutFromJson(const FLeaves &Leaves)
 * @brief Parses authored level layout leaf arrays.
 *
 *
 * User story: As a Level thunk author, atomic JSON leaves compose into one
 * layout seed before entering unidirectional RTK flow.
 * User Story: As a systems level layout consumer, I need to invoke layout from json through a stable signature so the systems level layout workflow remains explicit and composable.
 */
func::Maybe<FLayoutSeed>
LayoutFromJson(const FLeaves &Leaves);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
