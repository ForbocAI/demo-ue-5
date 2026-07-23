#pragma once

#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @fn func::Maybe<EScaleMode> ParseScaleMode(const FLevelEnumTextRequest &Request)
 * @brief Parses an authored level scale mode.
 *
 *
 * User story: As a Level data author, scale mode text becomes a typed ECS seed
 * value before the RTK reducer composes spawn payloads.
 * User Story: As a level layout enum consumer, I need to invoke parse scale mode through a stable signature so the level layout enum workflow remains explicit and composable.
 */
func::Maybe<EScaleMode>
ParseScaleMode(const FLevelEnumTextRequest &Request);

/**
 * @fn func::Maybe<EAnchorMode> ParseAnchorMode(const FLevelEnumTextRequest &Request)
 * @brief Parses an authored level anchor mode.
 *
 *
 * User story: As a Level data author, anchor mode text is validated in the
 * adapter layer instead of view display code.
 * User Story: As a level layout enum consumer, I need to invoke parse anchor mode through a stable signature so the level layout enum workflow remains explicit and composable.
 */
func::Maybe<EAnchorMode>
ParseAnchorMode(const FLevelEnumTextRequest &Request);

/**
 * @fn func::Maybe<ELabelHeightMode> ParseLabelHeightMode(const FLevelEnumTextRequest &Request)
 * @brief Parses an authored level label height mode.
 *
 *
 * User story: As a gameplay engineer, label height semantics stay in typed ECS
 * seed data before reducers derive world placement.
 * User Story: As a level layout enum consumer, I need to invoke parse label height mode through a stable signature so the level layout enum workflow remains explicit and composable.
 */
func::Maybe<ELabelHeightMode>
ParseLabelHeightMode(const FLevelEnumTextRequest &Request);

/**
 * @fn func::Maybe<ELevelRetroTexture> ParseTexture(const FLevelEnumTextRequest &Request)
 * @brief Parses an authored level retro texture.
 *
 *
 * User story: As a rendering reducer author, texture identifiers are typed
 * before state transitions and do not leak raw JSON strings downstream.
 * User Story: As a level layout enum consumer, I need to invoke parse texture through a stable signature so the level layout enum workflow remains explicit and composable.
 */
func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelEnumTextRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
