#include "Features/Systems/ForbocAI/Protocol/ProtocolSlice.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolSlice {
namespace {

FForbocAIFlowState PendingFlow(const FString &CorrelationId) {
  return {EForbocAIFlowStatus::Pending,
          {{CorrelationId, FString(), FString()},
           {FString(), FString(), FString(), FString()}}};
}

FForbocAIFlowState SucceededFlow(
    const FCommandEvidence &Evidence) {
  return {EForbocAIFlowStatus::Succeeded, Evidence};
}

FForbocAIFlowState FailedFlow(FForbocAIFlowState Flow,
                              const FString &Error) {
  Flow.Status = EForbocAIFlowStatus::Failed;
  Flow.Evidence.Result.Error = Error;
  return Flow;
}

FProtocolState ReduceNpcPending(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAINpcRequest> &Action) {
  FProtocolState Next = State;
  Next.Npc = PendingFlow(Action.PayloadValue.CorrelationId);
  return Next;
}

FProtocolState ReduceNpcFulfilled(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAINpcResult> &Action) {
  FProtocolState Next = State;
  Next.Npc = SucceededFlow(Action.PayloadValue.Evidence);
  return Next;
}

FProtocolState ReduceNpcRejected(
    const FProtocolState &State,
    const rtk::PayloadAction<FString> &Action) {
  FProtocolState Next = State;
  Next.Npc = FailedFlow(State.Npc, Action.PayloadValue);
  return Next;
}

FProtocolState ReduceGhostPending(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAIGhostRequest> &Action) {
  FProtocolState Next = State;
  Next.Ghost = PendingFlow(Action.PayloadValue.CorrelationId);
  return Next;
}

FProtocolState ReduceGhostFulfilled(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAIGhostResult> &Action) {
  FProtocolState Next = State;
  Next.Ghost = SucceededFlow(Action.PayloadValue.Evidence);
  return Next;
}

FProtocolState ReduceGhostRejected(
    const FProtocolState &State,
    const rtk::PayloadAction<FString> &Action) {
  FProtocolState Next = State;
  Next.Ghost = FailedFlow(State.Ghost, Action.PayloadValue);
  return Next;
}

} // namespace

FProtocolState CreateInitialState(
    const ForbocAI::Game::Data::FProtocolSettings &Settings) {
  return {Settings, FForbocAIFlowState{}, FForbocAIFlowState{}};
}

const rtk::Slice<FProtocolState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FProtocolState>> Slice =
      func::lazy([]() -> rtk::Slice<FProtocolState> {
        return rtk::createSlice<FProtocolState>(
            TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV5AF95E21585F),
            FProtocolState{},
            [](rtk::ActionReducerMapBuilder<FProtocolState> &Builder) {
              const auto &Npc =
                  ForbocAIProtocolThunks::NpcInteractionAsyncThunk();
              const auto &Ghost =
                  ForbocAIProtocolThunks::GhostAnalysisAsyncThunk();
              Builder.addCase(Npc.pending, ReduceNpcPending)
                  .addCase(Npc.fulfilled, ReduceNpcFulfilled)
                  .addCase(Npc.rejected, ReduceNpcRejected)
                  .addCase(Ghost.pending, ReduceGhostPending)
                  .addCase(Ghost.fulfilled, ReduceGhostFulfilled)
                  .addCase(Ghost.rejected, ReduceGhostRejected);
            });
      });
  return func::eval(Slice);
}

} // namespace ForbocAIProtocolSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
