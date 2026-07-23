#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Types/SlateEnums.h"

namespace ForbocAI {
namespace Game {
namespace UI {

/**
 * @brief Selector/reducer-owned display data for one chat row.
 *
 * Architecture: Widgets consume this value as inert presentation data. They do
 * not infer roles or colors; reducers build those choices before render.
 */
struct FChatMessageViewModel {
  FString Text;
  FLinearColor Color;
};

/** User Story: As a features systems ui consumer, I need to compare values for equality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator==(const FChatMessageViewModel &Left, const FChatMessageViewModel &Right) */
inline bool operator==(const FChatMessageViewModel &Left,
                       const FChatMessageViewModel &Right) {
  return Left.Text == Right.Text && Left.Color == Right.Color;
}

/** User Story: As a features systems ui consumer, I need to compare values for inequality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator!=(const FChatMessageViewModel &Left, const FChatMessageViewModel &Right) */
inline bool operator!=(const FChatMessageViewModel &Left,
                       const FChatMessageViewModel &Right) {
  return !(Left == Right);
}

/**
 * @brief Store-derived runtime conversation model consumed by WBP_Chat.
 *
 * User story: As a player, I see the selected townsperson reply without the
 * widget owning dialogue rules or reaching into gameplay state.
 */
struct FRuntimeConversationText {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
};

struct FRuntimeConversationColors {
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor PlayerColor;
  FLinearColor ReplyColor;
};

struct FRuntimeConversationLayout {
  float PanelPadding;
  float TitleSize;
  float BodySize;
};

struct FRuntimeConversationViewModel {
  FRuntimeConversationText Text;
  FRuntimeConversationColors Colors;
  FRuntimeConversationLayout Layout;
};

} // namespace UI
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {

/**
 * @brief Unary request for the UI reducer that builds one chat message model.
 */
struct FChatMessageViewModelRequest {
  FString Role;
  FString Text;
};

/**
 * @brief Request object for reducing persisted dialogue history into view
 * models.
 */
struct FChatHistoryViewModelsRequest {
  TArray<FString> History;
};

/**
 * @brief Request object for composing the runtime conversation UI model.
 */
struct FRuntimeConversationViewModelRequest {
  FString NpcName;
  FString Role;
  FString PlayerLine;
  FString NpcReply;
};

struct FBindDialogueViewModelRequest {
  FString Persona;
  TArray<FString> History;
  bool bBound = false;
};

struct FChatInputViewModelRequest {
  FText Text;
  ETextCommit::Type CommitMethod = ETextCommit::Default;
  bool bDialogueBound = false;
};

struct FDialogueResponseViewModelRequest {
  FString Persona;
  FString NPCText;
  bool bBound = false;
};

struct FBindDialogueViewModel {
  bool bBound = false;
  FString Persona;
  ForbocAI::Game::UI::FChatMessageViewModel ConnectionMessage;
  TArray<ForbocAI::Game::UI::FChatMessageViewModel> HistoryMessages;
};

struct FChatInputViewModel {
  bool bShouldSend = false;
  FString InputText;
  ForbocAI::Game::UI::FChatMessageViewModel PlayerMessage;
  ForbocAI::Game::UI::FChatMessageViewModel ErrorMessage;
};

struct FDialogueResponseViewModel {
  ForbocAI::Game::UI::FChatMessageViewModel Message;
};

/**
 * @brief RTK action payload for UI reducers.
 *
 * Architecture: UI actions carry already-decided presentation state. Views only
 * render these fields; gameplay decisions remain in dialogue/runtime reducers.
 */
struct FUIPayload {
  FString Id;
  ForbocAI::Game::UI::FRuntimeConversationViewModel Conversation;
  TArray<ForbocAI::Game::UI::FChatMessageViewModel> Messages;
};

/**
 * @brief Slice state owned by the UI system.
 *
 * Architecture: This is durable UI state in the single store, not widget-local
 * state. Selectors read it and views apply it.
 */
struct FUIState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  ForbocAI::Game::Data::FUISettings Settings;
  ForbocAI::Game::UI::FRuntimeConversationViewModel Conversation;
  TArray<ForbocAI::Game::UI::FChatMessageViewModel> Messages;
  bool bConversationVisible = false;
};

/** User Story: As a features systems ui consumer, I need to compare values for equality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator==( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left, const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) */
inline bool operator==(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) {
  return Left.Text.Title == Right.Text.Title &&
         Left.Text.PlayerLine == Right.Text.PlayerLine &&
         Left.Text.NpcReply == Right.Text.NpcReply &&
         Left.Colors.PanelColor == Right.Colors.PanelColor &&
         Left.Colors.TitleColor == Right.Colors.TitleColor &&
         Left.Colors.PlayerColor == Right.Colors.PlayerColor &&
         Left.Colors.ReplyColor == Right.Colors.ReplyColor &&
         FMath::IsNearlyEqual(Left.Layout.PanelPadding,
                              Right.Layout.PanelPadding) &&
         FMath::IsNearlyEqual(Left.Layout.TitleSize,
                              Right.Layout.TitleSize) &&
         FMath::IsNearlyEqual(Left.Layout.BodySize, Right.Layout.BodySize);
}

/** User Story: As a features systems ui consumer, I need to compare values for inequality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator!=( const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left, const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) */
inline bool operator!=(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems ui consumer, I need to compare values for equality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator==(const FUIState &Left, const FUIState &Right) */
inline bool operator==(const FUIState &Left,
                       const FUIState &Right) {
  return Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value) &&
         Left.Settings == Right.Settings &&
         Left.Conversation == Right.Conversation &&
         Left.Messages == Right.Messages &&
         Left.bConversationVisible == Right.bConversationVisible;
}

/** User Story: As a features systems ui consumer, I need to compare values for inequality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator!=(const FUIState &Left, const FUIState &Right) */
inline bool operator!=(const FUIState &Left,
                       const FUIState &Right) {
  return !(Left == Right);
}

/** User Story: As a features systems ui consumer, I need to compare values for equality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator==(const FUIPayload &Left, const FUIPayload &Right) */
inline bool operator==(const FUIPayload &Left,
                       const FUIPayload &Right) {
  return Left.Id == Right.Id && Left.Conversation == Right.Conversation &&
         Left.Messages == Right.Messages;
}

/** User Story: As a features systems ui consumer, I need to compare values for inequality through a stable signature so the features systems ui workflow remains explicit and composable. @fn inline bool operator!=(const FUIPayload &Left, const FUIPayload &Right) */
inline bool operator!=(const FUIPayload &Left,
                       const FUIPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
