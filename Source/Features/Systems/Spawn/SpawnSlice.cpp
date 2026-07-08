#include "Features/Systems/Spawn/SpawnSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnReducers {

FSpawnState
ReducePlayerSpawnAnchored(const FSpawnState &State,
                          const rtk::PayloadAction<FPointPayload> &Action) {
  return (func::pipe(State) | [&](FSpawnState Next) -> FSpawnState {
  Next.PlayerSpawn = Action.PayloadValue;
  return Next;
  }).val;
}

} // namespace SpawnReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Spawn/SpawnActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnSlice {

const rtk::Slice<FSpawnState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FSpawnState>> Slice =
      func::lazy([]() -> rtk::Slice<FSpawnState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
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
} // namespace Game
} // namespace ForbocAI


namespace ForbocAI {
namespace Game {
namespace Level {
namespace SpawnFactories {

FSpawnState CreateInitialState() {
  FSpawnState State;
  State.PlayerSpawn =
      SpawnPointPayload({FVector::ZeroVector, FRotator::ZeroRotator,
                         TEXT("unassigned")});
  return State;
}

FPointPayload SpawnPointPayload(const FPointSource &Source) {
  FPointPayload Payload;
  Payload.Location = Source.Location;
  Payload.Rotation = Source.Rotation;
  Payload.AnchorLabel = Source.AnchorLabel;
  return Payload;
}

} // namespace SpawnFactories
} // namespace Level
} // namespace Game
} // namespace ForbocAI
