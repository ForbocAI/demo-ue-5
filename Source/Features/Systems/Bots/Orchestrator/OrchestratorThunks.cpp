#include "Features/Systems/Bots/Orchestrator/OrchestratorThunks.h"

#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Features/Systems/Bots/SystemsBotsActions.h"
#include "Features/Systems/Bots/Orchestrator/OrchestratorActions.h"
#include "Features/Systems/Bots/Pipeline/PipelineActions.h"
#include "Features/Systems/Bots/Position/PositionActions.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/Actor.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace {

using FBotActionDispatch = void (*)(const FBotActionDispatchContext &);

struct FBotActionDispatchDeclaration {
  FString FBotSettings::*ActionType;
  FBotActionDispatch Dispatch;
};

void DispatchMoveAction(const FBotActionDispatchContext &Context);
void DispatchAttackAction(const FBotActionDispatchContext &Context);

template <typename Source> struct TBotRuntimeActionDeclaration {
  rtk::AnyAction (*Build)(const Source &);
};

struct FBotTickActionCatalog {};
struct FBotRegistrationActionCatalog {};

template <typename Declaration> struct TBotOrchestratorRegistry;

FString BotDisplayName(const FString &BotId, const FString &Persona) {
  return Persona.IsEmpty() ? BotId : Persona;
}

FBotEntity BotEntityPayload(const FBotRegistrationPayloadSource &Source) {
  return {Source.Id, BotDisplayName(Source.Id, Source.Persona),
          EBotEntityKind::Townsperson, EBotAlignment::Friendly,
          Source.Settings.bRegisteredBotActive};
}

FBotPositionMoved
BotPositionMovedPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

FBotPositionComponent
BotPositionComponentPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

FBotPipelinePayload BotPipelinePayload(const FString &BotId) {
  return {BotId};
}

FBotOrchestratorPayload BotOrchestratorPayload(const FString &BotId) {
  return {BotId};
}

rtk::AnyAction BotUpsertedAction(
    const FBotRegistrationPayloadSource &Source) {
  return BotActions::BotUpserted()(BotEntityPayload(Source));
}

rtk::AnyAction BotPositionMovedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionMoved()(
      BotPositionMovedPayload(Source));
}

rtk::AnyAction BotPositionUpsertedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionUpserted()(
      BotPositionComponentPayload(Source));
}

rtk::AnyAction PipelineObservedAction(const FString &BotId) {
  return BotPipelineActions::PipelineObserved()(BotPipelinePayload(BotId));
}

rtk::AnyAction OrchestratorObservedAction(const FString &BotId) {
  return BotOrchestratorActions::OrchestratorObserved()(
      BotOrchestratorPayload(BotId));
}

rtk::AnyAction PipelineObservedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return PipelineObservedAction(Source.Id);
}

rtk::AnyAction BotPositionMovedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return BotPositionMovedAction(Source);
}

rtk::AnyAction BotUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotUpsertedAction(Source.Registration);
}

rtk::AnyAction BotPositionUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotPositionUpsertedAction(Source.Position);
}

rtk::AnyAction OrchestratorObservedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return OrchestratorObservedAction(Source.Registration.Id);
}

template <> struct TBotOrchestratorRegistry<FBotTickActionCatalog> {
  static const TArray<TBotRuntimeActionDeclaration<FBotPositionPayloadSource>>
      &Declarations() {
    static const TArray<
        TBotRuntimeActionDeclaration<FBotPositionPayloadSource>>
        RegisteredDeclarations = {{PipelineObservedFromPositionSource},
                                  {BotPositionMovedFromPositionSource}};
    return RegisteredDeclarations;
  }
};

template <>
struct TBotOrchestratorRegistry<FBotRegistrationActionCatalog> {
  static const TArray<
      TBotRuntimeActionDeclaration<FBotRegistrationDispatchSource>>
      &Declarations() {
    static const TArray<
        TBotRuntimeActionDeclaration<FBotRegistrationDispatchSource>>
        RegisteredDeclarations = {
            {BotUpsertedFromRegistrationSource},
            {BotPositionUpsertedFromRegistrationSource},
            {OrchestratorObservedFromRegistrationSource}};
    return RegisteredDeclarations;
  }
};

template <> struct TBotOrchestratorRegistry<FBotActionDispatchDeclaration> {
  static const TArray<FBotActionDispatchDeclaration> &Declarations() {
    static const TArray<FBotActionDispatchDeclaration>
        RegisteredDeclarations = {
            {&FBotSettings::MoveActionType, DispatchMoveAction},
            {&FBotSettings::AttackActionType, DispatchAttackAction}};
    return RegisteredDeclarations;
  }
};

void DispatchRuntimeAction(const rtk::AnyAction &Action) {
  RuntimeDispatch::Dispatch(Action);
}

template <typename Source>
TArray<rtk::AnyAction> RuntimeActions(
    const Source &SourceValue,
    const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) {
  return func::map_array<TBotRuntimeActionDeclaration<Source>,
                         rtk::AnyAction>(
      Declarations,
      [&SourceValue](
          const TBotRuntimeActionDeclaration<Source> &Declaration) {
        return Declaration.Build(SourceValue);
      });
}

void DispatchRuntimeActions(const TArray<rtk::AnyAction> &Actions) {
  func::for_each_array<rtk::AnyAction>(Actions, DispatchRuntimeAction);
}

template <typename Source>
void DispatchRuntimeActions(
    const Source &SourceValue,
    const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) {
  DispatchRuntimeActions(RuntimeActions<Source>(SourceValue, Declarations));
}

void DispatchMoveAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(BotPositionMovedAction(
      {Context.Id, Context.InitialLocalPoint,
       Context.BotActor->GetActorLocation() + Context.Settings.MoveActionOffset,
       Context.Settings}));
}

void DispatchAttackAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(OrchestratorObservedAction(Context.Id));
}

FString ObservationDisplayName(const FBotObservationSource &Source) {
  return Source.Bot.hasValue ? Source.Bot.value.DisplayName : Source.BotId;
}

FVector ObservationWorldLocation(const FBotObservationSource &Source) {
  return Source.Position.hasValue && Source.Position.value.bHasWorldLocation
             ? Source.Position.value.WorldLocation
             : Source.Settings.InitialPosition;
}

int32 ObservationBehaviorState(const FBotObservationSource &Source) {
  return Source.AI.hasValue
             ? static_cast<int32>(Source.AI.value.BehaviorState)
             : Source.Settings.DefaultBehaviorState;
}

} // namespace

void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source) {
  DispatchRuntimeActions<FBotPositionPayloadSource>(
      Source, TBotOrchestratorRegistry<FBotTickActionCatalog>::Declarations());
}

void DispatchRuntimeActionsForRegistration(
    const FBotRegistrationDispatchSource &Source) {
  DispatchRuntimeActions<FBotRegistrationDispatchSource>(
      Source,
      TBotOrchestratorRegistry<FBotRegistrationActionCatalog>::Declarations());
}

bool RunBotActionDispatch(const FBotActionDispatchRequest &Request) {
  const TArray<FBotActionDispatchDeclaration> &Declarations =
      TBotOrchestratorRegistry<FBotActionDispatchDeclaration>::Declarations();
  return func::fold_array<FBotActionDispatchDeclaration, bool>(
      Declarations, false,
      [&Request](bool bHandled,
                 const FBotActionDispatchDeclaration &Declaration) {
        return bHandled ||
                       (Request.Context.Settings.*(Declaration.ActionType)) !=
                           Request.ActionType
                   ? bHandled
                   : (Declaration.Dispatch(Request.Context), true);
      });
}

FString BotStateObservation(const FBotObservationSource &Source) {
  return frmt::RuntimeString(
      Source.Settings.StateObservationFormat,
      frmt::Args(
          {frmt::Arg(Source.BotId),
           frmt::Arg(ObservationDisplayName(Source)),
           frmt::Arg(ObservationWorldLocation(Source).ToString()),
           frmt::Arg(ObservationBehaviorState(Source))}));
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
