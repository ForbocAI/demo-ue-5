#include "Features/Systems/Level/SystemsLevelAdapters.h"

#include "Features/Components/Data/Json/Read/ReadAdapters.h"
#include "Features/Systems/Level/Layout/SystemsLevelLayoutAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace LevelAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

FLayoutSeed LoadLayoutSeed(const ForbocAI::Game::Data::FDataSourceSettings &DataSources) {
  const func::Maybe<FLayoutSeed> ParsedLayout =
      Layout::LayoutFromJson(
          {{JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.TerrainBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.TerrainLabelsJsonPath})},
           {JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.TownBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.TownLabelsJsonPath})},
           {JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.MineBlocksJsonPath}),
            JsonAdapters::LoadRequiredArrayFromContent(
                {DataSources.MineLabelsJsonPath})},
           JsonAdapters::LoadRequiredArrayFromContent(
               {DataSources.OverlayLabelsJsonPath})});
  checkf(ParsedLayout.hasValue, TEXT("Level layout JSON is invalid"));
  return ParsedLayout.value;
}

} // namespace LevelAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
