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

struct FForbocAINpcCommandSettings {
  FForbocAICommandSettings Create;
  FForbocAICommandSettings Process;
};

struct FForbocAINpcPresentationSettings
    : FForbocAIFlowPresentationSettings {
  FString ActionLinePrefix;
};

struct FForbocAINpcSettings {
  FForbocAINpcCommandSettings Commands;
  FForbocAINpcPresentationSettings Presentation;
};

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

struct FProtocolSettings {
  FForbocAIErrorSettings Errors;
  FLogSettings Logging;
  FForbocAINpcSettings Npc;
  FForbocAIGhostSettings Ghost;
};

inline bool operator==(const FForbocAICommandSettings &Left,
                       const FForbocAICommandSettings &Right) {
  return Left.Family == Right.Family && Left.KeyPrefix == Right.KeyPrefix &&
         Left.Tokens == Right.Tokens;
}

inline bool operator!=(const FForbocAICommandSettings &Left,
                       const FForbocAICommandSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIErrorSettings &Left,
                       const FForbocAIErrorSettings &Right) {
  return Left.CommandUnmatched == Right.CommandUnmatched &&
         Left.RouteFamilyMismatchFormat == Right.RouteFamilyMismatchFormat &&
         Left.ActionMissing == Right.ActionMissing;
}

inline bool operator!=(const FForbocAIErrorSettings &Left,
                       const FForbocAIErrorSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIFlowStatusTextSettings &Left,
                       const FForbocAIFlowStatusTextSettings &Right) {
  return Left.Idle == Right.Idle && Left.Pending == Right.Pending &&
         Left.Succeeded == Right.Succeeded && Left.Failed == Right.Failed;
}

inline bool operator!=(const FForbocAIFlowStatusTextSettings &Left,
                       const FForbocAIFlowStatusTextSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLogSettings &Left,
                       const FLogSettings &Right) {
  return Left.StateFormat == Right.StateFormat &&
         Left.Present == Right.Present && Left.Absent == Right.Absent &&
         Left.Statuses == Right.Statuses;
}

inline bool operator!=(const FLogSettings &Left,
                       const FLogSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIFlowPresentationSettings &Left,
                       const FForbocAIFlowPresentationSettings &Right) {
  return Left.Pending == Right.Pending && Left.Succeeded == Right.Succeeded &&
         Left.FailurePrefix == Right.FailurePrefix;
}

inline bool operator!=(const FForbocAIFlowPresentationSettings &Left,
                       const FForbocAIFlowPresentationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAINpcCommandSettings &Left,
                       const FForbocAINpcCommandSettings &Right) {
  return Left.Create == Right.Create && Left.Process == Right.Process;
}

inline bool operator!=(const FForbocAINpcCommandSettings &Left,
                       const FForbocAINpcCommandSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAINpcPresentationSettings &Left,
                       const FForbocAINpcPresentationSettings &Right) {
  return static_cast<const FForbocAIFlowPresentationSettings &>(Left) ==
             static_cast<const FForbocAIFlowPresentationSettings &>(Right) &&
         Left.ActionLinePrefix == Right.ActionLinePrefix;
}

inline bool operator!=(const FForbocAINpcPresentationSettings &Left,
                       const FForbocAINpcPresentationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAINpcSettings &Left,
                       const FForbocAINpcSettings &Right) {
  return Left.Commands == Right.Commands &&
         Left.Presentation == Right.Presentation;
}

inline bool operator!=(const FForbocAINpcSettings &Left,
                       const FForbocAINpcSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostCommandSettings &Left,
                       const FForbocAIGhostCommandSettings &Right) {
  return Left.Run == Right.Run && Left.Create == Right.Create &&
         Left.Decide == Right.Decide && Left.Results == Right.Results &&
         Left.Stop == Right.Stop;
}

inline bool operator!=(const FForbocAIGhostCommandSettings &Left,
                       const FForbocAIGhostCommandSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostRunRequestSettings &Left,
                       const FForbocAIGhostRunRequestSettings &Right) {
  return Left.Suite == Right.Suite &&
         Left.DurationSeconds == Right.DurationSeconds &&
         Left.DurationOption == Right.DurationOption;
}

inline bool operator!=(const FForbocAIGhostRunRequestSettings &Left,
                       const FForbocAIGhostRunRequestSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostActorRequestSettings &Left,
                       const FForbocAIGhostActorRequestSettings &Right) {
  return Left.Persona == Right.Persona;
}

inline bool operator!=(const FForbocAIGhostActorRequestSettings &Left,
                       const FForbocAIGhostActorRequestSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostDecisionRequestSettings &Left,
                       const FForbocAIGhostDecisionRequestSettings &Right) {
  return Left.ObservationFormat == Right.ObservationFormat &&
         Left.LegalActionsOption == Right.LegalActionsOption &&
         Left.LegalActionsSeparator == Right.LegalActionsSeparator &&
         Left.LegalActions == Right.LegalActions;
}

inline bool operator!=(const FForbocAIGhostDecisionRequestSettings &Left,
                       const FForbocAIGhostDecisionRequestSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostRequestSettings &Left,
                       const FForbocAIGhostRequestSettings &Right) {
  return Left.Run == Right.Run && Left.Actor == Right.Actor &&
         Left.Decision == Right.Decision;
}

inline bool operator!=(const FForbocAIGhostRequestSettings &Left,
                       const FForbocAIGhostRequestSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostSettings &Left,
                       const FForbocAIGhostSettings &Right) {
  return Left.Commands == Right.Commands && Left.Request == Right.Request &&
         Left.Presentation == Right.Presentation;
}

inline bool operator!=(const FForbocAIGhostSettings &Left,
                       const FForbocAIGhostSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FProtocolSettings &Left,
                       const FProtocolSettings &Right) {
  return Left.Errors == Right.Errors && Left.Logging == Right.Logging &&
         Left.Npc == Right.Npc && Left.Ghost == Right.Ghost;
}

inline bool operator!=(const FProtocolSettings &Left,
                       const FProtocolSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
