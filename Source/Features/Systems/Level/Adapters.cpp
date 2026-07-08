#include "Features/Systems/Level/Adapters.h"

#include "Features/Components/Data/Json/Read/Adapters.h"
#include "Features/Systems/Level/RuntimeLayout/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

FLevelRuntimeLayoutSeed LoadRuntimeLayoutSeed(const ForbocAI::Game::Data::FLevelDataSourceSettings &DataSources) {
  const TSharedPtr<FJsonObject> TerrainRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.TerrainJsonPath});
  const TSharedPtr<FJsonObject> TownRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.TownJsonPath});
  const TSharedPtr<FJsonObject> MineRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.MineJsonPath});
  const TSharedPtr<FJsonObject> OverlayRoot =
      JsonAdapters::LoadRequiredObjectFromContent({DataSources.OverlayLabelsJsonPath});

  const func::Maybe<FLevelRuntimeLayoutSeed> Layout =
      RuntimeLayout::LayoutFromJson({TerrainRoot, TownRoot, MineRoot, OverlayRoot});
  checkf(Layout.hasValue, TEXT("Runtime layout JSON is invalid"));
  return Layout.value;
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
