#pragma once

#include "Features/Systems/Level/Layout/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses an authored level scale mode.
 *
 * @signature func::Maybe<ELevelScaleMode> ParseScaleMode(const FLevelEnumTextRequest &Request)
 *
 * User story: As a Level data author, scale mode text becomes a typed ECS seed
 * value before the RTK reducer composes spawn payloads.
 */
func::Maybe<ELevelScaleMode>
ParseScaleMode(const FLevelEnumTextRequest &Request);

/**
 * @brief Parses an authored level anchor mode.
 *
 * @signature func::Maybe<ELevelAnchorMode> ParseAnchorMode(const FLevelEnumTextRequest &Request)
 *
 * User story: As a Level data author, anchor mode text is validated in the
 * adapter layer instead of view display code.
 */
func::Maybe<ELevelAnchorMode>
ParseAnchorMode(const FLevelEnumTextRequest &Request);

/**
 * @brief Parses an authored level label height mode.
 *
 * @signature func::Maybe<ELevelLabelHeightMode> ParseLabelHeightMode(const FLevelEnumTextRequest &Request)
 *
 * User story: As a gameplay engineer, label height semantics stay in typed ECS
 * seed data before reducers derive world placement.
 */
func::Maybe<ELevelLabelHeightMode>
ParseLabelHeightMode(const FLevelEnumTextRequest &Request);

/**
 * @brief Parses an authored level retro texture.
 *
 * @signature func::Maybe<ELevelRetroTexture> ParseTexture(const FLevelEnumTextRequest &Request)
 *
 * User story: As a rendering reducer author, texture identifiers are typed
 * before state transitions and do not leak raw JSON strings downstream.
 */
func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelEnumTextRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
