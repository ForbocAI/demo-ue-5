#pragma once

#include "Features/Components/Data/Json/Value/ValueAdapters.h"
#include "Features/Systems/Level/Layout/SystemsLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses one authored level block seed object.
 *
 * @signature func::Maybe<FBlockSeed> BlockFromJson(const FLevelJsonObjectRequest &Request)
 *
 * User story: As a Level system author, block JSON is validated into ECS seed
 * data before RTK reducers derive display-ready spawn state.
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
  static func::Maybe<ForbocAI::Game::Level::FBlockSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::Layout::BlockFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
