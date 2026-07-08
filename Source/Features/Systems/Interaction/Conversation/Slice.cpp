#include "Features/Systems/Interaction/Conversation/Slice.h"

#include "Core/frmt.hpp"
#include "Features/Systems/Dialogue/Slice.h"
#include "Features/Systems/UI/Shared/Slice.h"
#include "Features/Systems/UI/Chat/Slice.h"
#include "Features/Systems/UI/Conversation/Slice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.LastTownspersonInteractionRequest =
                ReduceTownspersonInteractionRequest(Action.PayloadValue);
            const FRuntimeTownspersonInteractionPayload Payload =
                ReduceTownspersonInteractionPayload(
                    {Next.LastTownspersonInteractionRequest,
                     Next.Dialogue.Settings, Next.UI.Settings});
            Next.Dialogue = DialogueReducers::ReduceLocalReplyResolved(
                Next.Dialogue, {Payload.DialogueReply.Id,
                                Payload.DialogueReply});
            Next.UI = UIReducers::ReduceConversationPresented(
                Next.UI, {Payload.UI.Id, Payload.UI});
            return Next;
          })
      .val;
}

FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source) {
  return {Source.Name, Source.Role, Source.Persona, Source.DefaultPlayerLine,
          Source.PinnedResponse};
}

FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return {Request.Name, Request.Role, Request.Persona, Request.PlayerLine,
          Request.PinnedResponse};
}

FUIPayload ReduceConversationPresentedPayload(
    const FDialogueReplyPayload &DialogueReply,
    const ForbocAI::Game::Data::FUISettings &UISettings) {
  FUIPayload Payload;
  Payload.Id = frmt::RuntimeString(
      UISettings.PayloadIdFormat,
      frmt::Args(
          {frmt::Arg(DialogueReply.Request.Name)}));
  Payload.Conversation = UIReducers::ReduceRuntimeConversationViewModel(
      {DialogueReply.Request.Name, DialogueReply.Request.Role,
       DialogueReply.Request.PlayerLine, DialogueReply.Reply},
      UISettings);
  return Payload;
}

FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FRuntimeTownspersonInteractionPayloadRequest &Request) {
  const FDialogueReplyPayload DialogueReply =
      DialogueReducers::ReduceLocalReplyPayload(
          ReduceLocalDialogueReplyRequest(Request.Request),
          Request.DialogueSettings);
  return {DialogueReply,
          ReduceConversationPresentedPayload(DialogueReply,
                                             Request.UISettings)};
}

} // namespace RuntimeReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
