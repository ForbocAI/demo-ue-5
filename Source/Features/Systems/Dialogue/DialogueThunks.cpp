#include "Features/Systems/Dialogue/DialogueThunks.h"

#include "Features/Systems/Dialogue/DialogueSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace DialogueThunks {
namespace {

FDialogueReplyPayload LocalReplyPayload(
    const FLocalDialogueReplyRequest &Request) {
  FDialogueReplyPayload Payload;
  Payload.Id = FString::Printf(TEXT("systems/dialogue/localReply/%s"),
                               *Request.Name);
  Payload.Request = Request;
  Payload.Reply = DialogueSelectors::SelectLocalReply(Request);
  return Payload;
}

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
                        Resolve(LocalReplyPayload(Request));
                      });
                });
          });
  return func::eval(Thunk);
}

} // namespace DialogueThunks
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
