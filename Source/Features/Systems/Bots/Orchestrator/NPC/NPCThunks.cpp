#include "Features/Systems/Bots/Orchestrator/NPC/NPCThunks.h"

#include "Systems/CLI/NPC/NPCThunks.h"
#include "Systems/Protocol/ProtocolThunks.h"
#include "Systems/Store/StoreAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotNpcThunks {

/** User Story: As bot orchestration, I need SDK NPC registration to use the package-owned root store through one thunk boundary. @fn FNPCInternalState RegisterNpc(const FString &Persona) */
FNPCInternalState RegisterNpc(const FString &Persona) {
  return Ops::createNpc(StoreAdapters::RootStore(), Persona);
}

/** User Story: As bot orchestration, I need observations processed through the SDK NPC protocol while API-owned cognition remains outside the game. @fn func::AsyncResult<FAgentResponse> ProcessObservation(const FProtocolProcessInput &Input) */
func::AsyncResult<FAgentResponse>
ProcessObservation(const FProtocolProcessInput &Input) {
  return StoreAdapters::RootStore().dispatch(
      rtk::processNPC(Input, rtk::LocalProtocolHandlerContext(Input.NpcId)));
}

} // namespace BotNpcThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
