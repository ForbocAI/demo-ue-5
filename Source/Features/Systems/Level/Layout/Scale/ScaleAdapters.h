#pragma once

#include "Features/Systems/Level/Layout/SystemsLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses one authored world-space location object.
 *
 * @signature func::Maybe<FVector> WorldLocationFromJson(const FLevelJsonObjectRequest &Request)
 *
 * User story: As a Level system author, world-anchored ECS seeds can carry a
 * typed location before reducers compose spawn output.
 */
func::Maybe<FVector>
WorldLocationFromJson(const FLevelJsonObjectRequest &Request);

/**
 * @brief Parses one authored scale seed object.
 *
 * @signature func::Maybe<FScaleSeed> ScaleFromJson(const FLevelJsonObjectRequest &Request)
 *
 * User story: As a Level system author, scale mode data is validated into a
 * typed seed before RTK reducers derive geometry.
 */
func::Maybe<FScaleSeed>
ScaleFromJson(const FLevelJsonObjectRequest &Request);

/**
 * @brief Reads and parses one required scale seed field.
 *
 * @signature func::Maybe<FScaleSeed> ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request)
 *
 * User story: As a higher Level adapter, I can import downward into shared
 * JSON primitives while keeping scale parsing in the Level layout
 * subdomain.
 */
func::Maybe<FScaleSeed>
ReadScaleSeed(const ForbocAI::Game::Data::FFieldRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
