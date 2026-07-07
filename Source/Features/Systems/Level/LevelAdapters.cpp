#include "Features/Systems/Level/LevelAdapters.h"

#include "Features/Components/Data/Json/JsonReadAdapters.h"
#include "Features/Systems/Level/RuntimeLayout/LevelRuntimeLayoutAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

FLevelRuntimeLayoutSeed LoadRuntimeLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources) {
  const TSharedPtr<FJsonObject> TerrainRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.RuntimeTerrainJsonPath});
  const TSharedPtr<FJsonObject> TownRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.RuntimeTownJsonPath});
  const TSharedPtr<FJsonObject> MineRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.RuntimeMineJsonPath});
  const TSharedPtr<FJsonObject> OverlayRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.RuntimeOverlayLabelsJsonPath});

  const func::Maybe<FLevelRuntimeLayoutSeed> Layout =
      RuntimeLayout::LayoutFromJson({TerrainRoot, TownRoot, MineRoot, OverlayRoot});
  checkf(Layout.hasValue, TEXT("Runtime layout JSON is invalid"));
  return Layout.value;
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
