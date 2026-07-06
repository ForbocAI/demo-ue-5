#pragma once

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Core/rtk.hpp"
#include "Features/Systems/UI/UITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace UIReducers {
namespace detail {

using FChatMessageViewModel =
    ForbocAI::Game::UI::FChatMessageViewModel;
using FChatMessageViewModels =
    TArray<ForbocAI::Game::UI::FChatMessageViewModel>;
using FRuntimeConversationViewModel =
    ForbocAI::Game::UI::FRuntimeConversationViewModel;
using FUIRuntimeSettings =
    ForbocAI::Game::Data::FUIRuntimeSettings;

struct FUIRuntimeConversationText {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
};

struct FUIRoleColorDeclaration {
  FString FUIRuntimeSettings::*Role;
  FLinearColor FUIRuntimeSettings::*Color;
};

template <typename Value> struct TUIStatePayloadFieldDeclaration {
  Value FUIState::*StateField;
  Value FUIPayload::*PayloadField;
};

struct FUIChatHistoryRenderedDeclaration {};

template <typename Declaration> struct TUIReducerRegistry;

template <> struct TUIReducerRegistry<FUIRoleColorDeclaration> {
  static const TArray<FUIRoleColorDeclaration> &Declarations() {
    static const TArray<FUIRoleColorDeclaration> RegisteredDeclarations = {
        {&FUIRuntimeSettings::PlayerRoleLabel, &FUIRuntimeSettings::PlayerColor},
        {&FUIRuntimeSettings::SystemRoleLabel, &FUIRuntimeSettings::SystemColor},
        {&FUIRuntimeSettings::NpcRoleLabel, &FUIRuntimeSettings::NpcColor},
        {&FUIRuntimeSettings::UnknownRoleLabel,
         &FUIRuntimeSettings::UnknownColor}};
    return RegisteredDeclarations;
  }
};

template <> struct TUIReducerRegistry<FUIChatHistoryRenderedDeclaration> {
  static const TArray<TUIStatePayloadFieldDeclaration<FChatMessageViewModels>>
      &Declarations() {
    static const TArray<
        TUIStatePayloadFieldDeclaration<FChatMessageViewModels>>
        RegisteredDeclarations = {{&FUIState::Messages,
                                   &FUIPayload::Messages}};
    return RegisteredDeclarations;
  }
};

inline func::Maybe<FLinearColor> FindRoleColor(
    const FString &Role, const FUIRuntimeSettings &Settings) {
  const TArray<FUIRoleColorDeclaration> &Declarations =
      TUIReducerRegistry<FUIRoleColorDeclaration>::Declarations();
  return func::fold_array<FUIRoleColorDeclaration,
                          func::Maybe<FLinearColor>>(
      Declarations, func::nothing<FLinearColor>(),
      [&Role, &Settings](const func::Maybe<FLinearColor> &Acc,
                         const FUIRoleColorDeclaration &Declaration) {
        return Acc.hasValue || (Settings.*(Declaration.Role)) != Role
                   ? Acc
                   : func::just(Settings.*(Declaration.Color));
      });
}

/**
 * Groups the payload and field declarations for a reduce-payload-fields call.
 * Pure data — no behavior.
 */
template <typename Value>
struct TReducePayloadFieldsRequest {
  const FUIPayload &Payload;
  const TArray<TUIStatePayloadFieldDeclaration<Value>> &Declarations;
};

template <typename Value>
FUIState ReducePayloadFields(
    const FUIState &State,
    const TReducePayloadFieldsRequest<Value> &Request) {
  return func::fold_array<TUIStatePayloadFieldDeclaration<Value>, FUIState>(
      Request.Declarations, State,
      [&Request](const FUIState &Acc,
                 const TUIStatePayloadFieldDeclaration<Value> &Declaration) {
        FUIState Next = Acc;
        Next.*(Declaration.StateField) = Request.Payload.*(Declaration.PayloadField);
        return Next;
      });
}

/**
 * Groups the action and field declarations for a reduce-payload-action-fields call.
 * Pure data — no behavior.
 */
template <typename Value>
struct TReducePayloadActionFieldsRequest {
  const rtk::PayloadAction<FUIPayload> &Action;
  const TArray<TUIStatePayloadFieldDeclaration<Value>> &Declarations;
};

template <typename Value>
FUIState ReducePayloadActionFields(
    const FUIState &State,
    const TReducePayloadActionFieldsRequest<Value> &Request) {
  return (func::pipe(State) |
          [&Request](FUIState Next) -> FUIState {
            Next.LastActionId = func::just(Request.Action.PayloadValue.Id);
            return Next;
          } |
          [&Request](const FUIState &Next) -> FUIState {
            return ReducePayloadFields<Value>(Next,
                {Request.Action.PayloadValue, Request.Declarations});
          })
      .val;
}

inline FLinearColor ReduceChatColorForRole(const FString &Role,
                                           const FUIRuntimeSettings &Settings) {
  const func::Maybe<FLinearColor> Color = FindRoleColor(Role, Settings);
  check(Color.hasValue);
  return Color.value;
}

inline FChatMessageViewModel ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const FUIRuntimeSettings &Settings) {
  return {frmt::RuntimeString(
              Settings.ChatMessageFormat,
              frmt::Args(
                  {frmt::Arg(Request.Role),
                   frmt::Arg(Request.Text)})),
          ReduceChatColorForRole(Request.Role, Settings)};
}

inline FChatMessageViewModel
ReduceHistoryEntryViewModel(const FString &Entry,
                            const FUIRuntimeSettings &Settings) {
  const int32 SeparatorIndex = Entry.Find(Settings.HistoryRoleSeparator);
  return SeparatorIndex > Settings.HistoryMinimumRoleIndex
             ? ReduceChatMessageViewModel(
                   {Entry.Left(SeparatorIndex),
                    Entry.Mid(SeparatorIndex +
                              Settings.HistoryTextStartOffset)},
                   Settings)
             : ReduceChatMessageViewModel({Settings.UnknownRoleLabel, Entry},
                                          Settings);
}

inline FString ReduceSubmittedChatText(const FText &Text) {
  return Text.ToString().TrimStartAndEnd();
}

template <typename Source, typename Output, typename Context>
TArray<Output> MapWithContext(const TArray<Source> &Values,
                              const Context &ContextValue,
                              TFunctionRef<Output(const Source &, const Context &)> Map) {
  return func::map_array<Source, Output>(
      Values, [&ContextValue, Map](const Source &Value) {
        return Map(Value, ContextValue);
      });
}

inline FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationText &Text,
    const FUIRuntimeSettings &Settings) {
  return {Text.Title,
          Text.PlayerLine,
          Text.NpcReply,
          Settings.PanelColor,
          Settings.TitleColor,
          Settings.PlayerColor,
          Settings.RuntimeReplyColor,
          Settings.PanelPadding,
          Settings.TitleSize,
          Settings.BodySize};
}

} // namespace detail

/**
 * @brief Case reducer for UIActions::ConversationPresented.
 *
 * @param State Previous UI slice state.
 * @param Action Payload action carrying reducer-owned presentation data.
 * @return Next immutable UI state.
 *
 * Architecture: This reducer owns visibility and conversation replacement. It
 * uses functional-core piping as composition only; RTK action/reducer semantics
 * remain the state model.
 */
inline FUIState ReduceConversationPresented(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FUIState Next) -> FUIState {
            Next.LastActionId = func::just(Action.PayloadValue.Id);
            Next.Conversation = Action.PayloadValue.Conversation;
            Next.bConversationVisible = true;
            return Next;
          })
      .val;
}

/**
 * @brief Case reducer for UIActions::ChatHistoryRendered.
 */
inline FUIState ReduceChatHistoryRendered(
    const FUIState &State,
    const rtk::PayloadAction<FUIPayload> &Action) {
  return detail::ReducePayloadActionFields<detail::FChatMessageViewModels>(
      State,
      {Action,
       detail::TUIReducerRegistry<
           detail::FUIChatHistoryRenderedDeclaration>::Declarations()});
}

/**
 * @brief Pure reducer helper for turning role/text into one view model.
 */
inline ForbocAI::Game::UI::FChatMessageViewModel ReduceChatMessageViewModel(
    const FUIChatMessageViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceChatMessageViewModel(Request, Settings);
}

/**
 * @brief Pure reducer helper for converting history entries into view models.
 */
inline TArray<ForbocAI::Game::UI::FChatMessageViewModel>
ReduceChatHistoryViewModels(
    const FUIChatHistoryViewModelsRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::MapWithContext<FString, detail::FChatMessageViewModel,
                                detail::FUIRuntimeSettings>(
      Request.History, Settings, detail::ReduceHistoryEntryViewModel);
}

/**
 * @brief Pure reducer helper that normalizes committed chat input text.
 */
inline FString ReduceNormalizedSubmittedChatText(
    const FUIChatInputViewModelRequest &Request) {
  return detail::ReduceSubmittedChatText(Request.Text);
}

/**
 * @brief Pure reducer helper that returns the default hidden conversation
 * model.
 */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationPlaceholder(
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {Settings.PlaceholderTitle, Settings.PlaceholderPlayerLine,
       Settings.PlaceholderNpcReply},
      Settings);
}

/**
 * @brief Pure reducer helper that composes the runtime conversation model.
 */
inline ForbocAI::Game::UI::FRuntimeConversationViewModel
ReduceRuntimeConversationViewModel(
    const FUIRuntimeConversationViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  return detail::ReduceRuntimeConversationViewModel(
      {frmt::RuntimeString(
           Settings.ConversationTitleFormat,
           frmt::Args(
               {frmt::Arg(Request.NpcName),
                frmt::Arg(Request.Role)})),
       frmt::RuntimeString(
           Settings.PlayerLineFormat,
           frmt::Args(
               {frmt::Arg(Request.PlayerLine)})),
       frmt::RuntimeString(
           Settings.NpcReplyFormat,
           frmt::Args(
               {frmt::Arg(Request.NpcReply)}))},
      Settings);
}

inline FUIBindDialogueViewModel ReduceBindDialogueViewModel(
    const FUIBindDialogueViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  FUIBindDialogueViewModel Model;
  Model.bBound = Request.bBound;
  Model.Persona = Request.Persona;
  Model.ConnectionMessage = ReduceChatMessageViewModel(
      {Settings.SystemRoleLabel,
       frmt::RuntimeString(
           Settings.ConnectionMessageFormat,
           frmt::Args(
               {frmt::Arg(Request.Persona)}))},
      Settings);
  Model.HistoryMessages = ReduceChatHistoryViewModels({Request.History},
                                                      Settings);
  return Model;
}

inline FUIChatInputViewModel ReduceChatInputViewModel(
    const FUIChatInputViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  const FString InputText = ReduceNormalizedSubmittedChatText(Request);
  FUIChatInputViewModel Model;
  Model.bShouldSend =
      Request.CommitMethod == ETextCommit::OnEnter && !InputText.IsEmpty();
  Model.InputText = InputText;
  Model.PlayerMessage =
      ReduceChatMessageViewModel({Settings.PlayerRoleLabel, InputText},
                                 Settings);
  Model.ErrorMessage =
      ReduceChatMessageViewModel({Settings.SystemRoleLabel,
                                  Settings.UnboundDialogueError},
                                 Settings);
  return Model;
}

inline FUIDialogueResponseViewModel ReduceDialogueResponseViewModel(
    const FUIDialogueResponseViewModelRequest &Request,
    const ForbocAI::Game::Data::FUIRuntimeSettings &Settings) {
  FUIDialogueResponseViewModel Model;
  Model.Message = ReduceChatMessageViewModel(
      {Settings.NpcRoleLabel, Request.NPCText}, Settings);
  return Model;
}

} // namespace UIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
