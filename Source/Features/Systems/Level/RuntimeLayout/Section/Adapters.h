#pragma once

#include "Features/Systems/Level/RuntimeLayout/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses one authored runtime section seed object.
 *
 * @signature func::Maybe<FLevelRuntimeSectionSeed> SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, section JSON becomes typed block and
 * label ECS seed arrays before reducer composition.
 */
func::Maybe<FLevelRuntimeSectionSeed>
SectionFromJson(const FLevelRuntimeJsonObjectRequest &Request);

/**
 * @brief Reads and parses one required runtime section field.
 *
 * @signature func::Maybe<FLevelRuntimeSectionSeed> ReadSection(const ForbocAI::Game::Data::FJsonFieldRequest &Request)
 *
 * User story: As a runtime layout adapter, parent layout parsing imports
 * downward into the section parser instead of sibling helper logic.
 */
func::Maybe<FLevelRuntimeSectionSeed>
ReadSection(const ForbocAI::Game::Data::FJsonFieldRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
