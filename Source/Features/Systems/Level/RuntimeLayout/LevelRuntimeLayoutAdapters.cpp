#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

#include "Features/Components/Data/JsonValueAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLabelAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeSectionAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonValueAdapters {

template <>
func::Maybe<ForbocAI::Game::Level::FLevelRuntimeSectionSeed>
ReadRequiredValue<ForbocAI::Game::Level::FLevelRuntimeSectionSeed>(
    const ForbocAI::Game::Data::FJsonFieldRequest &Request) {
  return ForbocAI::Game::Level::RuntimeLayout::ReadSection(Request);
}

JSON_REQUIRED_FIELD_REGISTRY(ForbocAI::Game::Level::FLevelRuntimeLayoutSeed,
                             Terrain, Town, Mine, OverlayLabels);

} // namespace JsonValueAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLayout {

namespace JsonValues = ForbocAI::Game::Data::JsonValueAdapters;

func::Maybe<FLevelRuntimeLayoutSeed>
LayoutFromJson(const FLevelRuntimeJsonObjectRequest &Request) {
  return JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>(
      Request.Object, JSON_REQUIRED_ATOMS(Terrain, Town, Mine, OverlayLabels));
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
