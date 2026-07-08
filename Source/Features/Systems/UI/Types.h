#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Types.h"
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

inline bool operator==(const FChatMessageViewModel &Left,
                       const FChatMessageViewModel &Right) {
  return Left.Text == Right.Text && Left.Color == Right.Color;
}

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
struct FRuntimeConversationViewModel {
  FString Title;
  FString PlayerLine;
  FString NpcReply;
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor PlayerColor;
  FLinearColor ReplyColor;
  float PanelPadding;
  float TitleSize;
  float BodySize;
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
struct FUIChatMessageViewModelRequest {
  FString Role;
  FString Text;
};

/**
 * @brief Request object for reducing persisted dialogue history into view
 * models.
 */
struct FUIChatHistoryViewModelsRequest {
  TArray<FString> History;
};

/**
 * @brief Request object for composing the runtime conversation UI model.
 */
struct FUIRuntimeConversationViewModelRequest {
  FString NpcName;
  FString Role;
  FString PlayerLine;
  FString NpcReply;
};

struct FUIBindDialogueViewModelRequest {
  FString Persona;
  TArray<FString> History;
  bool bBound = false;
};

struct FUIChatInputViewModelRequest {
  FText Text;
  ETextCommit::Type CommitMethod = ETextCommit::Default;
  bool bDialogueBound = false;
};

struct FUIDialogueResponseViewModelRequest {
  FString Persona;
  FString NPCText;
  bool bBound = false;
};

struct FUIBindDialogueViewModel {
  bool bBound = false;
  FString Persona;
  ForbocAI::Game::UI::FChatMessageViewModel ConnectionMessage;
  TArray<ForbocAI::Game::UI::FChatMessageViewModel> HistoryMessages;
};

struct FUIChatInputViewModel {
  bool bShouldSend = false;
  FString InputText;
  ForbocAI::Game::UI::FChatMessageViewModel PlayerMessage;
  ForbocAI::Game::UI::FChatMessageViewModel ErrorMessage;
};

struct FUIDialogueResponseViewModel {
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

inline bool operator==(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) {
  return Left.Title == Right.Title && Left.PlayerLine == Right.PlayerLine &&
         Left.NpcReply == Right.NpcReply &&
         Left.PanelColor == Right.PanelColor &&
         Left.TitleColor == Right.TitleColor &&
         Left.PlayerColor == Right.PlayerColor &&
         Left.ReplyColor == Right.ReplyColor &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.TitleSize, Right.TitleSize) &&
         FMath::IsNearlyEqual(Left.BodySize, Right.BodySize);
}

inline bool operator!=(
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Game::UI::FRuntimeConversationViewModel &Right) {
  return !(Left == Right);
}

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

inline bool operator!=(const FUIState &Left,
                       const FUIState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FUIPayload &Left,
                       const FUIPayload &Right) {
  return Left.Id == Right.Id && Left.Conversation == Right.Conversation &&
         Left.Messages == Right.Messages;
}

inline bool operator!=(const FUIPayload &Left,
                       const FUIPayload &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
