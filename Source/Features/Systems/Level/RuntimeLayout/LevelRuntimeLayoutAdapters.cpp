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
LayoutFromJson(const FLevelRuntimeLayoutRoots &Roots) {
  FLevelRuntimeLayoutSeed Seed;
  Seed = func::or_else(JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>({Seed, Roots.TerrainRoot}, JSON_REQUIRED_ATOMS(Terrain)), Seed);
  Seed = func::or_else(JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>({Seed, Roots.TownRoot}, JSON_REQUIRED_ATOMS(Town)), Seed);
  Seed = func::or_else(JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>({Seed, Roots.MineRoot}, JSON_REQUIRED_ATOMS(Mine)), Seed);
  Seed = func::or_else(JsonValues::ReadRequiredFields<FLevelRuntimeLayoutSeed>({Seed, Roots.OverlayRoot}, JSON_REQUIRED_ATOMS(OverlayLabels)), Seed);
  return func::just(Seed);
}

} // namespace RuntimeLayout
} // namespace Level
} // namespace Game
} // namespace ForbocAI
