#include "Features/Systems/Spawn/SpawnSlice.h"

#include "Features/Systems/Spawn/SpawnActions.h"
#include "Features/Systems/Spawn/SpawnFactories.h"
#include "Features/Systems/Spawn/SpawnReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace SpawnSlice {

const rtk::Slice<FSpawnState> &GetSlice() {
  static const rtk::Slice<FSpawnState> Slice = []() {
    rtk::SliceBuilder<FSpawnState> Builder =
        rtk::sliceBuilder<FSpawnState>(
            TEXT("spawn"), SpawnFactories::CreateInitialState());
    Builder = rtk::addExtraCase(Builder, SpawnActions::PlayerSpawnAnchored(),
                                SpawnReducers::ReducePlayerSpawnAnchored);
    return rtk::buildSlice(Builder);
  }();
  return Slice;
}

} // namespace SpawnSlice
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
