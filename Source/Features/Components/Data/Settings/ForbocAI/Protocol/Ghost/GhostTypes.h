#pragma once

#include "Features/Components/Data/Settings/ForbocAI/Protocol/Command/ProtocolCommandTypes.h"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/Presentation/ProtocolPresentationTypes.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FForbocAIGhostCommandSettings {
  FForbocAICommandSettings Run;
  FForbocAICommandSettings Create;
  FForbocAICommandSettings Decide;
  FForbocAICommandSettings Results;
  FForbocAICommandSettings Stop;
};

struct FForbocAIGhostRunRequestSettings {
  FString Suite;
  int32 DurationSeconds = int32{};
  FString DurationOption;
};

struct FForbocAIGhostActorRequestSettings {
  FString Persona;
};

struct FForbocAIGhostDecisionRequestSettings {
  FString ObservationFormat;
  FString LegalActionsOption;
  FString LegalActionsSeparator;
  TArray<FString> LegalActions;
};

struct FForbocAIGhostRequestSettings {
  FForbocAIGhostRunRequestSettings Run;
  FForbocAIGhostActorRequestSettings Actor;
  FForbocAIGhostDecisionRequestSettings Decision;
};

struct FForbocAIGhostSettings {
  FForbocAIGhostCommandSettings Commands;
  FForbocAIGhostRequestSettings Request;
  FForbocAIFlowPresentationSettings Presentation;
};

/** User Story: As the Ghost protocol state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostCommandSettings &Left, const FForbocAIGhostCommandSettings &Right) */
inline bool operator==(const FForbocAIGhostCommandSettings &Left,
                       const FForbocAIGhostCommandSettings &Right) {
  return Left.Run == Right.Run && Left.Create == Right.Create &&
         Left.Decide == Right.Decide && Left.Results == Right.Results &&
         Left.Stop == Right.Stop;
}

/** User Story: As the Ghost protocol state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostCommandSettings &Left, const FForbocAIGhostCommandSettings &Right) */
inline bool operator!=(const FForbocAIGhostCommandSettings &Left,
                       const FForbocAIGhostCommandSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the Ghost run request state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostRunRequestSettings &Left, const FForbocAIGhostRunRequestSettings &Right) */
inline bool operator==(const FForbocAIGhostRunRequestSettings &Left,
                       const FForbocAIGhostRunRequestSettings &Right) {
  return Left.Suite == Right.Suite &&
         Left.DurationSeconds == Right.DurationSeconds &&
         Left.DurationOption == Right.DurationOption;
}

/** User Story: As the Ghost run request state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostRunRequestSettings &Left, const FForbocAIGhostRunRequestSettings &Right) */
inline bool operator!=(const FForbocAIGhostRunRequestSettings &Left,
                       const FForbocAIGhostRunRequestSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the Ghost actor request state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostActorRequestSettings &Left, const FForbocAIGhostActorRequestSettings &Right) */
inline bool operator==(const FForbocAIGhostActorRequestSettings &Left,
                       const FForbocAIGhostActorRequestSettings &Right) {
  return Left.Persona == Right.Persona;
}

/** User Story: As the Ghost actor request state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostActorRequestSettings &Left, const FForbocAIGhostActorRequestSettings &Right) */
inline bool operator!=(const FForbocAIGhostActorRequestSettings &Left,
                       const FForbocAIGhostActorRequestSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the Ghost decision request state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostDecisionRequestSettings &Left, const FForbocAIGhostDecisionRequestSettings &Right) */
inline bool operator==(const FForbocAIGhostDecisionRequestSettings &Left,
                       const FForbocAIGhostDecisionRequestSettings &Right) {
  return Left.ObservationFormat == Right.ObservationFormat &&
         Left.LegalActionsOption == Right.LegalActionsOption &&
         Left.LegalActionsSeparator == Right.LegalActionsSeparator &&
         Left.LegalActions == Right.LegalActions;
}

/** User Story: As the Ghost decision request state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostDecisionRequestSettings &Left, const FForbocAIGhostDecisionRequestSettings &Right) */
inline bool operator!=(const FForbocAIGhostDecisionRequestSettings &Left,
                       const FForbocAIGhostDecisionRequestSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the Ghost request state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostRequestSettings &Left, const FForbocAIGhostRequestSettings &Right) */
inline bool operator==(const FForbocAIGhostRequestSettings &Left,
                       const FForbocAIGhostRequestSettings &Right) {
  return Left.Run == Right.Run && Left.Actor == Right.Actor &&
         Left.Decision == Right.Decision;
}

/** User Story: As the Ghost request state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostRequestSettings &Left, const FForbocAIGhostRequestSettings &Right) */
inline bool operator!=(const FForbocAIGhostRequestSettings &Left,
                       const FForbocAIGhostRequestSettings &Right) {
  return !(Left == Right);
}

/** User Story: As the Ghost protocol state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FForbocAIGhostSettings &Left, const FForbocAIGhostSettings &Right) */
inline bool operator==(const FForbocAIGhostSettings &Left,
                       const FForbocAIGhostSettings &Right) {
  return Left.Commands == Right.Commands && Left.Request == Right.Request &&
         Left.Presentation == Right.Presentation;
}

/** User Story: As the Ghost protocol state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FForbocAIGhostSettings &Left, const FForbocAIGhostSettings &Right) */
inline bool operator!=(const FForbocAIGhostSettings &Left,
                       const FForbocAIGhostSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
