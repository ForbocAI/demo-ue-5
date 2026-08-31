#include "Features/Systems/ForbocAI/Protocol/NPC/NPCThunks.h"

#include "Features/Systems/ForbocAI/Protocol/CLI/CLIAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAINpcThunks {
namespace {

using FArgumentsExecutor =
    std::function<ProtocolCLI::FCommandOutcome(const TArray<FString> &)>;
using FNpcCompletion = std::function<
    FForbocAINpcResult(const ProtocolCLI::FCommandExecution &)>;

/** User Story: As a forboc ai protocol npc consumer, I need to invoke execute through a stable signature so the forboc ai protocol npc workflow remains explicit and composable. @fn FArgumentsExecutor Execute( const ForbocAI::Game::Data::FProtocolSettings &Settings, const ForbocAI::Game::Data::FForbocAICommandSettings &Command) */
FArgumentsExecutor Execute(
    const ForbocAI::Game::Data::FProtocolSettings &Settings,
    const ForbocAI::Game::Data::FForbocAICommandSettings &Command) {
  return [Command, Errors = Settings.Errors](
             const TArray<FString> &Arguments) {
    return ProtocolCLI::ExecuteCommand({Command, Errors, Arguments});
  };
}

/** User Story: As a forboc ai protocol npc consumer, I need to invoke complete npc interaction through a stable signature so the forboc ai protocol npc workflow remains explicit and composable. @fn FNpcCompletion CompleteNpcInteraction( const FForbocAINpcRequest &Request, const ProtocolCLI::FCommandExecution &Created) */
FNpcCompletion CompleteNpcInteraction(
    const FForbocAINpcRequest &Request,
    const ProtocolCLI::FCommandExecution &Created) {
  return [Request, Created](const ProtocolCLI::FCommandExecution &Processed) {
    FRuntimeTownspersonInteractionSource Source = Request.Source;
    Source.PinnedResponse = Processed.Output;
    return FForbocAINpcResult{
        {{Request.CorrelationId, Processed.Family, Processed.CommandKey},
         {Created.Output, FString(), Processed.Output, FString()}},
        Source};
  };
}

} // namespace

/** User Story: As a forboc ai protocol npc consumer, I need to invoke execute npc interaction through a stable signature so the forboc ai protocol npc workflow remains explicit and composable. @fn FNpcOutcome ExecuteNpcInteraction(const FForbocAINpcRequest &Request, const FRuntimeState &State) */
FNpcOutcome ExecuteNpcInteraction(const FForbocAINpcRequest &Request,
                                  const FRuntimeState &State) {
  const ProtocolCLI::FCommandOutcome Created =
      CreateNpc(Request.Source.Persona, State);
  return func::ebind(
      Created,
      [&Request, &State](const ProtocolCLI::FCommandExecution &Actor) {
        return func::either_map(
            ProcessNpc(Actor.Output, State)(
                Request.Source.DefaultPlayerLine),
            CompleteNpcInteraction(Request, Actor));
      });
}

/** User Story: As a forboc ai protocol npc consumer, I need to invoke create npc through a stable signature so the forboc ai protocol npc workflow remains explicit and composable. @fn ProtocolCLI::FCommandOutcome CreateNpc(const FString &Persona, const FRuntimeState &State) */
ProtocolCLI::FCommandOutcome CreateNpc(const FString &Persona,
                                       const FRuntimeState &State) {
  const auto &Settings = State.ForbocAIProtocol.Settings;
  return Execute(Settings, Settings.Npc.Commands.Create)({Persona});
}

/** User Story: As a forboc ai protocol npc consumer, I need to invoke process npc through a stable signature so the forboc ai protocol npc workflow remains explicit and composable. @fn FNpcObservationProcessor ProcessNpc(const FString &NpcId, const FRuntimeState &State) */
FNpcObservationProcessor ProcessNpc(const FString &NpcId,
                                    const FRuntimeState &State) {
  const ForbocAI::Game::Data::FProtocolSettings Settings =
      State.ForbocAIProtocol.Settings;
  return [NpcId, Settings](const FString &Observation) {
    return Execute(Settings, Settings.Npc.Commands.Process)(
        {NpcId, Observation});
  };
}

} // namespace ForbocAINpcThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
