#include "Features/Systems/Terrain/TerrainSlice.h"

#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice() {
  static const rtk::Slice<FTerrainState> Slice = []() {
    rtk::SliceBuilder<FTerrainState> Builder =
        rtk::sliceBuilder<FTerrainState>(
            TEXT("terrain"), TerrainFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, TerrainActions::TerrainLoaded(),
                                TerrainReducers::ReduceTerrainLoaded);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace TerrainSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
