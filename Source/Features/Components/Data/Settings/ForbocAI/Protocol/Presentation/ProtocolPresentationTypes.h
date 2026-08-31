#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FForbocAIFlowStatusTextSettings {
  FString Idle;
  FString Pending;
  FString Succeeded;
  FString Failed;
};

struct FLogSettings {
  FString StateFormat;
  FString Present;
  FString Absent;
  FForbocAIFlowStatusTextSettings Statuses;
};

struct FForbocAIFlowPresentationSettings {
  FString Pending;
  FString Succeeded;
  FString FailurePrefix;
};

/** User Story: As the protocol presentation state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIFlowStatusTextSettings &Left, const FForbocAIFlowStatusTextSettings &Right) */
inline bool operator==(const FForbocAIFlowStatusTextSettings &Left,
                       const FForbocAIFlowStatusTextSettings &Right) {
  return Left.Idle == Right.Idle && Left.Pending == Right.Pending &&
         Left.Succeeded == Right.Succeeded && Left.Failed == Right.Failed;
}

/** User Story: As the protocol presentation state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIFlowStatusTextSettings &Left, const FForbocAIFlowStatusTextSettings &Right) */
inline bool operator!=(const FForbocAIFlowStatusTextSettings &Left,
                       const FForbocAIFlowStatusTextSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the protocol logging state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FLogSettings &Left, const FLogSettings &Right) */
inline bool operator==(const FLogSettings &Left,
                       const FLogSettings &Right) {
  return Left.StateFormat == Right.StateFormat &&
         Left.Present == Right.Present && Left.Absent == Right.Absent &&
         Left.Statuses == Right.Statuses;
}

/** User Story: As the protocol logging state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FLogSettings &Left, const FLogSettings &Right) */
inline bool operator!=(const FLogSettings &Left,
                       const FLogSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the protocol presentation state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIFlowPresentationSettings &Left, const FForbocAIFlowPresentationSettings &Right) */
inline bool operator==(const FForbocAIFlowPresentationSettings &Left,
                       const FForbocAIFlowPresentationSettings &Right) {
  return Left.Pending == Right.Pending && Left.Succeeded == Right.Succeeded &&
         Left.FailurePrefix == Right.FailurePrefix;
}

/** User Story: As the protocol presentation state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIFlowPresentationSettings &Left, const FForbocAIFlowPresentationSettings &Right) */
inline bool operator!=(const FForbocAIFlowPresentationSettings &Left,
                       const FForbocAIFlowPresentationSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
