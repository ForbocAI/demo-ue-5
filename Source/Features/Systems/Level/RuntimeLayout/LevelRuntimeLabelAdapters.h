#pragma once

#include "Features/Components/Data/JsonValueAdapters.h"
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

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace JsonValueAdapters {

template <>
struct TRequiredJsonObjectMapper<ForbocAI::Demo::Level::FLevelRuntimeLabelSeed> {
  static func::Maybe<ForbocAI::Demo::Level::FLevelRuntimeLabelSeed>
  Read(const TSharedPtr<FJsonObject> &Object) {
    return ForbocAI::Demo::Level::RuntimeLayout::LabelFromJson({Object});
  }
};

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
