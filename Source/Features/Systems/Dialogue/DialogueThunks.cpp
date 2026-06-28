#include "Features/Systems/Dialogue/DialogueThunks.h"

#include "Features/Systems/Dialogue/DialogueReducers.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueThunks {
namespace {

} // namespace

const rtk::AsyncThunkConfig<FDialogueReplyPayload, FLocalDialogueReplyRequest,
                            FDialogueState> &
RequestLocalReply() {
  static const func::Lazy<
      rtk::AsyncThunkConfig<FDialogueReplyPayload, FLocalDialogueReplyRequest,
                            FDialogueState>>
      Thunk = func::lazy(
          []() -> rtk::AsyncThunkConfig<FDialogueReplyPayload,
                                        FLocalDialogueReplyRequest,
                                        FDialogueState> {
            return rtk::createAsyncThunk<
                FDialogueReplyPayload, FLocalDialogueReplyRequest,
                FDialogueState>(
                TEXT("systems/dialogue/requestLocalReply"),
                [](const FLocalDialogueReplyRequest &Request,
                   const rtk::ThunkApi<FDialogueState> &Api)
                    -> func::AsyncResult<FDialogueReplyPayload> {
                  (void)Api;
                  return func::createAsyncResult<FDialogueReplyPayload>(
                      [Request](std::function<void(FDialogueReplyPayload)>
                                    Resolve,
                                std::function<void(std::string)> Reject) {
                        (void)Reject;
                        Resolve(DialogueReducers::ReduceLocalReplyPayload(
                            Request));
                      });
                });
          });
  return func::eval(Thunk);
}

} // namespace DialogueThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
