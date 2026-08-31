#include "Features/Systems/ForbocAI/Protocol/ProtocolSlice.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolSlice {
namespace {

/** User Story: As a systems forboc ai protocol consumer, I need to invoke pending flow through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowState PendingFlow(const FString &CorrelationId) */
FForbocAIFlowState PendingFlow(const FString &CorrelationId) {
  return {EForbocAIFlowStatus::Pending,
          {{CorrelationId, FString(), FString()},
           {FString(), FString(), FString(), FString()}}};
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke succeeded flow through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowState SucceededFlow( const FCommandEvidence &Evidence) */
FForbocAIFlowState SucceededFlow(
    const FCommandEvidence &Evidence) {
  return {EForbocAIFlowStatus::Succeeded, Evidence};
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke failed flow through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowState FailedFlow(FForbocAIFlowState Flow, const FString &Error) */
FForbocAIFlowState FailedFlow(FForbocAIFlowState Flow,
                              const FString &Error) {
  Flow.Status = EForbocAIFlowStatus::Failed;
  Flow.Evidence.Result.Error = Error;
  return Flow;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce npc pending through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceNpcPending( const FProtocolState &State, const rtk::PayloadAction<FForbocAINpcRequest> &Action) */
FProtocolState ReduceNpcPending(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAINpcRequest> &Action) {
  FProtocolState Next = State;
  Next.Npc = PendingFlow(Action.PayloadValue.CorrelationId);
  return Next;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce npc fulfilled through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceNpcFulfilled( const FProtocolState &State, const rtk::PayloadAction<FForbocAINpcResult> &Action) */
FProtocolState ReduceNpcFulfilled(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAINpcResult> &Action) {
  FProtocolState Next = State;
  Next.Npc = SucceededFlow(Action.PayloadValue.Evidence);
  return Next;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce npc rejected through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceNpcRejected( const FProtocolState &State, const rtk::PayloadAction<FString> &Action) */
FProtocolState ReduceNpcRejected(
    const FProtocolState &State,
    const rtk::PayloadAction<FString> &Action) {
  FProtocolState Next = State;
  Next.Npc = FailedFlow(State.Npc, Action.PayloadValue);
  return Next;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce ghost pending through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceGhostPending( const FProtocolState &State, const rtk::PayloadAction<FForbocAIGhostRequest> &Action) */
FProtocolState ReduceGhostPending(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAIGhostRequest> &Action) {
  FProtocolState Next = State;
  Next.Ghost = PendingFlow(Action.PayloadValue.CorrelationId);
  return Next;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce ghost fulfilled through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceGhostFulfilled( const FProtocolState &State, const rtk::PayloadAction<FForbocAIGhostResult> &Action) */
FProtocolState ReduceGhostFulfilled(
    const FProtocolState &State,
    const rtk::PayloadAction<FForbocAIGhostResult> &Action) {
  FProtocolState Next = State;
  Next.Ghost = SucceededFlow(Action.PayloadValue.Evidence);
  return Next;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke reduce ghost rejected through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState ReduceGhostRejected( const FProtocolState &State, const rtk::PayloadAction<FString> &Action) */
FProtocolState ReduceGhostRejected(
    const FProtocolState &State,
    const rtk::PayloadAction<FString> &Action) {
  FProtocolState Next = State;
  Next.Ghost = FailedFlow(State.Ghost, Action.PayloadValue);
  return Next;
}

} // namespace

/** User Story: As a systems forboc ai protocol consumer, I need to invoke create initial state through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FProtocolState CreateInitialState( const ForbocAI::Game::Data::FProtocolSettings &Settings) */
FProtocolState CreateInitialState(
    const ForbocAI::Game::Data::FProtocolSettings &Settings) {
  return {Settings, FForbocAIFlowState{}, FForbocAIFlowState{}};
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke get slice through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn const rtk::Slice<FProtocolState> &GetSlice() */
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
