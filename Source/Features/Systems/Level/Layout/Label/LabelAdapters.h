#pragma once

#include "Features/Components/Data/Json/Value/JsonValueAdapters.h"
#include "Features/Systems/Level/Layout/Document/DocumentAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @fn func::Maybe<FLabelSeed> LabelFromJson(const FLevelJsonObjectRequest &Request)
 * @brief Parses one authored level label seed object.
 *
 *
 * User story: As a Level system author, label JSON becomes typed ECS seed data
 * before reducers compose display-ready text placement.
 * User Story: As a level layout label consumer, I need to invoke label from json through a stable signature so the level layout label workflow remains explicit and composable.
 */
func::Maybe<FLabelSeed>
LabelFromJson(const FLevelJsonObjectRequest &Request);

} // namespace Layout
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
struct TRequiredJsonObjectMapper<ForbocAI::Game::Level::FLabelSeed> {
  /** User Story: As a level layout label consumer, I need to invoke read through a stable signature so the level layout label workflow remains explicit and composable. @fn static func::Maybe<ForbocAI::Game::Level::FLabelSeed> Read(const TSharedPtr<FJsonObject> &Object) */
  static func::Maybe<ForbocAI::Game::Level::FLabelSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::Layout::LabelFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
