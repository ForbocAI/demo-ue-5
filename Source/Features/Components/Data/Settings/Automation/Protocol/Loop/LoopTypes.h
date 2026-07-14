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
  FString Bridge;
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
  FString Action;
  int32 MinimumRules;
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
  FString CreateRpgRules;
  FString ValidateRpgAction;
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
  FString RpgRulesNotEmpty;
  FString MoveActionValid;
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

inline bool operator==(const FPersonas &Left, const FPersonas &Right) {
  return Left.Agent == Right.Agent && Left.Immutable == Right.Immutable &&
         Left.State == Right.State && Left.Async == Right.Async &&
         Left.Bridge == Right.Bridge;
}

inline bool operator!=(const FPersonas &Left, const FPersonas &Right) {
  return !(Left == Right);
}

inline bool operator==(const FState &Left, const FState &Right) {
  return Left.Json == Right.Json && Left.Needle == Right.Needle;
}

inline bool operator!=(const FState &Left, const FState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAsync &Left, const FAsync &Right) {
  return Left.Prompt == Right.Prompt &&
         Left.ApiUrlVariable == Right.ApiUrlVariable &&
         Left.ApiKeyVariable == Right.ApiKeyVariable &&
         Left.FailurePrefix == Right.FailurePrefix &&
         Left.TimeoutSeconds == Right.TimeoutSeconds;
}

inline bool operator!=(const FAsync &Left, const FAsync &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBridge &Left, const FBridge &Right) {
  return Left.Action == Right.Action &&
         Left.MinimumRules == Right.MinimumRules;
}

inline bool operator!=(const FBridge &Left, const FBridge &Right) {
  return !(Left == Right);
}

inline bool operator==(const FGroups &Left, const FGroups &Right) {
  return Left.AgentCreation == Right.AgentCreation &&
         Left.StateUpdates == Right.StateUpdates &&
         Left.AsyncProcessPipeline == Right.AsyncProcessPipeline &&
         Left.BridgeValidation == Right.BridgeValidation;
}

inline bool operator!=(const FGroups &Left, const FGroups &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCaseLabels &Left, const FCaseLabels &Right) {
  return Left.CreateAgent == Right.CreateAgent &&
         Left.CreateImmutableAgent == Right.CreateImmutableAgent &&
         Left.WithStateUpdate == Right.WithStateUpdate &&
         Left.InvokeProcess == Right.InvokeProcess &&
         Left.CreateRpgRules == Right.CreateRpgRules &&
         Left.ValidateRpgAction == Right.ValidateRpgAction;
}

inline bool operator!=(const FCaseLabels &Left, const FCaseLabels &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAgentAssertions &Left,
                       const FAgentAssertions &Right) {
  return Left.AgentIdNotEmpty == Right.AgentIdNotEmpty &&
         Left.PersonaMatches == Right.PersonaMatches &&
         Left.AgentPointerValid == Right.AgentPointerValid;
}

inline bool operator!=(const FAgentAssertions &Left,
                       const FAgentAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FImmutableAssertions &Left,
                       const FImmutableAssertions &Right) {
  return Left.PersonaPreserved == Right.PersonaPreserved &&
         Left.OriginalIdPreserved == Right.OriginalIdPreserved;
}

inline bool operator!=(const FImmutableAssertions &Left,
                       const FImmutableAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return Left.UpdatedStateContainsMood == Right.UpdatedStateContainsMood;
}

inline bool operator!=(const FStateAssertions &Left,
                       const FStateAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAsyncAssertions &Left,
                       const FAsyncAssertions &Right) {
  return Left.ApiUrlPresent == Right.ApiUrlPresent &&
         Left.ApiKeyPresent == Right.ApiKeyPresent &&
         Left.ResponsePayloadPresent == Right.ResponsePayloadPresent;
}

inline bool operator!=(const FAsyncAssertions &Left,
                       const FAsyncAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBridgeAssertions &Left,
                       const FBridgeAssertions &Right) {
  return Left.RpgRulesNotEmpty == Right.RpgRulesNotEmpty &&
         Left.MoveActionValid == Right.MoveActionValid;
}

inline bool operator!=(const FBridgeAssertions &Left,
                       const FBridgeAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAssertions &Left, const FAssertions &Right) {
  return Left.Agent == Right.Agent &&
         Left.Immutable == Right.Immutable &&
         Left.State == Right.State &&
         Left.Async == Right.Async &&
         Left.Bridge == Right.Bridge;
}

inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Groups == Right.Groups &&
         Left.Cases == Right.Cases && Left.Assertions == Right.Assertions &&
         Left.Personas == Right.Personas && Left.State == Right.State &&
         Left.Async == Right.Async && Left.Bridge == Right.Bridge;
}

inline bool operator!=(const FSettings &Left, const FSettings &Right) {
  return !(Left == Right);
}

} // namespace Loop
} // namespace Protocol
} // namespace Automation
} // namespace Data
} // namespace Game
} // namespace ForbocAI
