#include "Features/Systems/Level/SystemsLevelAdapters.h"

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Features/Systems/Level/Layout/SystemsLevelLayoutAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

/** User Story: As a features systems level consumer, I need to invoke load layout seed through a stable signature so the features systems level workflow remains explicit and composable. @fn FLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources) */
FLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources) {
  const func::Maybe<FLayoutSeed> ParsedLayout =
      Layout::LayoutFromJson(
          {{JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Terrain.TerrainBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Terrain.TerrainLabelsJsonPath})},
           {JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Town.TownBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Town.TownLabelsJsonPath})},
           {JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Mine.MineBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.Mine.MineLabelsJsonPath})},
           JsonAdapters::LoadRequiredArrayFromContent(
               {DataSources.Overlay.OverlayLabelsJsonPath})});
  checkf(ParsedLayout.hasValue, TEXT("Level layout JSON is invalid"));
  return ParsedLayout.value;
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
