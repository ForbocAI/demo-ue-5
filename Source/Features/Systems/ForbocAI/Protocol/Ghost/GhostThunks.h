#pragma once

#include "Core/fp.hpp"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIGhostThunks {

using FGhostOutcome = func::Either<FString, FForbocAIGhostResult>;

/** User Story: As the demo Ghost workflow, I need Ghost-only CLI commands composed around API-owned level analysis. @fn FGhostOutcome ExecuteGhostAnalysis(const FForbocAIGhostRequest &Request, const FRuntimeState &State) */
FGhostOutcome ExecuteGhostAnalysis(const FForbocAIGhostRequest &Request,
                                   const FRuntimeState &State);

} // namespace ForbocAIGhostThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
