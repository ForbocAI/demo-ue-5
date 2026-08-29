#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FBotDiagnosticSettings {
  FString StartLog;
  FString RegisteredLogFormat;
  FString ProcessFailedLogFormat;
  FString ExecuteLogFormat;
  FString NullActorLabel;
};

/** User Story: As a bot settings consumer, I need to compare diagnostic settings so authored observability remains deterministic. @fn inline bool operator==(const FBotDiagnosticSettings &Left, const FBotDiagnosticSettings &Right) */
inline bool operator==(const FBotDiagnosticSettings &Left,
                       const FBotDiagnosticSettings &Right) {
  return Left.StartLog == Right.StartLog &&
         Left.RegisteredLogFormat == Right.RegisteredLogFormat &&
         Left.ProcessFailedLogFormat == Right.ProcessFailedLogFormat &&
         Left.ExecuteLogFormat == Right.ExecuteLogFormat &&
         Left.NullActorLabel == Right.NullActorLabel;
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
