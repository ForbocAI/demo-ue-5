#pragma once

#include "Components/NPC/NPCTypes.h"
#include "Components/Protocol/Process/ProtocolProcessTypes.h"
#include "Core/fp.hpp"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotNpcThunks {

/** User Story: As bot orchestration, I need SDK NPC registration to use the package-owned root store through one thunk boundary. @fn FNPCInternalState RegisterNpc(const FString &Persona) */
FNPCInternalState RegisterNpc(const FString &Persona);

/** User Story: As bot orchestration, I need observations processed through the SDK NPC protocol while API-owned cognition remains outside the game. @fn func::AsyncResult<FAgentResponse> ProcessObservation(const FProtocolProcessInput &Input) */
func::AsyncResult<FAgentResponse>
ProcessObservation(const FProtocolProcessInput &Input);

} // namespace BotNpcThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
