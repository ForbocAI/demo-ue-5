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

template <typename Source, typename Output, typename Context>
TArray<Output> MapWithContext(const TArray<Source> &Values,
                              const Context &ContextValue,
                              TFunctionRef<Output(const Source &, const Context &)> Map) {
  return func::map_array<Source, Output>(
      Values, [&ContextValue, Map](const Source &Value) {
        return Map(Value, ContextValue);
      });
}

} // namespace detail
} // namespace UIReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI
