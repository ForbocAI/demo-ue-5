#include "Features/Systems/Interaction/Conversation/InteractionConversationSlice.h"

#include "Core/frmt.hpp"
#include "Features/Systems/Dialogue/DialogueSlice.h"
#include "Features/Systems/UI/Chat/Rendering/ChatRenderingSlice.h"
#include "Features/Systems/UI/Chat/ChatSlice.h"
#include "Features/Systems/UI/Conversation/UIConversationSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeReducers {

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction source observed through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FRuntimeState ReduceTownspersonInteractionSourceObserved( const FRuntimeState &State, const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action) */
FRuntimeState ReduceTownspersonInteractionSourceObserved(
    const FRuntimeState &State,
    const rtk::PayloadAction<FRuntimeTownspersonInteractionSource> &Action) {
  return (func::pipe(State) |
          [&Action](FRuntimeState Next) -> FRuntimeState {
            Next.TownspersonInteractionRequest =
                ReduceTownspersonInteractionRequest(Action.PayloadValue);
            const FRuntimeTownspersonInteractionPayload Payload =
                ReduceTownspersonInteractionPayload(
                    {Next.TownspersonInteractionRequest,
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

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction request through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FRuntimeTownspersonInteractionRequest ReduceTownspersonInteractionRequest( const FRuntimeTownspersonInteractionSource &Source) */
FRuntimeTownspersonInteractionRequest
ReduceTownspersonInteractionRequest(
    const FRuntimeTownspersonInteractionSource &Source) {
  return {Source.Name, Source.Role, Source.Persona, Source.DefaultPlayerLine,
          Source.PinnedResponse};
}

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce local dialogue reply request through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest( const FRuntimeTownspersonInteractionRequest &Request) */
FLocalDialogueReplyRequest ReduceLocalDialogueReplyRequest(
    const FRuntimeTownspersonInteractionRequest &Request) {
  return {Request.Name, Request.Role, Request.Persona, Request.PlayerLine,
          Request.PinnedResponse};
}

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce conversation presented payload through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FUIPayload ReduceConversationPresentedPayload( const FReplyPayload &DialogueReply, const ForbocAI::Game::Data::FUISettings &UISettings) */
FUIPayload ReduceConversationPresentedPayload(
    const FReplyPayload &DialogueReply,
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

/** User Story: As a systems interaction conversation consumer, I need to invoke reduce townsperson interaction payload through a stable signature so the systems interaction conversation workflow remains explicit and composable. @fn FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload( const FRuntimeTownspersonInteractionPayloadRequest &Request) */
FRuntimeTownspersonInteractionPayload ReduceTownspersonInteractionPayload(
    const FRuntimeTownspersonInteractionPayloadRequest &Request) {
  const FReplyPayload DialogueReply =
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
