#pragma once

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Game {
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
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
struct TRequiredJsonObjectMapper<ForbocAI::Game::Level::FLevelRuntimeLabelSeed> {
  static func::Maybe<ForbocAI::Game::Level::FLevelRuntimeLabelSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::RuntimeLayout::LabelFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
