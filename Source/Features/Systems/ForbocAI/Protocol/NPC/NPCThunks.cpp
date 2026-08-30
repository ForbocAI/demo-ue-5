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

FArgumentsExecutor Execute(
    const ForbocAI::Game::Data::FProtocolSettings &Settings,
    const ForbocAI::Game::Data::FForbocAICommandSettings &Command) {
  return [Command, Errors = Settings.Errors](
             const TArray<FString> &Arguments) {
    return ProtocolCLI::ExecuteCommand({Command, Errors, Arguments});
  };
}

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

ProtocolCLI::FCommandOutcome CreateNpc(const FString &Persona,
                                       const FRuntimeState &State) {
  const auto &Settings = State.ForbocAIProtocol.Settings;
  return Execute(Settings, Settings.Npc.Commands.Create)({Persona});
}

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
