#pragma once

#include "Features/Systems/ForbocAI/Protocol/CLI/CLITypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ProtocolCLI {

/** User Story: As demo protocol orchestration, I need one command resolved and dispatched exclusively through the public SDK CLI boundary. @fn FCommandOutcome ExecuteCommand(const FCommandRequest &Request) */
FCommandOutcome ExecuteCommand(const FCommandRequest &Request);

} // namespace ProtocolCLI
} // namespace Level
} // namespace Game
} // namespace ForbocAI
