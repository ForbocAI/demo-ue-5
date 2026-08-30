#include "Features/Systems/ForbocAI/Protocol/Ghost/GhostThunks.h"

#include "Core/frmt.hpp"
#include "Features/Systems/ForbocAI/Protocol/CLI/CLIAdapters.h"
#include "Features/Systems/SystemsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIGhostThunks {
namespace {

using FSettings = ForbocAI::Game::Data::FProtocolSettings;
using FExecution = ProtocolCLI::FCommandExecution;
using FExecutionOutcome = ProtocolCLI::FCommandOutcome;
using FArgumentsExecutor =
    std::function<FExecutionOutcome(const TArray<FString> &)>;

struct FFlowContext {
  FString CorrelationId;
  FString SessionId;
  FString ActorId;
  FString Observation;
  FString Output;
  FString CommandKey;
  FSettings Settings;
};

FArgumentsExecutor Execute(
    const FSettings &Settings,
    const ForbocAI::Game::Data::FForbocAICommandSettings &Command) {
  return [Command, Errors = Settings.Errors](
             const TArray<FString> &Arguments) {
    return ProtocolCLI::ExecuteCommand({Command, Errors, Arguments});
  };
}

FString GhostObservation(const FRuntimeState &State,
                         const FSettings &Settings) {
  return frmt::RuntimeString(
      Settings.Ghost.Request.Decision.ObservationFormat,
      frmt::Args(
          {frmt::Arg(RuntimeSelectors::SelectTownspeople(State).Num()),
           frmt::Arg(RuntimeSelectors::SelectHorses(State).Num()),
           frmt::Arg(RuntimeSelectors::SelectLandmarks(State).Num()),
           frmt::Arg(RuntimeSelectors::SelectNatureFeatures(State).Num())}));
}

func::Either<FString, FFlowContext>
StartGhost(const FForbocAIGhostRequest &Request,
           const FRuntimeState &State) {
  const FSettings &Settings = State.ForbocAIProtocol.Settings;
  return func::either_map(
      Execute(Settings, Settings.Ghost.Commands.Run)(
          {Settings.Ghost.Request.Run.Suite,
           Settings.Ghost.Request.Run.DurationOption,
           LexToString(Settings.Ghost.Request.Run.DurationSeconds)}),
      [&Request, &State, Settings](const FExecution &Started) {
        return FFlowContext{Request.CorrelationId, Started.Output,
                            FString(), GhostObservation(State, Settings),
                            FString(), FString(), Settings};
      });
}

func::Either<FString, FFlowContext>
CreateGhostActor(const FFlowContext &Context) {
  return func::either_map(
      Execute(Context.Settings, Context.Settings.Ghost.Commands.Create)(
          {Context.SessionId,
           Context.Settings.Ghost.Request.Actor.Persona}),
      [&Context](const FExecution &Created) {
        FFlowContext Next = Context;
        Next.ActorId = Created.Output;
        return Next;
      });
}

func::Either<FString, FFlowContext>
DecideGhost(const FFlowContext &Context) {
  const ForbocAI::Game::Data::FForbocAIGhostDecisionRequestSettings &Request =
      Context.Settings.Ghost.Request.Decision;
  return func::either_map(
      Execute(Context.Settings, Context.Settings.Ghost.Commands.Decide)(
          {Context.SessionId, Context.ActorId, Context.Observation,
           Request.LegalActionsOption,
           FString::Join(Request.LegalActions,
                         *Request.LegalActionsSeparator)}),
      [&Context](const FExecution &Decided) {
        FFlowContext Next = Context;
        Next.Output = Decided.Output;
        Next.CommandKey = Decided.CommandKey;
        return Next;
      });
}

func::Either<FString, FFlowContext>
ReadGhostResults(const FFlowContext &Context) {
  return func::either_map(
      Execute(Context.Settings, Context.Settings.Ghost.Commands.Results)(
          {Context.SessionId}),
      [&Context](const FExecution &) { return Context; });
}

FExecutionOutcome StopGhost(const FFlowContext &Context) {
  return Execute(Context.Settings, Context.Settings.Ghost.Commands.Stop)(
      {Context.SessionId});
}

func::Either<FString, FFlowContext>
FinalizeGhost(const FFlowContext &Context) {
  return func::either_map(
      StopGhost(Context),
      [&Context](const FExecution &) { return Context; });
}

FGhostOutcome CompleteGhost(const FFlowContext &Context) {
  return func::right<FString, FForbocAIGhostResult>(
      {{{Context.CorrelationId,
         Context.Settings.Ghost.Commands.Decide.Family,
         Context.CommandKey},
        {Context.ActorId, Context.SessionId, Context.Output, FString()}}});
}

FGhostOutcome RunGhostSession(const FFlowContext &Started) {
  const func::Either<FString, FFlowContext> Completed = func::ebind(
      func::ebind(
          func::ebind(CreateGhostActor(Started), DecideGhost),
          FinalizeGhost),
      ReadGhostResults);
  return func::ematch(
      Completed,
      [&Started](const FString &Error) {
        StopGhost(Started);
        return func::left<FString, FForbocAIGhostResult>(Error);
      },
      CompleteGhost);
}

} // namespace

FGhostOutcome ExecuteGhostAnalysis(const FForbocAIGhostRequest &Request,
                                   const FRuntimeState &State) {
  return func::ebind(StartGhost(Request, State), RunGhostSession);
}

} // namespace ForbocAIGhostThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
