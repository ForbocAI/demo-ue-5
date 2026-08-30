#pragma once

#include "Core/fp.hpp"
#include "Features/Components/Data/Settings/ForbocAI/Protocol/SettingsForbocAIProtocolTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ProtocolCLI {

struct FCommandRequest {
  ForbocAI::Game::Data::FForbocAICommandSettings Command;
  ForbocAI::Game::Data::FForbocAIErrorSettings Errors;
  TArray<FString> Arguments;
};

struct FCommandExecution {
  FString Family;
  FString CommandKey;
  FString Output;
};

using FCommandOutcome = func::Either<FString, FCommandExecution>;

inline bool operator==(const FCommandRequest &Left,
                       const FCommandRequest &Right) {
  return Left.Command == Right.Command && Left.Errors == Right.Errors &&
         Left.Arguments == Right.Arguments;
}

inline bool operator!=(const FCommandRequest &Left,
                       const FCommandRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCommandExecution &Left,
                       const FCommandExecution &Right) {
  return Left.Family == Right.Family && Left.CommandKey == Right.CommandKey &&
         Left.Output == Right.Output;
}

inline bool operator!=(const FCommandExecution &Left,
                       const FCommandExecution &Right) {
  return !(Left == Right);
}

} // namespace ProtocolCLI
} // namespace Level
} // namespace Game
} // namespace ForbocAI
