#include "Features/Systems/ForbocAI/Protocol/ProtocolSelectors.h"

#include "Core/frmt.hpp"
#include "Features/Systems/SystemsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ForbocAIProtocolSelectors {
namespace {

/** User Story: As a systems forboc ai protocol consumer, I need to invoke failure message through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString FailureMessage( const FForbocAIFlowState &Flow, const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings) */
FString FailureMessage(
    const FForbocAIFlowState &Flow,
    const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings) {
  return Settings.FailurePrefix + Flow.Evidence.Result.Error;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke flow status label through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString FlowStatusLabel( EForbocAIFlowStatus Status, const ForbocAI::Game::Data::FLogSettings &Settings) */
FString FlowStatusLabel(
    EForbocAIFlowStatus Status,
    const ForbocAI::Game::Data::FLogSettings &Settings) {
  const auto Labels =
      func::createDispatcher<EForbocAIFlowStatus, FString>({
          {EForbocAIFlowStatus::Idle,
           [&Settings]() { return Settings.Statuses.Idle; }},
          {EForbocAIFlowStatus::Pending,
           [&Settings]() { return Settings.Statuses.Pending; }},
          {EForbocAIFlowStatus::Succeeded,
           [&Settings]() { return Settings.Statuses.Succeeded; }},
          {EForbocAIFlowStatus::Failed,
           [&Settings]() { return Settings.Statuses.Failed; }},
      });
  const func::Maybe<FString> Label = func::dispatch(Labels, Status);
  return Label.hasValue ? Label.value : Settings.Statuses.Idle;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke evidence value through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString EvidenceValue( const FString &Value, const ForbocAI::Game::Data::FLogSettings &Settings) */
FString EvidenceValue(
    const FString &Value,
    const ForbocAI::Game::Data::FLogSettings &Settings) {
  return Value.IsEmpty() ? Settings.Absent : Value;
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke evidence presence through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString EvidencePresence( const FString &Value, const ForbocAI::Game::Data::FLogSettings &Settings) */
FString EvidencePresence(
    const FString &Value,
    const ForbocAI::Game::Data::FLogSettings &Settings) {
  return Value.IsEmpty() ? Settings.Absent : Settings.Present;
}

} // namespace

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select flow presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectFlowPresentation( const FForbocAIFlowState &Flow, const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings) */
FForbocAIFlowPresentation SelectFlowPresentation(
    const FForbocAIFlowState &Flow,
    const ForbocAI::Game::Data::FForbocAIFlowPresentationSettings &Settings) {
  const auto Projectors =
      func::createDispatcher<EForbocAIFlowStatus, FForbocAIFlowPresentation>({
          {EForbocAIFlowStatus::Idle,
           []() { return FForbocAIFlowPresentation{}; }},
          {EForbocAIFlowStatus::Pending,
           [&Settings]() {
             return FForbocAIFlowPresentation{Settings.Pending, true, false};
           }},
          {EForbocAIFlowStatus::Succeeded,
           [&Flow]() {
             return FForbocAIFlowPresentation{
                 Flow.Evidence.Result.Output, false, true};
           }},
          {EForbocAIFlowStatus::Failed,
           [&Flow, &Settings]() {
             return FForbocAIFlowPresentation{
                 FailureMessage(Flow, Settings), false, false};
           }},
      });
  const func::Maybe<FForbocAIFlowPresentation> Selected =
      func::dispatch(Projectors, Flow.Status);
  return Selected.hasValue ? Selected.value : FForbocAIFlowPresentation{};
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select npc presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectNpcPresentation(const FRuntimeState &State) */
FForbocAIFlowPresentation SelectNpcPresentation(const FRuntimeState &State) {
  return SelectFlowPresentation(
      State.ForbocAIProtocol.Npc,
      State.ForbocAIProtocol.Settings.Npc.Presentation);
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select npc presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectNpcPresentation() */
FForbocAIFlowPresentation SelectNpcPresentation() {
  return SelectNpcPresentation(RuntimeSelectors::SelectState());
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select ghost presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectGhostPresentation(const FRuntimeState &State) */
FForbocAIFlowPresentation SelectGhostPresentation(const FRuntimeState &State) {
  return SelectFlowPresentation(
      State.ForbocAIProtocol.Ghost,
      State.ForbocAIProtocol.Settings.Ghost.Presentation);
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select ghost presentation through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FForbocAIFlowPresentation SelectGhostPresentation() */
FForbocAIFlowPresentation SelectGhostPresentation() {
  return SelectGhostPresentation(RuntimeSelectors::SelectState());
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select protocol log summary through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn FString SelectProtocolLogSummary(const FRuntimeState &State) */
FString SelectProtocolLogSummary(const FRuntimeState &State) {
  const FProtocolState &Protocol = State.ForbocAIProtocol;
  const auto &Logging = Protocol.Settings.Logging;
  const FForbocAIFlowState &Npc = Protocol.Npc;
  const FForbocAIFlowState &Ghost = Protocol.Ghost;
  return frmt::RuntimeString(
      Logging.StateFormat,
      frmt::Args({
          frmt::Arg(FlowStatusLabel(Npc.Status, Logging)),
          frmt::Arg(EvidenceValue(Npc.Evidence.Trace.CorrelationId, Logging)),
          frmt::Arg(EvidenceValue(Npc.Evidence.Trace.Family, Logging)),
          frmt::Arg(EvidenceValue(Npc.Evidence.Trace.CommandKey, Logging)),
          frmt::Arg(EvidenceValue(Npc.Evidence.Result.ActorId, Logging)),
          frmt::Arg(EvidencePresence(Npc.Evidence.Result.Output, Logging)),
          frmt::Arg(EvidencePresence(Npc.Evidence.Result.Error, Logging)),
          frmt::Arg(FlowStatusLabel(Ghost.Status, Logging)),
          frmt::Arg(
              EvidenceValue(Ghost.Evidence.Trace.CorrelationId, Logging)),
          frmt::Arg(EvidenceValue(Ghost.Evidence.Trace.Family, Logging)),
          frmt::Arg(EvidenceValue(Ghost.Evidence.Trace.CommandKey, Logging)),
          frmt::Arg(EvidenceValue(Ghost.Evidence.Result.ActorId, Logging)),
          frmt::Arg(EvidenceValue(Ghost.Evidence.Result.SessionId, Logging)),
          frmt::Arg(EvidencePresence(Ghost.Evidence.Result.Output, Logging)),
          frmt::Arg(EvidencePresence(Ghost.Evidence.Result.Error, Logging)),
      }));
}

/** User Story: As a systems forboc ai protocol consumer, I need to invoke select npc action through a stable signature so the systems forboc ai protocol workflow remains explicit and composable. @fn func::Maybe<FString> SelectNpcAction(const FString &Output, const FString &ActionLinePrefix) */
func::Maybe<FString> SelectNpcAction(const FString &Output,
                                     const FString &ActionLinePrefix) {
  TArray<FString> Lines;
  Output.ParseIntoArrayLines(Lines, true);
  return func::fmap(
      func::find_array<FString>(
          Lines, [&ActionLinePrefix](const FString &Line) {
            return Line.StartsWith(ActionLinePrefix);
          }),
      [&ActionLinePrefix](const FString &Line) {
        return Line.RightChop(ActionLinePrefix.Len()).TrimStartAndEnd();
      });
}

} // namespace ForbocAIProtocolSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
