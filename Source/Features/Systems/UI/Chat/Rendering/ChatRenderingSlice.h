#pragma once

#include "Core/frmt.hpp"
#include "Core/fp.hpp"
#include "Core/rtk.hpp"
#include "Features/Systems/UI/SystemsUITypes.h"

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
using FUISettings =
    ForbocAI::Game::Data::FUISettings;

struct FUIRoleColorDeclaration {
  FString FUISettings::*Role;
  FLinearColor FUISettings::*Color;
};

template <typename Value> struct TUIStatePayloadFieldDeclaration {
  Value FUIState::*StateField;
  Value FUIPayload::*PayloadField;
};

struct FUIChatHistoryRenderedDeclaration {};

template <typename Declaration> struct TUIReducerRegistry;

template <> struct TUIReducerRegistry<FUIRoleColorDeclaration> {
  /** User Story: As a ui chat rendering consumer, I need to invoke declarations through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn static const TArray<FUIRoleColorDeclaration> &Declarations() */
  static const TArray<FUIRoleColorDeclaration> &Declarations() {
    static const TArray<FUIRoleColorDeclaration> RegisteredDeclarations = {
        {&FUISettings::PlayerRoleLabel, &FUISettings::PlayerColor},
        {&FUISettings::SystemRoleLabel, &FUISettings::SystemColor},
        {&FUISettings::NpcRoleLabel, &FUISettings::NpcColor},
        {&FUISettings::UnknownRoleLabel,
         &FUISettings::UnknownColor}};
    return RegisteredDeclarations;
  }
};

template <> struct TUIReducerRegistry<FUIChatHistoryRenderedDeclaration> {
  /** User Story: As a ui chat rendering consumer, I need to invoke declarations through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn static const TArray<TUIStatePayloadFieldDeclaration<FChatMessageViewModels>> &Declarations() */
  static const TArray<TUIStatePayloadFieldDeclaration<FChatMessageViewModels>>
      &Declarations() {
    static const TArray<
        TUIStatePayloadFieldDeclaration<FChatMessageViewModels>>
        RegisteredDeclarations = {{&FUIState::Messages,
                                   &FUIPayload::Messages}};
    return RegisteredDeclarations;
  }
};

/** User Story: As a ui chat rendering consumer, I need to invoke find role color through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn inline func::Maybe<FLinearColor> FindRoleColor( const FString &Role, const FUISettings &Settings) */
inline func::Maybe<FLinearColor> FindRoleColor(
    const FString &Role, const FUISettings &Settings) {
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

/** User Story: As a ui chat rendering consumer, I need to invoke reduce payload fields through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn template <typename Value> FUIState ReducePayloadFields( const FUIState &State, const TReducePayloadFieldsRequest<Value> &Request) */
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

/** User Story: As a ui chat rendering consumer, I need to invoke reduce payload action fields through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn template <typename Value> FUIState ReducePayloadActionFields( const FUIState &State, const TReducePayloadActionFieldsRequest<Value> &Request) */
template <typename Value>
FUIState ReducePayloadActionFields(
    const FUIState &State,
    const TReducePayloadActionFieldsRequest<Value> &Request) {
  return (func::pipe(State) |
          [&Request](FUIState Next) -> FUIState {
            Next.ActionId = func::just(Request.Action.PayloadValue.Id);
            return Next;
          } |
          [&Request](const FUIState &Next) -> FUIState {
            return ReducePayloadFields<Value>(Next,
                {Request.Action.PayloadValue, Request.Declarations});
          })
      .val;
}

/** User Story: As a ui chat rendering consumer, I need to invoke map with context through a stable signature so the ui chat rendering workflow remains explicit and composable. @fn template <typename Source, typename Output, typename Context> TArray<Output> MapWithContext(const TArray<Source> &Values, const Context &ContextValue, TFunctionRef<Output(const Source &, const Context &)> Map) */
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
