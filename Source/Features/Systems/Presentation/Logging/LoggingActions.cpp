#include "Features/Systems/Presentation/Logging/LoggingActions.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"


DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntime, Log, All);

namespace ForbocAI {
namespace Game {
namespace PresentationLoggingActions {

/** User Story: As a presentation diagnostics consumer, I need to log level startup through a stable signature so views only dispatch presentation effects. @fn void LogLevelViewStarted() */
void LogLevelViewStarted() {
  UE_LOG(LogForbocRuntime, Display,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV99AE70415D18));
}

/** User Story: As a presentation diagnostics consumer, I need to log a townsperson reply through a stable signature so views only dispatch presentation effects. @fn void LogTownspersonReply(const FString &Reply) */
void LogTownspersonReply(const FString &Reply) {
  UE_LOG(LogTemp, Display,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV03A110C67C3C), *Reply);
}

} // namespace PresentationLoggingActions
} // namespace Game
} // namespace ForbocAI
