#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Conversation {
namespace UI {

struct FMessage {
  FString Role;
  FString Text;
  FString ExpectedText;
  float ExpectedGreen;
};

struct FLine {
  int32 Index;
  FString ExpectedText;
};

struct FHistory {
  TArray<FString> Lines;
  int32 ExpectedCount;
  FLine Tagged;
  FLine Untagged;
};

struct FSubmitted {
  FString Input;
  FString ExpectedText;
};

struct FDialogue {
  FString SpeakerName;
  FString SpeakerRole;
  FString PlayerLine;
  FString NpcReply;
  FString ExpectedTitle;
  FString ExpectedPlayerLine;
  FString ExpectedNpcReply;
};

struct FSettings {
  FString Test;
  TArray<FString> Assertions;
  FMessage PlayerMessage;
  FHistory History;
  FSubmitted Submitted;
  FDialogue Dialogue;
};

inline bool operator==(const FMessage &Left, const FMessage &Right) {
  return Left.Role == Right.Role && Left.Text == Right.Text &&
         Left.ExpectedText == Right.ExpectedText &&
         FMath::IsNearlyEqual(Left.ExpectedGreen, Right.ExpectedGreen);
}

inline bool operator!=(const FMessage &Left, const FMessage &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLine &Left, const FLine &Right) {
  return Left.Index == Right.Index && Left.ExpectedText == Right.ExpectedText;
}

inline bool operator!=(const FLine &Left, const FLine &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHistory &Left, const FHistory &Right) {
  return Left.Lines == Right.Lines &&
         Left.ExpectedCount == Right.ExpectedCount &&
         Left.Tagged == Right.Tagged && Left.Untagged == Right.Untagged;
}

inline bool operator!=(const FHistory &Left, const FHistory &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSubmitted &Left, const FSubmitted &Right) {
  return Left.Input == Right.Input && Left.ExpectedText == Right.ExpectedText;
}

inline bool operator!=(const FSubmitted &Left, const FSubmitted &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDialogue &Left, const FDialogue &Right) {
  return Left.SpeakerName == Right.SpeakerName &&
         Left.SpeakerRole == Right.SpeakerRole &&
         Left.PlayerLine == Right.PlayerLine &&
         Left.NpcReply == Right.NpcReply &&
         Left.ExpectedTitle == Right.ExpectedTitle &&
         Left.ExpectedPlayerLine == Right.ExpectedPlayerLine &&
         Left.ExpectedNpcReply == Right.ExpectedNpcReply;
}

inline bool operator!=(const FDialogue &Left, const FDialogue &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Test == Right.Test && Left.Assertions == Right.Assertions &&
         Left.PlayerMessage == Right.PlayerMessage &&
         Left.History == Right.History && Left.Submitted == Right.Submitted &&
         Left.Dialogue == Right.Dialogue;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace UI
} // namespace Conversation
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
