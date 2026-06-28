#pragma once

#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses one authored world-space location object.
 *
 * @signature func::Maybe<FVector> WorldLocationFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, world-anchored ECS seeds can carry a
 * typed location before reducers compose spawn output.
 */
func::Maybe<FVector>
WorldLocationFromJson(const FLevelRuntimeJsonObjectRequest &Request);

/**
 * @brief Parses one authored scale seed object.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ScaleFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, scale mode data is validated into a
 * typed seed before RTK reducers derive geometry.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ScaleFromJson(const FLevelRuntimeJsonObjectRequest &Request);

/**
 * @brief Reads and parses one required scale seed field.
 *
 * @signature func::Maybe<FLevelRuntimeScaleSeed> ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request)
 *
 * User story: As a higher Level adapter, I can import downward into shared
 * JSON primitives while keeping scale parsing in the Level runtime-layout
 * subdomain.
 */
func::Maybe<FLevelRuntimeScaleSeed>
ReadScaleSeed(const ForbocAI::Demo::Data::FJsonFieldRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
