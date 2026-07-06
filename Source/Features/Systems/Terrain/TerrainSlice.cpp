#include "Features/Systems/Terrain/TerrainSlice.h"

#include "Features/Systems/Terrain/TerrainActions.h"
#include "Features/Systems/Terrain/TerrainFactories.h"
#include "Features/Systems/Terrain/TerrainReducers.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace TerrainSlice {

const rtk::Slice<FTerrainState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FTerrainState>> Slice =
      func::lazy([]() -> rtk::Slice<FTerrainState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FTerrainState>(
          TEXT("terrain"), TerrainFactories::CreateInitialState(),
          [](rtk::ActionReducerMapBuilder<FTerrainState> &Builder) {
    Builder.addCase(TerrainActions::TerrainLoaded(),
                                TerrainReducers::ReduceTerrainLoaded);
  });
      });
  return func::eval(Slice);
}

} // namespace TerrainSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
