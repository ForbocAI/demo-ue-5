#pragma once

#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses one authored runtime label seed object.
 *
 * @signature func::Maybe<FLevelRuntimeLabelSeed> LabelFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, label JSON becomes typed ECS seed data
 * before reducers compose display-ready text placement.
 */
func::Maybe<FLevelRuntimeLabelSeed>
LabelFromJson(const FLevelRuntimeJsonObjectRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
