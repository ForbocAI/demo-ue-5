#pragma once

#include "Features/Components/Data/Json/Value/ValueAdapters.h"
#include "Features/Systems/Level/Layout/SystemsLevelLayoutTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Layout {

/**
 * @brief Parses one authored level label seed object.
 *
 * @signature func::Maybe<FLevelLabelSeed> LabelFromJson(const FLevelJsonObjectRequest &Request)
 *
 * User story: As a Level system author, label JSON becomes typed ECS seed data
 * before reducers compose display-ready text placement.
 */
func::Maybe<FLevelLabelSeed>
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
struct TRequiredJsonObjectMapper<ForbocAI::Game::Level::FLevelLabelSeed> {
  static func::Maybe<ForbocAI::Game::Level::FLevelLabelSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Game::Level::Layout::LabelFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
