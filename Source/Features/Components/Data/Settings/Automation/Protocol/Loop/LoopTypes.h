#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace Automation {
namespace Protocol {
namespace Loop {

struct FGate {
  FString Variable;
  FString Warning;
};

struct FConnection {
  FString Url;
};

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

struct FBridgeAssertions {
  FString RpgRulesNotEmpty;
  FString MoveActionValid;
};

struct FAssertions {
  FAgentAssertions Agent;
  FImmutableAssertions Immutable;
  FStateAssertions State;
  FBridgeAssertions Bridge;
};

struct FSettings {
  FString Spec;
  FGroups Groups;
  FCaseLabels Cases;
  FAssertions Assertions;
  FGate Gate;
  FConnection Connection;
  FPersonas Personas;
  FState State;
  FAsync Async;
  FBridge Bridge;
};

inline bool operator==(const FGate &Left, const FGate &Right) {
  return Left.Variable == Right.Variable && Left.Warning == Right.Warning;
}

inline bool operator!=(const FGate &Left, const FGate &Right) {
  return !(Left == Right);
}

inline bool operator==(const FConnection &Left, const FConnection &Right) {
  return Left.Url == Right.Url;
}

inline bool operator!=(const FConnection &Left, const FConnection &Right) {
  return !(Left == Right);
}

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
  return Left.Prompt == Right.Prompt;
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
         Left.Bridge == Right.Bridge;
}

inline bool operator!=(const FAssertions &Left, const FAssertions &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSettings &Left, const FSettings &Right) {
  return Left.Spec == Right.Spec && Left.Groups == Right.Groups &&
         Left.Cases == Right.Cases && Left.Assertions == Right.Assertions &&
         Left.Gate == Right.Gate && Left.Connection == Right.Connection &&
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
