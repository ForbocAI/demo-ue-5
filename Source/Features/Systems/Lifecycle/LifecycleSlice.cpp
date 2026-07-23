#include "Features/Systems/Lifecycle/LifecycleSlice.h"

#include "Core/fp.hpp"
#include "Features/Systems/SystemsThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeLifecycleSlice {
namespace {

/** User Story: As a features systems lifecycle consumer, I need to invoke with player spawn status through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState WithPlayerSpawnStatus(FRuntimeLifecycleState State, ERuntimeLoadStatus Status) */
FRuntimeLifecycleState WithPlayerSpawnStatus(FRuntimeLifecycleState State,
                                             ERuntimeLoadStatus Status) {
  State.PlayerSpawnStatus = Status;
  return State;
}

/** User Story: As a features systems lifecycle consumer, I need to invoke with level view status through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState WithLevelViewStatus(FRuntimeLifecycleState State, ERuntimeLoadStatus Status) */
FRuntimeLifecycleState WithLevelViewStatus(FRuntimeLifecycleState State,
                                           ERuntimeLoadStatus Status) {
  State.LevelViewStatus = Status;
  return State;
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce spawn pending through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceSpawnPending( const FRuntimeLifecycleState &State, const rtk::PayloadAction<rtk::FEmptyPayload> &) */
FRuntimeLifecycleState ReduceSpawnPending(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<rtk::FEmptyPayload> &) {
  return WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Loading);
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce spawn fulfilled through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceSpawnFulfilled( const FRuntimeLifecycleState &State, const rtk::PayloadAction<FPointPayload> &) */
FRuntimeLifecycleState ReduceSpawnFulfilled(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FPointPayload> &) {
  return WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Ready);
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce spawn rejected through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceSpawnRejected( const FRuntimeLifecycleState &State, const rtk::PayloadAction<FString> &Action) */
FRuntimeLifecycleState ReduceSpawnRejected(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FString> &Action) {
  FRuntimeLifecycleState Next =
      WithPlayerSpawnStatus(State, ERuntimeLoadStatus::Failed);
  Next.Error = Action.PayloadValue;
  return Next;
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce level view pending through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceLevelViewPending( const FRuntimeLifecycleState &State, const rtk::PayloadAction<rtk::FEmptyPayload> &) */
FRuntimeLifecycleState ReduceLevelViewPending(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<rtk::FEmptyPayload> &) {
  return WithLevelViewStatus(State, ERuntimeLoadStatus::Loading);
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce level view fulfilled through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceLevelViewFulfilled( const FRuntimeLifecycleState &State, const rtk::PayloadAction<FRuntimeLevelViewPayload> &) */
FRuntimeLifecycleState ReduceLevelViewFulfilled(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FRuntimeLevelViewPayload> &) {
  return WithLevelViewStatus(State, ERuntimeLoadStatus::Ready);
}

/** User Story: As a features systems lifecycle consumer, I need to invoke reduce level view rejected through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState ReduceLevelViewRejected( const FRuntimeLifecycleState &State, const rtk::PayloadAction<FString> &Action) */
FRuntimeLifecycleState ReduceLevelViewRejected(
    const FRuntimeLifecycleState &State,
    const rtk::PayloadAction<FString> &Action) {
  FRuntimeLifecycleState Next =
      WithLevelViewStatus(State, ERuntimeLoadStatus::Failed);
  Next.Error = Action.PayloadValue;
  return Next;
}

} // namespace

/** User Story: As a features systems lifecycle consumer, I need to invoke create initial state through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn FRuntimeLifecycleState CreateInitialState() */
FRuntimeLifecycleState CreateInitialState() {
  return FRuntimeLifecycleState{};
}

/** User Story: As a features systems lifecycle consumer, I need to invoke get slice through a stable signature so the features systems lifecycle workflow remains explicit and composable. @fn const rtk::Slice<FRuntimeLifecycleState> &GetSlice() */
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
