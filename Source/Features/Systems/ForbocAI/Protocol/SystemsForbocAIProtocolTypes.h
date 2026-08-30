#pragma once

#include "Features/Components/Data/Settings/ForbocAI/Protocol/SettingsForbocAIProtocolTypes.h"
#include "Features/Systems/Interaction/Conversation/ConversationTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EForbocAIFlowStatus : uint8 {
  Idle,
  Pending,
  Succeeded,
  Failed
};

struct FCommandTrace {
  FString CorrelationId;
  FString Family;
  FString CommandKey;
};

struct FCommandResultEvidence {
  FString ActorId;
  FString SessionId;
  FString Output;
  FString Error;
};

struct FCommandEvidence {
  FCommandTrace Trace;
  FCommandResultEvidence Result;
};

struct FForbocAIFlowState {
  EForbocAIFlowStatus Status = EForbocAIFlowStatus::Idle;
  FCommandEvidence Evidence;
};

struct FProtocolState {
  ForbocAI::Game::Data::FProtocolSettings Settings;
  FForbocAIFlowState Npc;
  FForbocAIFlowState Ghost;
};

struct FForbocAINpcRequest {
  FString CorrelationId;
  FRuntimeTownspersonInteractionSource Source;
};

struct FForbocAINpcResult {
  FCommandEvidence Evidence;
  FRuntimeTownspersonInteractionSource Source;
};

struct FForbocAIGhostRequest {
  FString CorrelationId;
};

struct FForbocAIGhostResult {
  FCommandEvidence Evidence;
};

struct FForbocAIFlowPresentation {
  FString Message;
  bool bPending = false;
  bool bSucceeded = false;
};

inline bool operator==(const FCommandTrace &Left,
                       const FCommandTrace &Right) {
  return Left.CorrelationId == Right.CorrelationId &&
         Left.Family == Right.Family && Left.CommandKey == Right.CommandKey;
}

inline bool operator!=(const FCommandTrace &Left,
                       const FCommandTrace &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCommandResultEvidence &Left,
                       const FCommandResultEvidence &Right) {
  return Left.ActorId == Right.ActorId && Left.SessionId == Right.SessionId &&
         Left.Output == Right.Output && Left.Error == Right.Error;
}

inline bool operator!=(const FCommandResultEvidence &Left,
                       const FCommandResultEvidence &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCommandEvidence &Left,
                       const FCommandEvidence &Right) {
  return Left.Trace == Right.Trace && Left.Result == Right.Result;
}

inline bool operator!=(const FCommandEvidence &Left,
                       const FCommandEvidence &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIFlowState &Left,
                       const FForbocAIFlowState &Right) {
  return Left.Status == Right.Status && Left.Evidence == Right.Evidence;
}

inline bool operator!=(const FForbocAIFlowState &Left,
                       const FForbocAIFlowState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FProtocolState &Left,
                       const FProtocolState &Right) {
  return Left.Settings == Right.Settings && Left.Npc == Right.Npc &&
         Left.Ghost == Right.Ghost;
}

inline bool operator!=(const FProtocolState &Left,
                       const FProtocolState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAINpcRequest &Left,
                       const FForbocAINpcRequest &Right) {
  return Left.CorrelationId == Right.CorrelationId &&
         Left.Source == Right.Source;
}

inline bool operator!=(const FForbocAINpcRequest &Left,
                       const FForbocAINpcRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAINpcResult &Left,
                       const FForbocAINpcResult &Right) {
  return Left.Evidence == Right.Evidence && Left.Source == Right.Source;
}

inline bool operator!=(const FForbocAINpcResult &Left,
                       const FForbocAINpcResult &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostRequest &Left,
                       const FForbocAIGhostRequest &Right) {
  return Left.CorrelationId == Right.CorrelationId;
}

inline bool operator!=(const FForbocAIGhostRequest &Left,
                       const FForbocAIGhostRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIGhostResult &Left,
                       const FForbocAIGhostResult &Right) {
  return Left.Evidence == Right.Evidence;
}

inline bool operator!=(const FForbocAIGhostResult &Left,
                       const FForbocAIGhostResult &Right) {
  return !(Left == Right);
}

inline bool operator==(const FForbocAIFlowPresentation &Left,
                       const FForbocAIFlowPresentation &Right) {
  return Left.Message == Right.Message && Left.bPending == Right.bPending &&
         Left.bSucceeded == Right.bSucceeded;
}

inline bool operator!=(const FForbocAIFlowPresentation &Left,
                       const FForbocAIFlowPresentation &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
