#include "Features/Systems/Terrain/TerrainSlice.h"

#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FTerrainState>> Slice =
      func::lazy([]() -> rtk::Slice<FTerrainState> {
        return rtk::createSlice<FTerrainState>(
          TEXT("terrain"), TerrainFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTerrainState> &Builder) {
    Builder.addCase(TerrainActions::TerrainLoaded(),
                                TerrainReducers::ReduceTerrainLoaded);
    Builder.addCase(
        TerrainActions::TerrainMeshPayloadObserved(),
        TerrainReducers::ReduceTerrainMeshPayloadObserved);
  });
      });
  return func::eval(Slice);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
