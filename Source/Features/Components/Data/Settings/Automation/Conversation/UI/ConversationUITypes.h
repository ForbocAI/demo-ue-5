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

struct FSpeaker {
  FString SpeakerName;
  FString SpeakerRole;
};

struct FExchange {
  FString PlayerLine;
  FString NpcReply;
};

struct FExpected {
  FString ExpectedTitle;
  FString ExpectedPlayerLine;
  FString ExpectedNpcReply;
};

struct FDialogue {
  FSpeaker Speaker;
  FExchange Exchange;
  FExpected Expected;
};

struct FSettings {
  FString Test;
  TArray<FString> Assertions;
  FMessage PlayerMessage;
  FHistory History;
  FSubmitted Submitted;
  FDialogue Dialogue;
};

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FMessage &Left, const FMessage &Right) */
inline bool operator==(const FMessage &Left, const FMessage &Right) {
  return Left.Role == Right.Role && Left.Text == Right.Text &&
         Left.ExpectedText == Right.ExpectedText &&
         FMath::IsNearlyEqual(Left.ExpectedGreen, Right.ExpectedGreen);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FMessage &Left, const FMessage &Right) */
inline bool operator!=(const FMessage &Left, const FMessage &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FLine &Left, const FLine &Right) */
inline bool operator==(const FLine &Left, const FLine &Right) {
  return Left.Index == Right.Index && Left.ExpectedText == Right.ExpectedText;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FLine &Left, const FLine &Right) */
inline bool operator!=(const FLine &Left, const FLine &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FHistory &Left, const FHistory &Right) */
inline bool operator==(const FHistory &Left, const FHistory &Right) {
  return Left.Lines == Right.Lines &&
         Left.ExpectedCount == Right.ExpectedCount &&
         Left.Tagged == Right.Tagged && Left.Untagged == Right.Untagged;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FHistory &Left, const FHistory &Right) */
inline bool operator!=(const FHistory &Left, const FHistory &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FSubmitted &Left, const FSubmitted &Right) */
inline bool operator==(const FSubmitted &Left, const FSubmitted &Right) {
  return Left.Input == Right.Input && Left.ExpectedText == Right.ExpectedText;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FSubmitted &Left, const FSubmitted &Right) */
inline bool operator!=(const FSubmitted &Left, const FSubmitted &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FSpeaker &Left, const FSpeaker &Right) */
inline bool operator==(const FSpeaker &Left, const FSpeaker &Right) {
  return Left.SpeakerName == Right.SpeakerName &&
         Left.SpeakerRole == Right.SpeakerRole;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FSpeaker &Left, const FSpeaker &Right) */
inline bool operator!=(const FSpeaker &Left, const FSpeaker &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FExchange &Left, const FExchange &Right) */
inline bool operator==(const FExchange &Left, const FExchange &Right) {
  return Left.PlayerLine == Right.PlayerLine &&
         Left.NpcReply == Right.NpcReply;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FExchange &Left, const FExchange &Right) */
inline bool operator!=(const FExchange &Left, const FExchange &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FExpected &Left, const FExpected &Right) */
inline bool operator==(const FExpected &Left, const FExpected &Right) {
  return Left.ExpectedTitle == Right.ExpectedTitle &&
         Left.ExpectedPlayerLine == Right.ExpectedPlayerLine &&
         Left.ExpectedNpcReply == Right.ExpectedNpcReply;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FExpected &Left, const FExpected &Right) */
inline bool operator!=(const FExpected &Left, const FExpected &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FDialogue &Left, const FDialogue &Right) */
inline bool operator==(const FDialogue &Left, const FDialogue &Right) {
  return Left.Speaker == Right.Speaker &&
         Left.Exchange == Right.Exchange &&
         Left.Expected == Right.Expected;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FDialogue &Left, const FDialogue &Right) */
inline bool operator!=(const FDialogue &Left, const FDialogue &Right) {
  return !(Left == Right);
}

/** User Story: As a automation conversation ui consumer, I need to compare values for equality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Test == Right.Test && Left.Assertions == Right.Assertions &&
         Left.PlayerMessage == Right.PlayerMessage &&
         Left.History == Right.History && Left.Submitted == Right.Submitted &&
         Left.Dialogue == Right.Dialogue;
}

/** User Story: As a automation conversation ui consumer, I need to compare values for inequality through a stable signature so the automation conversation ui workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace UI
} // namespace Conversation
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
