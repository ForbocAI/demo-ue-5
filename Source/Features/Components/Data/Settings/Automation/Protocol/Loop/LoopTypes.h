#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Protocol {
namespace Loop {

struct FPersonas {
  FString Agent;
  FString Immutable;
  FString State;
  FString Async;
};

struct FState {
  FString Json;
  FString Needle;
};

struct FAsync {
  FString Prompt;
  FString ApiUrlVariable;
  FString ApiKeyVariable;
  FString FailurePrefix;
  float TimeoutSeconds;
};

struct FBridge {
  FString ValidActionJson;
  FString InvalidActionJson;
};

struct FGroups {
  FString AgentCreation;
  FString StateUpdates;
  FString AsyncProcessPipeline;
  FString BridgeValidation;
};

struct FCaseLabels {
  FString CreateAgent;
  FString CreateImmutableAgent;
  FString WithStateUpdate;
  FString InvokeProcess;
  FString ValidateAcceptedAction;
  FString ValidateRejectedAction;
};

struct FAgentAssertions {
  FString AgentIdNotEmpty;
  FString PersonaMatches;
  FString AgentPointerValid;
};

struct FImmutableAssertions {
  FString PersonaPreserved;
  FString OriginalIdPreserved;
};

struct FStateAssertions {
  FString UpdatedStateContainsMood;
};

struct FAsyncAssertions {
  FString ApiUrlPresent;
  FString ApiKeyPresent;
  FString ResponsePayloadPresent;
};

struct FBridgeAssertions {
  FString AcceptedActionValid;
  FString RejectedActionInvalid;
};

struct FAssertions {
  FAgentAssertions Agent;
  FImmutableAssertions Immutable;
  FStateAssertions State;
  FAsyncAssertions Async;
  FBridgeAssertions Bridge;
};

struct FSettings {
  FString Spec;
  FGroups Groups;
  FCaseLabels Cases;
  FAssertions Assertions;
  FPersonas Personas;
  FState State;
  FAsync Async;
  FBridge Bridge;
};

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FPersonas &Left, const FPersonas &Right) */
inline bool operator==(const FPersonas &Left, const FPersonas &Right) {
  return Left.Agent == Right.Agent && Left.Immutable == Right.Immutable &&
         Left.State == Right.State && Left.Async == Right.Async;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FPersonas &Left, const FPersonas &Right) */
inline bool operator!=(const FPersonas &Left, const FPersonas &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FState &Left, const FState &Right) */
inline bool operator==(const FState &Left, const FState &Right) {
  return Left.Json == Right.Json && Left.Needle == Right.Needle;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FState &Left, const FState &Right) */
inline bool operator!=(const FState &Left, const FState &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FAsync &Left, const FAsync &Right) */
inline bool operator==(const FAsync &Left, const FAsync &Right) {
  return Left.Prompt == Right.Prompt &&
         Left.ApiUrlVariable == Right.ApiUrlVariable &&
         Left.ApiKeyVariable == Right.ApiKeyVariable &&
         Left.FailurePrefix == Right.FailurePrefix &&
         Left.TimeoutSeconds == Right.TimeoutSeconds;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FAsync &Left, const FAsync &Right) */
inline bool operator!=(const FAsync &Left, const FAsync &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FBridge &Left, const FBridge &Right) */
inline bool operator==(const FBridge &Left, const FBridge &Right) {
  return Left.ValidActionJson == Right.ValidActionJson &&
         Left.InvalidActionJson == Right.InvalidActionJson;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FBridge &Left, const FBridge &Right) */
inline bool operator!=(const FBridge &Left, const FBridge &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FGroups &Left, const FGroups &Right) */
inline bool operator==(const FGroups &Left, const FGroups &Right) {
  return Left.AgentCreation == Right.AgentCreation &&
         Left.StateUpdates == Right.StateUpdates &&
         Left.AsyncProcessPipeline == Right.AsyncProcessPipeline &&
         Left.BridgeValidation == Right.BridgeValidation;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FGroups &Left, const FGroups &Right) */
inline bool operator!=(const FGroups &Left, const FGroups &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) */
inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) {
  return Left.CreateAgent == Right.CreateAgent &&
         Left.CreateImmutableAgent == Right.CreateImmutableAgent &&
         Left.WithStateUpdate == Right.WithStateUpdate &&
         Left.InvokeProcess == Right.InvokeProcess &&
         Left.ValidateAcceptedAction == Right.ValidateAcceptedAction &&
         Left.ValidateRejectedAction == Right.ValidateRejectedAction;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) */
inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FAgentAssertions &Left, const FAgentAssertions &Right) */
inline bool operator==(const FAgentAssertions &Left,
                       const FAgentAssertions &Right) {
  return Left.AgentIdNotEmpty == Right.AgentIdNotEmpty &&
         Left.PersonaMatches == Right.PersonaMatches &&
         Left.AgentPointerValid == Right.AgentPointerValid;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FAgentAssertions &Left, const FAgentAssertions &Right) */
inline bool operator!=(const FAgentAssertions &Left,
                       const FAgentAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FImmutableAssertions &Left, const FImmutableAssertions &Right) */
inline bool operator==(const FImmutableAssertions &Left,
                       const FImmutableAssertions &Right) {
  return Left.PersonaPreserved == Right.PersonaPreserved &&
         Left.OriginalIdPreserved == Right.OriginalIdPreserved;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FImmutableAssertions &Left, const FImmutableAssertions &Right) */
inline bool operator!=(const FImmutableAssertions &Left,
                       const FImmutableAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FStateAssertions &Left, const FStateAssertions &Right) */
inline bool operator==(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return Left.UpdatedStateContainsMood == Right.UpdatedStateContainsMood;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FStateAssertions &Left, const FStateAssertions &Right) */
inline bool operator!=(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FAsyncAssertions &Left, const FAsyncAssertions &Right) */
inline bool operator==(const FAsyncAssertions &Left,
                       const FAsyncAssertions &Right) {
  return Left.ApiUrlPresent == Right.ApiUrlPresent &&
         Left.ApiKeyPresent == Right.ApiKeyPresent &&
         Left.ResponsePayloadPresent == Right.ResponsePayloadPresent;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FAsyncAssertions &Left, const FAsyncAssertions &Right) */
inline bool operator!=(const FAsyncAssertions &Left,
                       const FAsyncAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FBridgeAssertions &Left, const FBridgeAssertions &Right) */
inline bool operator==(const FBridgeAssertions &Left,
                       const FBridgeAssertions &Right) {
  return Left.AcceptedActionValid == Right.AcceptedActionValid &&
         Left.RejectedActionInvalid == Right.RejectedActionInvalid;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FBridgeAssertions &Left, const FBridgeAssertions &Right) */
inline bool operator!=(const FBridgeAssertions &Left,
                       const FBridgeAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FAssertions &Left, const FAssertions &Right) */
inline bool operator==(const FAssertions &Left, const FAssertions &Right) {
  return Left.Agent == Right.Agent &&
         Left.Immutable == Right.Immutable &&
         Left.State == Right.State &&
         Left.Async == Right.Async &&
         Left.Bridge == Right.Bridge;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FAssertions &Left, const FAssertions &Right) */
inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

/** User Story: As a automation protocol loop consumer, I need to compare values for equality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator==(const FSettings &Left, const FSettings &Right) */
inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Groups == Right.Groups &&
         Left.Cases == Right.Cases && Left.Assertions == Right.Assertions &&
         Left.Personas == Right.Personas && Left.State == Right.State &&
         Left.Async == Right.Async && Left.Bridge == Right.Bridge;
}

/** User Story: As a automation protocol loop consumer, I need to compare values for inequality through a stable signature so the automation protocol loop workflow remains explicit and composable. @fn inline bool operator!=(const FSettings &Left, const FSettings &Right) */
inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Loop
} // namespace Protocol
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
