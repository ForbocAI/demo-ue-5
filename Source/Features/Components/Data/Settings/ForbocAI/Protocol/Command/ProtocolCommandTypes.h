#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FForbocAICommandSettings {
  FString Family;
  FString KeyPrefix;
  TArray<FString> Tokens;
};

struct FForbocAIErrorSettings {
  FString CommandUnmatched;
  FString RouteFamilyMismatchFormat;
  FString ActionMissing;
};

/** User Story: As the protocol command state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAICommandSettings &Left, const FForbocAICommandSettings &Right) */
inline bool operator==(const FForbocAICommandSettings &Left,
                       const FForbocAICommandSettings &Right) {
  return Left.Family == Right.Family && Left.KeyPrefix == Right.KeyPrefix &&
         Left.Tokens == Right.Tokens;
}

/** User Story: As the protocol command state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAICommandSettings &Left, const FForbocAICommandSettings &Right) */
inline bool operator!=(const FForbocAICommandSettings &Left,
                       const FForbocAICommandSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the protocol error state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIErrorSettings &Left, const FForbocAIErrorSettings &Right) */
inline bool operator==(const FForbocAIErrorSettings &Left,
                       const FForbocAIErrorSettings &Right) {
  return Left.CommandUnmatched == Right.CommandUnmatched &&
         Left.RouteFamilyMismatchFormat == Right.RouteFamilyMismatchFormat &&
         Left.ActionMissing == Right.ActionMissing;
}

/** User Story: As the protocol error state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIErrorSettings &Left, const FForbocAIErrorSettings &Right) */
inline bool operator!=(const FForbocAIErrorSettings &Left,
                       const FForbocAIErrorSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
