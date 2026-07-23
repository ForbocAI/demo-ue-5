#pragma once

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"
#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @fn func::Maybe<FBlockSeed> BlockFromJson(const FLevelJsonObjectRequest &Request)
 * @brief Parses one authored level block seed object.
 *
 *
 * User story: As a Level system author, block JSON is validated into ECS seed
 * data before RTK reducers derive display-ready spawn state.
 * User Story: As a level layout block consumer, I need to invoke block from json through a stable signature so the level layout block workflow remains explicit and composable.
 */
func::Maybe<FBlockSeed>
BlockFromJson(const FLevelJsonObjectRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
struct TRequiredJsonObjectMapper<ForbocAI::Game::Level::FBlockSeed> {
  /** User Story: As a level layout block consumer, I need to invoke read through a stable signature so the level layout block workflow remains explicit and composable. @fn static func::Maybe<ForbocAI::Game::Level::FBlockSeed> Read(const TSharedPtr<FJsonObject> &Object) */
  static func::Maybe<ForbocAI::Game::Level::FBlockSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::Layout::BlockFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
