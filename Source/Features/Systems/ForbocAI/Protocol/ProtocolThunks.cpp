#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/ForbocAI/Protocol/Ghost/GhostThunks.h"
#include "Features/Systems/ForbocAI/Protocol/NPC/NPCThunks.h"
#include "Features/Systems/SystemsActions.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolThunks {
namespace {

/** User Story: As a systems forboc ai protocol consumer, I need to invoke to async result through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn template <typename Result> func::AsyncResult<Result> ToAsyncResult(const func::Either<FString, Result> &Outcome) */
template <typename Result>
func::AsyncResult<Result>
ToAsyncResult(const func::Either<FString, Result> &Outcome) {
  return func::ematch(
      Outcome,
      [](const FString &Error) {
        const std::string Utf8 = TCHAR_TO_UTF8(*Error);
        return func::createAsyncResult<Result>(
            [Utf8](std::function<void(Result)>,
                   std::function<void(std::string)> Reject) {
              Reject(Utf8);
            });
      },
      [](const Result &Value) {
        return func::createAsyncResult<Result>(
            [Value](std::function<void(Result)> Resolve,
                    std::function<void(std::string)>) { Resolve(Value); });
      });
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke npc flow available through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn bool NpcFlowAvailable(const FRuntimeState &State) */
bool NpcFlowAvailable(const FRuntimeState &State) {
  return State.ForbocAIProtocol.Npc.Status != EForbocAIFlowStatus::Pending;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke ghost flow available through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn bool GhostFlowAvailable(const FRuntimeState &State) */
bool GhostFlowAvailable(const FRuntimeState &State) {
  return State.ForbocAIProtocol.Ghost.Status != EForbocAIFlowStatus::Pending;
}

} // namespace

/** User Story: As a systems forboc ai protocol consumer, I need to invoke npc interaction async thunk through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn const rtk::AsyncThunkConfig<FForbocAINpcResult, FForbocAINpcRequest, FRuntimeState> & NpcInteractionAsyncThunk() */
const rtk::AsyncThunkConfig<FForbocAINpcResult, FForbocAINpcRequest,
                            FRuntimeState> &
NpcInteractionAsyncThunk() {
  const auto Condition =
      rtk::ConditionCallback<FForbocAINpcRequest, FRuntimeState>(
          [](const FForbocAINpcRequest &,
             const rtk::ThunkApi<FRuntimeState> &Api) {
            return NpcFlowAvailable(Api.getState());
          });
  static const auto Config =
      rtk::createAsyncThunk<FForbocAINpcResult, FForbocAINpcRequest,
                            FRuntimeState>(
          TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV915770BC9684),
          [](const FForbocAINpcRequest &Request,
             const rtk::ThunkApi<FRuntimeState> &Api) {
            const auto Outcome = func::either_map(
                ForbocAINpcThunks::ExecuteNpcInteraction(
                    Request, Api.getState()),
                [&Api](const FForbocAINpcResult &Result) {
                  Api.dispatch(
                      RuntimeActions::TownspersonInteractionSourceObserved()(
                          Result.Source));
                  return Result;
                });
            return ToAsyncResult(Outcome);
          }, Condition);
  return Config;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke ghost analysis async thunk through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn const rtk::AsyncThunkConfig<FForbocAIGhostResult, FForbocAIGhostRequest, FRuntimeState> & GhostAnalysisAsyncThunk() */
const rtk::AsyncThunkConfig<FForbocAIGhostResult, FForbocAIGhostRequest,
                            FRuntimeState> &
GhostAnalysisAsyncThunk() {
  const auto Condition =
      rtk::ConditionCallback<FForbocAIGhostRequest, FRuntimeState>(
          [](const FForbocAIGhostRequest &,
             const rtk::ThunkApi<FRuntimeState> &Api) {
            return GhostFlowAvailable(Api.getState());
          });
  static const auto Config =
      rtk::createAsyncThunk<FForbocAIGhostResult, FForbocAIGhostRequest,
                            FRuntimeState>(
          TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV0DFF7457F377),
          [](const FForbocAIGhostRequest &Request,
             const rtk::ThunkApi<FRuntimeState> &Api) {
            return ToAsyncResult(
                ForbocAIGhostThunks::ExecuteGhostAnalysis(
                    Request, Api.getState()));
          }, Condition);
  return Config;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke request npc interaction through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn rtk::ThunkAction<FForbocAINpcResult, FRuntimeState> RequestNpcInteraction(const FForbocAINpcRequest &Request) */
rtk::ThunkAction<FForbocAINpcResult, FRuntimeState>
RequestNpcInteraction(const FForbocAINpcRequest &Request) {
  return NpcInteractionAsyncThunk()(Request);
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke request ghost analysis through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn rtk::ThunkAction<FForbocAIGhostResult, FRuntimeState> RequestGhostAnalysis(const FForbocAIGhostRequest &Request) */
rtk::ThunkAction<FForbocAIGhostResult, FRuntimeState>
RequestGhostAnalysis(const FForbocAIGhostRequest &Request) {
  return GhostAnalysisAsyncThunk()(Request);
}

} // namespace ForbocAIProtocolThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
