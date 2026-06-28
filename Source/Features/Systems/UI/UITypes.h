#pragma once

#include "Core/rtk.hpp"
#include "Types/SlateEnums.h"

namespace ForbocAI {
namespace Demo {
namespace UI {

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
} // namespace Demo
} // namespace ForbocAI

namespace ForbocAI {
namespace Demo {
namespace Level {

struct FUIChatMessageViewModelRequest {
  FString Role;
  FString Text;
};

struct FUIChatHistoryViewModelsRequest {
  TArray<FString> History;
};

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
  ForbocAI::Demo::UI::FChatMessageViewModel ConnectionMessage;
  TArray<ForbocAI::Demo::UI::FChatMessageViewModel> HistoryMessages;
};

struct FUIChatInputViewModel {
  bool bShouldSend = false;
  FString InputText;
  ForbocAI::Demo::UI::FChatMessageViewModel PlayerMessage;
  ForbocAI::Demo::UI::FChatMessageViewModel ErrorMessage;
};

struct FUIDialogueResponseViewModel {
  ForbocAI::Demo::UI::FChatMessageViewModel Message;
};

struct FUIPayload {
  FString Id;
  ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation;
  TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Messages;
};

struct FUIState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  ForbocAI::Demo::UI::FRuntimeConversationViewModel Conversation;
  TArray<ForbocAI::Demo::UI::FChatMessageViewModel> Messages;
  bool bConversationVisible = false;
};

inline bool operator==(
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Right) {
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
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Left,
    const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FUIState &Left,
                       const FUIState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
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
} // namespace Demo
} // namespace ForbocAI
