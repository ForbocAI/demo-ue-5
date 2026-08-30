#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolActions.h"
#include "Features/Systems/ForbocAI/Protocol/CLI/CLIAdapters.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolSelectors.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolSlice.h"
#include "Features/Systems/ForbocAI/Protocol/ProtocolThunks.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsSelectors.h"
#include "Misc/AutomationTest.h"

namespace {

using FAutomationSettings =
    ForbocAI::Game::Data::Automation::Bot::FOrchestratorSettings;
using FProtocolSettings =
    ForbocAI::Game::Data::FProtocolSettings;

const ForbocAI::Game::Data::FSettings &Settings() {
  static const ForbocAI::Game::Data::FSettings Value =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  return Value;
}

const FAutomationSettings &AutomationSettings() {
  return Settings().Automation.Bot.Orchestrator;
}

const FProtocolSettings &ProtocolSettings() {
  return Settings().ForbocAI;
}

ForbocAI::Game::Level::FRuntimeTownspersonInteractionSource
InteractionSource(const ForbocAI::Game::Level::FTownspersonSeed &Townsperson) {
  return {Townsperson.Name, Townsperson.Role, Townsperson.Persona,
          Townsperson.Interaction.DefaultPlayerLine,
          Townsperson.Interaction.PinnedResponse};
}

} // namespace

DEFINE_SPEC(FForbocAIProtocolSpec, AutomationSettings().ProtocolSpec,
            EAutomationTestFlags::ProductFilter |
                EAutomationTestFlags_ApplicationContextMask)

void FForbocAIProtocolSpec::Define() {
  const FAutomationSettings Automation = AutomationSettings();
  const FProtocolSettings Protocol = ProtocolSettings();

  Describe(Automation.Groups.Protocol, [this, Automation, Protocol]() {
    It(Automation.Cases.Protocol.RejectRouteCrossover,
       [this, Automation, Protocol]() {
         ForbocAI::Game::Data::FForbocAICommandSettings Mismatched =
             Protocol.Npc.Commands.Create;
         Mismatched.KeyPrefix = Protocol.Ghost.Commands.Create.KeyPrefix;
         const auto Outcome =
             ForbocAI::Game::Level::ProtocolCLI::ExecuteCommand(
                 {Mismatched, Protocol.Errors,
                  {Protocol.Ghost.Request.Actor.Persona}});
         TestTrue(Automation.Assertions.Protocol.Core.RouteCrossoverRejected,
                  Outcome.isLeft);
       });

    It(Automation.Cases.Protocol.ProjectAction,
       [this, Automation, Protocol]() {
         check(!Protocol.Ghost.Request.Decision.LegalActions.IsEmpty());
         const FString Expected =
             Protocol.Ghost.Request.Decision.LegalActions.Top();
         const auto Action =
             ForbocAI::Game::Level::ForbocAIProtocolSelectors::SelectNpcAction(
                 Protocol.Npc.Presentation.ActionLinePrefix + Expected,
                 Protocol.Npc.Presentation.ActionLinePrefix);
         TestTrue(Automation.Assertions.Protocol.Core.ActionProjected,
                  Action.hasValue);
         func::match(
             Action,
             [this, Automation, Expected](const FString &Projected) {
               TestEqual(Automation.Assertions.Protocol.Core.ActionProjected,
                         Projected, Expected);
             },
             []() {});
       });

    It(Automation.Cases.Protocol.ReduceProtocolLifecycle,
       [this, Automation, Protocol]() {
         using namespace ForbocAI::Game::Level;
         const auto &Thunk =
             ForbocAIProtocolThunks::GhostAnalysisAsyncThunk();
         const FForbocAIGhostRequest Request{
             Protocol.Ghost.Request.Run.Suite};
         const auto &Reducer = ForbocAIProtocolSlice::GetSlice().Reducer;
         const FProtocolState Initial =
             ForbocAIProtocolSlice::CreateInitialState(Protocol);
         const FProtocolState Pending =
             Reducer(Initial, Thunk.pending(Request));
         TestEqual(Automation.Assertions.Protocol.Core.CorrelationPreserved,
                   Pending.Ghost.Evidence.Trace.CorrelationId,
                   Request.CorrelationId);
         const FProtocolState Failed =
             Reducer(Pending, Thunk.rejected(Protocol.Errors.ActionMissing));
         TestEqual(Automation.Assertions.Protocol.Core.ProtocolFailurePreserved,
                   Failed.Ghost.Evidence.Result.Error,
                   Protocol.Errors.ActionMissing);
       });

    LatentIt(
        Automation.Cases.Protocol.ExecuteLiveFlows,
        FTimespan::FromSeconds(Automation.ProtocolRun.TimeoutSeconds),
        [this, Automation, Protocol](const FDoneDelegate &Done) {
          using namespace ForbocAI::Game::Level;
          RuntimeActions::DispatchRequestLevelViewPayload()
              .then([this, Automation, Protocol, Done](
                        const FRuntimeLevelViewPayload &) {
                const TArray<FTownspersonSeed> Townspeople =
                    RuntimeSelectors::SelectTownspeople(
                        RuntimeSelectors::SelectState());
                const func::Maybe<FTownspersonSeed> Townsperson =
                    func::find_array<FTownspersonSeed>(
                        Townspeople,
                        [](const FTownspersonSeed &) { return true; });
                func::match(
                    Townsperson,
                    [this, Automation, Protocol, Done](
                        const FTownspersonSeed &Seed) {
                      const auto Flow =
                          func::AsyncChain::then<FForbocAINpcResult,
                                                 FForbocAIGhostResult>(
                              ForbocAIProtocolActions::DispatchNpcInteraction(
                                  InteractionSource(Seed)),
                              [this, Automation, Protocol](
                                  const FForbocAINpcResult &Npc) {
                                const auto &Assertions =
                                    Automation.Assertions.Protocol.Npc;
                                TestTrue(
                                    Assertions.Succeeded,
                                    RuntimeSelectors::SelectState()
                                            .ForbocAIProtocol.Npc.Status ==
                                        EForbocAIFlowStatus::Succeeded);
                                TestFalse(
                                    Assertions.Trace.CorrelationPresent,
                                    Npc.Evidence.Trace.CorrelationId.IsEmpty());
                                TestEqual(
                                    Assertions.Trace.FamilyMatched,
                                    Npc.Evidence.Trace.Family,
                                    Protocol.Npc.Commands.Process.Family);
                                TestTrue(
                                    Assertions.Trace.CommandMatched,
                                    Npc.Evidence.Trace.CommandKey.StartsWith(
                                        Protocol.Npc.Commands.Process
                                            .KeyPrefix));
                                TestFalse(
                                    Assertions.Result.ActorPresent,
                                    Npc.Evidence.Result.ActorId.IsEmpty());
                                TestFalse(
                                    Assertions.Result.OutputPresent,
                                    Npc.Evidence.Result.Output.IsEmpty());
                                TestTrue(Assertions.Result.ErrorAbsent,
                                         Npc.Evidence.Result.Error.IsEmpty());
                                return ForbocAIProtocolActions::
                                    DispatchGhostAnalysis();
                              });
                      Flow.then([this, Automation, Protocol, Done](
                                    const FForbocAIGhostResult &Ghost) {
                            const auto &Assertions =
                                Automation.Assertions.Protocol.Ghost;
                            const auto &LogAssertions =
                                Automation.Assertions.Protocol.Log;
                            const FRuntimeState &State =
                                RuntimeSelectors::SelectState();
                            TestTrue(
                                Assertions.Succeeded,
                                State.ForbocAIProtocol.Ghost.Status ==
                                    EForbocAIFlowStatus::Succeeded);
                            TestFalse(
                                Assertions.Trace.CorrelationPresent,
                                Ghost.Evidence.Trace.CorrelationId.IsEmpty());
                            TestEqual(
                                Assertions.Trace.FamilyMatched,
                                Ghost.Evidence.Trace.Family,
                                Protocol.Ghost.Commands.Decide.Family);
                            TestTrue(
                                Assertions.Trace.CommandMatched,
                                Ghost.Evidence.Trace.CommandKey.StartsWith(
                                    Protocol.Ghost.Commands.Decide.KeyPrefix));
                            TestFalse(
                                Assertions.Result.ActorPresent,
                                Ghost.Evidence.Result.ActorId.IsEmpty());
                            TestFalse(
                                Assertions.Result.SessionPresent,
                                Ghost.Evidence.Result.SessionId.IsEmpty());
                            TestFalse(
                                Assertions.Result.OutputPresent,
                                Ghost.Evidence.Result.Output.IsEmpty());
                            TestTrue(
                                Assertions.Result.QualifiedOutput,
                                !Automation.ProtocolRun
                                     .QualifiedDiagnosticMarker.IsEmpty() &&
                                    Ghost.Evidence.Result.Output.Contains(
                                        Automation.ProtocolRun
                                            .QualifiedDiagnosticMarker));
                            TestTrue(Assertions.Result.ErrorAbsent,
                                     Ghost.Evidence.Result.Error.IsEmpty());
                            const FString Summary =
                                ForbocAIProtocolSelectors::
                                    SelectProtocolLogSummary(State);
                            TestTrue(
                                LogAssertions.SummaryNpcCorrelation,
                                Summary.Contains(State.ForbocAIProtocol.Npc
                                                     .Evidence.Trace
                                                     .CorrelationId));
                            TestTrue(
                                LogAssertions.SummaryGhostCorrelation,
                                Summary.Contains(
                                    Ghost.Evidence.Trace.CorrelationId));
                            TestTrue(
                                LogAssertions.SummaryGhostSession,
                                Summary.Contains(
                                    Ghost.Evidence.Result.SessionId));
                            TestFalse(
                                LogAssertions.NpcOutputRedacted,
                                Summary.Contains(State.ForbocAIProtocol.Npc
                                                     .Evidence.Result.Output));
                            TestFalse(
                                LogAssertions.GhostOutputRedacted,
                                Summary.Contains(
                                    Ghost.Evidence.Result.Output));
                            Done.ExecuteIfBound();
                          })
                          .catch_([this, Automation, Done](std::string Error) {
                            AddError(Automation.ProtocolRun.FailurePrefix +
                                     UTF8_TO_TCHAR(Error.c_str()));
                            Done.ExecuteIfBound();
                          })
                          .execute();
                    },
                    [this, Automation, Done]() {
                      TestTrue(Automation.Assertions.Protocol.Core
                                   .TownspersonAvailable,
                               false);
                      Done.ExecuteIfBound();
                    });
              })
              .catch_([this, Automation, Done](std::string Error) {
                AddError(Automation.ProtocolRun.FailurePrefix +
                         UTF8_TO_TCHAR(Error.c_str()));
                Done.ExecuteIfBound();
              })
              .execute();
        });
  });
}
