#include "Features/Systems/Lifecycle/LifecycleSlice.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/SystemsThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLifecycleSlice {
namespace {

FRuntimeLifecycleState WithPlayerSpawnStatus(FRuntimeLifecycleState State,
                                             ERuntimeLoadStatus Status) {
  State.PlayerSpawnStatus = Status;
  return State;
}

FRuntimeLifecycleState WithLevelViewStatus(FRuntimeLifecycleState State,
                                           ERuntimeLoadStatus Status) {
  State.LevelViewStatus = Status;
  return State;
}

FRuntimeLifecycleState ReduceSpawnPending(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<rtk::FEmptyPayload> &) {
  return WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Loading);
}

FRuntimeLifecycleState ReduceSpawnFulfilled(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FPointPayload> &) {
  return WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Ready);
}

FRuntimeLifecycleState ReduceSpawnRejected(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FString> &Action) {
  FRuntimeLifecycleState Next =
      WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Failed);
  Next.Error = Action.PayloadValue;
  return Next;
}

FRuntimeLifecycleState ReduceLevelViewPending(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<rtk::FEmptyPayload> &) {
  return WithLevelViewStatus(State, ERuntimeLoadStatus::Loading);
}

FRuntimeLifecycleState ReduceLevelViewFulfilled(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FRuntimeLevelViewPayload> &) {
  return WithLevelViewStatus(State, ERuntimeLoadStatus::Ready);
}

FRuntimeLifecycleState ReduceLevelViewRejected(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FString> &Action) {
  FRuntimeLifecycleState Next =
      WithLevelViewStatus(State, ERuntimeLoadStatus::Failed);
  Next.Error = Action.PayloadValue;
  return Next;
}

} // namespace

FRuntimeLifecycleState CreateInitialState() {
  return FRuntimeLifecycleState{};
}

const rtk::Slice<FRuntimeLifecycleState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRuntimeLifecycleState>> Slice =
      func::lazy([]() -> rtk::Slice<FRuntimeLifecycleState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON data.
        return rtk::createSlice<FRuntimeLifecycleState>(
            TEXT("runtime/lifecycle"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FRuntimeLifecycleState> &Builder) {
              const rtk::AsyncThunkConfig<FPointPayload, rtk::FEmptyPayload,
                                          FRuntimeState> &Spawn =
                  RuntimeThunks::RequestPlayerSpawnAsyncThunk();
              const rtk::AsyncThunkConfig<FRuntimeLevelViewPayload,
                                          rtk::FEmptyPayload, FRuntimeState>
                  &LevelView = RuntimeThunks::RequestLevelViewPayloadAsyncThunk();
              Builder.addCase(Spawn.pending, ReduceSpawnPending)
                  .addCase(Spawn.fulfilled, ReduceSpawnFulfilled)
                  .addCase(Spawn.rejected, ReduceSpawnRejected)
                  .addCase(LevelView.pending, ReduceLevelViewPending)
                  .addCase(LevelView.fulfilled, ReduceLevelViewFulfilled)
                  .addCase(LevelView.rejected, ReduceLevelViewRejected);
            });
      });
  return func::eval(Slice);
}

} // namespace RuntimeLifecycleSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
