#include "Features/Systems/Spawn/SpawnSlice.h"

#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Spawn/SpawnReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SpawnSlice {

const rtk::Slice<FSpawnState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSpawnState>> Slice =
      func::lazy([]() -> rtk::Slice<FSpawnState> {
        return rtk::createSlice<FSpawnState>(
      TEXT("spawn"), SpawnFactories::CreateInitialState(),
      [](rtk::ActionReducerMapBuilder<FSpawnState> &Builder) {
    Builder.addCase(SpawnActions::PlayerSpawnAnchored(),
                                SpawnReducers::ReducePlayerSpawnAnchored);
  });
      });
  return func::eval(Slice);
}

} // namespace SpawnSlice
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
