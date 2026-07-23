#pragma once

#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @fn func::Maybe<FVector> WorldLocationFromJson(const FLevelJsonObjectRequest &Request)
 * @brief Parses one authored world-space location object.
 *
 *
 * User story: As a Level system author, world-anchored ECS seeds can carry a
 * typed location before reducers compose spawn output.
 * User Story: As a level layout scale consumer, I need to invoke world location from json through a stable signature so the level layout scale workflow remains explicit and composable.
 */
func::Maybe<FVector>
WorldLocationFromJson(const FLevelJsonObjectRequest &Request);

/**
 * @fn func::Maybe<FScaleSeed> ScaleFromJson(const FLevelJsonObjectRequest &Request)
 * @brief Parses one authored scale seed object.
 *
 *
 * User story: As a Level system author, scale mode data is validated into a
 * typed seed before RTK reducers derive geometry.
 * User Story: As a level layout scale consumer, I need to invoke scale from json through a stable signature so the level layout scale workflow remains explicit and composable.
 */
func::Maybe<FScaleSeed>
ScaleFromJson(const FLevelJsonObjectRequest &Request);

/**
 * @fn func::Maybe<FScaleSeed> ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request)
 * @brief Reads and parses one required scale seed field.
 *
 *
 * User story: As a higher Level adapter, I can import downward into shared
 * JSON primitives while keeping scale parsing in the Level layout
 * subdomain.
 * User Story: As a level layout scale consumer, I need to invoke read scale seed through a stable signature so the level layout scale workflow remains explicit and composable.
 */
func::Maybe<FScaleSeed>
ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
