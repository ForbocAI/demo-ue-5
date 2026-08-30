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

bool NpcFlowAvailable(const FRuntimeState &State) {
  return State.ForbocAIProtocol.Npc.Status != EForbocAIFlowStatus::Pending;
}

bool GhostFlowAvailable(const FRuntimeState &State) {
  return State.ForbocAIProtocol.Ghost.Status != EForbocAIFlowStatus::Pending;
}

} // namespace

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

rtk::ThunkAction<FForbocAINpcResult, FRuntimeState>
RequestNpcInteraction(const FForbocAINpcRequest &Request) {
  return NpcInteractionAsyncThunk()(Request);
}

rtk::ThunkAction<FForbocAIGhostResult, FRuntimeState>
RequestGhostAnalysis(const FForbocAIGhostRequest &Request) {
  return GhostAnalysisAsyncThunk()(Request);
}

} // namespace ForbocAIProtocolThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
