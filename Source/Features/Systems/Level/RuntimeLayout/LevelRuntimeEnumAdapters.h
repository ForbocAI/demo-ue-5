#pragma once

#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses an authored runtime scale mode.
 *
 * @signature func::Maybe<ELevelRuntimeScaleMode> ParseScaleMode(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a Level data author, scale mode text becomes a typed ECS seed
 * value before the RTK reducer composes spawn payloads.
 */
func::Maybe<ELevelRuntimeScaleMode>
ParseScaleMode(const FLevelRuntimeEnumTextRequest &Request);

/**
 * @brief Parses an authored runtime anchor mode.
 *
 * @signature func::Maybe<ELevelRuntimeAnchorMode> ParseAnchorMode(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a Level data author, anchor mode text is validated in the
 * adapter layer instead of view display code.
 */
func::Maybe<ELevelRuntimeAnchorMode>
ParseAnchorMode(const FLevelRuntimeEnumTextRequest &Request);

/**
 * @brief Parses an authored runtime label height mode.
 *
 * @signature func::Maybe<ELevelRuntimeLabelHeightMode> ParseLabelHeightMode(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a gameplay engineer, label height semantics stay in typed ECS
 * seed data before reducers derive world placement.
 */
func::Maybe<ELevelRuntimeLabelHeightMode>
ParseLabelHeightMode(const FLevelRuntimeEnumTextRequest &Request);

/**
 * @brief Parses an authored level retro texture.
 *
 * @signature func::Maybe<ELevelRetroTexture> ParseTexture(const FLevelRuntimeEnumTextRequest &Request)
 *
 * User story: As a rendering reducer author, texture identifiers are typed
 * before state transitions and do not leak raw JSON strings downstream.
 */
func::Maybe<ELevelRetroTexture>
ParseTexture(const FLevelRuntimeEnumTextRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
