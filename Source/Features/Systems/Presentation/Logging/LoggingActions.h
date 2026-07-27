#pragma once

#include "CoreMinimal.h"

namespace ForbocAI {
namespace Game {
namespace PresentationLoggingActions {

/** User Story: As a presentation diagnostics consumer, I need to log level startup through a stable signature so views only dispatch presentation effects. @fn void LogLevelViewStarted() */
void LogLevelViewStarted();

/** User Story: As a presentation diagnostics consumer, I need to log a townsperson reply through a stable signature so views only dispatch presentation effects. @fn void LogTownspersonReply(const FString &Reply) */
void LogTownspersonReply(const FString &Reply);

} // namespace PresentationLoggingActions
} // namespace Game
} // namespace ForbocAI
