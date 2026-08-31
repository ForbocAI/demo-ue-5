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

/** User Story: As a forboc ai protocol cli state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FCommandRequest &Left, const FCommandRequest &Right) */
inline bool operator==(const FCommandRequest &Left,
                       const FCommandRequest &Right) {
  return Left.Command == Right.Command && Left.Errors == Right.Errors &&
         Left.Arguments == Right.Arguments;
}

/** User Story: As a forboc ai protocol cli state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FCommandRequest &Left, const FCommandRequest &Right) */
inline bool operator!=(const FCommandRequest &Left,
                       const FCommandRequest &Right) {
  return !(Left == Right);
}

/** User Story: As a forboc ai protocol cli state owner, I need values compared structurally so unchanged authored state does not trigger redundant transitions. @fn inline bool operator==(const FCommandExecution &Left, const FCommandExecution &Right) */
inline bool operator==(const FCommandExecution &Left,
                       const FCommandExecution &Right) {
  return Left.Family == Right.Family && Left.CommandKey == Right.CommandKey &&
         Left.Output == Right.Output;
}

/** User Story: As a forboc ai protocol cli state owner, I need inequality derived from structural equality so change detection keeps one comparison contract. @fn inline bool operator!=(const FCommandExecution &Left, const FCommandExecution &Right) */
inline bool operator!=(const FCommandExecution &Left,
                       const FCommandExecution &Right) {
  return !(Left == Right);
}

} // namespace ProtocolCLI
} // namespace Level
} // namespace Game
} // namespace ForbocAI
