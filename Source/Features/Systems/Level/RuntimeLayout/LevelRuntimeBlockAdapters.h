#pragma once

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

/**
 * @brief Parses one authored runtime block seed object.
 *
 * @signature func::Maybe<FLevelRuntimeBlockSeed> BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request)
 *
 * User story: As a Level system author, block JSON is validated into ECS seed
 * data before RTK reducers derive display-ready spawn state.
 */
func::Maybe<FLevelRuntimeBlockSeed>
BlockFromJson(const FLevelRuntimeJsonObjectRequest &Request);

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
struct TRequiredJsonObjectMapper<ForbocAI::Game::Level::FLevelRuntimeBlockSeed> {
  static func::Maybe<ForbocAI::Game::Level::FLevelRuntimeBlockSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::RuntimeLayout::BlockFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
