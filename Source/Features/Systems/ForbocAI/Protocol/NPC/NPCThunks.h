#pragma once

#include "Core/fp.hpp"
#include "Features/Systems/ForbocAI/Protocol/CLI/CLITypes.h"
#include "Features/Systems/ForbocAI/Protocol/SystemsForbocAIProtocolTypes.h"
#include "Features/Systems/State/SystemsStateTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAINpcThunks {

using FNpcOutcome = func::Either<FString, FForbocAINpcResult>;
using FNpcObservationProcessor =
    std::function<ProtocolCLI::FCommandOutcome(const FString &)>;

/** User Story: As demo NPC registration, I need actor creation delegated through the authored SDK CLI command. @fn ProtocolCLI::FCommandOutcome CreateNpc(const FString &Persona, const FRuntimeState &State) */
ProtocolCLI::FCommandOutcome CreateNpc(const FString &Persona,
                                       const FRuntimeState &State);

/** User Story: As demo NPC observation, I need an SDK CLI processor partially applied to one NPC and root state. @fn FNpcObservationProcessor ProcessNpc(const FString &NpcId, const FRuntimeState &State) */
FNpcObservationProcessor ProcessNpc(const FString &NpcId,
                                    const FRuntimeState &State);

/** User Story: As the demo NPC workflow, I need create and process CLI commands composed without bypassing the SDK CLI. @fn FNpcOutcome ExecuteNpcInteraction(const FForbocAINpcRequest &Request, const FRuntimeState &State) */
FNpcOutcome ExecuteNpcInteraction(const FForbocAINpcRequest &Request,
                                  const FRuntimeState &State);

} // namespace ForbocAINpcThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
