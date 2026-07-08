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

struct FSettings {
  FString Spec;
  TArray<FString> Groups;
  TArray<FString> Cases;
  TArray<FString> Assertions;
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
