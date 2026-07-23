#include "Features/Systems/Bots/Orchestrator/OrchestratorThunks.h"

#include "Core/frmt.hpp"
#include "Core/fp.hpp"
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch move action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchMoveAction(const FBotActionDispatchContext &Context) */
void DispatchMoveAction(const FBotActionDispatchContext &Context);
/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch attack action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchAttackAction(const FBotActionDispatchContext &Context) */
void DispatchAttackAction(const FBotActionDispatchContext &Context);

template <typename Source> struct TBotRuntimeActionDeclaration {
  /** User Story: As a systems bots orchestrator consumer, I need to invoke any action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction (*Build)(const Source &) */
  rtk::AnyAction (*Build)(const Source &);
};

struct FBotTickActionPlan {};
struct FBotRegistrationActionPlan {};

template <typename Declaration> struct TBotOrchestratorRegistry;

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot display name through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString BotDisplayName(const FString &BotId, const FString &Persona) */
FString BotDisplayName(const FString &BotId, const FString &Persona) {
  return Persona.IsEmpty() ? BotId : Persona;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot entity payload through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotEntity BotEntityPayload(const FBotRegistrationPayloadSource &Source) */
FBotEntity BotEntityPayload(const FBotRegistrationPayloadSource &Source) {
  return {Source.Id, BotDisplayName(Source.Id, Source.Persona),
          EBotEntityKind::Townsperson, EBotAlignment::Friendly,
          Source.Settings.bRegisteredBotActive};
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position moved payload through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotPositionMoved BotPositionMovedPayload(const FBotPositionPayloadSource &Source) */
FBotPositionMoved
BotPositionMovedPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position component payload through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotPositionComponent BotPositionComponentPayload(const FBotPositionPayloadSource &Source) */
FBotPositionComponent
BotPositionComponentPayload(const FBotPositionPayloadSource &Source) {
  return {Source.Id, Source.LocalLocation, Source.WorldLocation,
          Source.Settings.bPositionPayloadHasLocalLocation,
          Source.Settings.bPositionPayloadHasWorldLocation};
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot pipeline payload through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotPipelinePayload BotPipelinePayload(const FString &BotId) */
FBotPipelinePayload BotPipelinePayload(const FString &BotId) {
  return {BotId};
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot orchestrator payload through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FBotOrchestratorPayload BotOrchestratorPayload(const FString &BotId) */
FBotOrchestratorPayload BotOrchestratorPayload(const FString &BotId) {
  return {BotId};
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot upserted action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotUpsertedAction( const FBotRegistrationPayloadSource &Source) */
rtk::AnyAction BotUpsertedAction(
    const FBotRegistrationPayloadSource &Source) {
  return BotActions::BotUpserted()(BotEntityPayload(Source));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position moved action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotPositionMovedAction( const FBotPositionPayloadSource &Source) */
rtk::AnyAction BotPositionMovedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionMoved()(
      BotPositionMovedPayload(Source));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position upserted action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotPositionUpsertedAction( const FBotPositionPayloadSource &Source) */
rtk::AnyAction BotPositionUpsertedAction(
    const FBotPositionPayloadSource &Source) {
  return BotPositionActions::BotPositionUpserted()(
      BotPositionComponentPayload(Source));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke pipeline observed action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction PipelineObservedAction(const FString &BotId) */
rtk::AnyAction PipelineObservedAction(const FString &BotId) {
  return BotPipelineActions::PipelineObserved()(BotPipelinePayload(BotId));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke orchestrator observed action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction OrchestratorObservedAction(const FString &BotId) */
rtk::AnyAction OrchestratorObservedAction(const FString &BotId) {
  return BotOrchestratorActions::OrchestratorObserved()(
      BotOrchestratorPayload(BotId));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke pipeline observed from position source through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction PipelineObservedFromPositionSource( const FBotPositionPayloadSource &Source) */
rtk::AnyAction PipelineObservedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return PipelineObservedAction(Source.Id);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position moved from position source through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotPositionMovedFromPositionSource( const FBotPositionPayloadSource &Source) */
rtk::AnyAction BotPositionMovedFromPositionSource(
    const FBotPositionPayloadSource &Source) {
  return BotPositionMovedAction(Source);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot upserted from registration source through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotUpsertedFromRegistrationSource( const FBotRegistrationDispatchSource &Source) */
rtk::AnyAction BotUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotUpsertedAction(Source.Registration);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot position upserted from registration source through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction BotPositionUpsertedFromRegistrationSource( const FBotRegistrationDispatchSource &Source) */
rtk::AnyAction BotPositionUpsertedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return BotPositionUpsertedAction(Source.Position);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke orchestrator observed from registration source through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn rtk::AnyAction OrchestratorObservedFromRegistrationSource( const FBotRegistrationDispatchSource &Source) */
rtk::AnyAction OrchestratorObservedFromRegistrationSource(
    const FBotRegistrationDispatchSource &Source) {
  return OrchestratorObservedAction(Source.Registration.Id);
}

template <> struct TBotOrchestratorRegistry<FBotTickActionPlan> {
  /** User Story: As a systems bots orchestrator consumer, I need to invoke declarations through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn static const TArray<TBotRuntimeActionDeclaration<FBotPositionPayloadSource>> &Declarations() */
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
struct TBotOrchestratorRegistry<FBotRegistrationActionPlan> {
  /** User Story: As a systems bots orchestrator consumer, I need to invoke declarations through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn static const TArray< TBotRuntimeActionDeclaration<FBotRegistrationDispatchSource>> &Declarations() */
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
  /** User Story: As a systems bots orchestrator consumer, I need to invoke declarations through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn static const TArray<FBotActionDispatchDeclaration> &Declarations() */
  static const TArray<FBotActionDispatchDeclaration> &Declarations() {
    static const TArray<FBotActionDispatchDeclaration>
        RegisteredDeclarations = {
            {&FBotSettings::MoveActionType, DispatchMoveAction},
            {&FBotSettings::AttackActionType, DispatchAttackAction}};
    return RegisteredDeclarations;
  }
};

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeAction(const rtk::AnyAction &Action) */
void DispatchRuntimeAction(const rtk::AnyAction &Action) {
  RuntimeDispatch::Dispatch(Action);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke runtime actions through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn template <typename Source> TArray<rtk::AnyAction> RuntimeActions( const Source &SourceValue, const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeActions(const TArray<rtk::AnyAction> &Actions) */
void DispatchRuntimeActions(const TArray<rtk::AnyAction> &Actions) {
  func::for_each_array<rtk::AnyAction>(Actions, DispatchRuntimeAction);
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn template <typename Source> void DispatchRuntimeActions( const Source &SourceValue, const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) */
template <typename Source>
void DispatchRuntimeActions(
    const Source &SourceValue,
    const TArray<TBotRuntimeActionDeclaration<Source>> &Declarations) {
  DispatchRuntimeActions(RuntimeActions<Source>(SourceValue, Declarations));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch move action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchMoveAction(const FBotActionDispatchContext &Context) */
void DispatchMoveAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(BotPositionMovedAction(
      {Context.Id, Context.InitialLocalPoint,
       Context.BotActor->GetActorLocation() + Context.Settings.MoveActionOffset,
       Context.Settings}));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch attack action through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchAttackAction(const FBotActionDispatchContext &Context) */
void DispatchAttackAction(const FBotActionDispatchContext &Context) {
  DispatchRuntimeAction(OrchestratorObservedAction(Context.Id));
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke observation display name through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString ObservationDisplayName(const FBotObservationSource &Source) */
FString ObservationDisplayName(const FBotObservationSource &Source) {
  return Source.Bot.hasValue ? Source.Bot.value.DisplayName : Source.BotId;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke observation world location through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FVector ObservationWorldLocation(const FBotObservationSource &Source) */
FVector ObservationWorldLocation(const FBotObservationSource &Source) {
  return Source.Position.hasValue && Source.Position.value.bHasWorldLocation
             ? Source.Position.value.WorldLocation
             : Source.Settings.InitialPosition;
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke observation behavior state through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn int32 ObservationBehaviorState(const FBotObservationSource &Source) */
int32 ObservationBehaviorState(const FBotObservationSource &Source) {
  return Source.AI.hasValue
             ? static_cast<int32>(Source.AI.value.BehaviorState)
             : Source.Settings.DefaultBehaviorState;
}

} // namespace

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions for tick through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source) */
void DispatchRuntimeActionsForTick(const FBotPositionPayloadSource &Source) {
  DispatchRuntimeActions<FBotPositionPayloadSource>(
      Source, TBotOrchestratorRegistry<FBotTickActionPlan>::Declarations());
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke dispatch runtime actions for registration through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn void DispatchRuntimeActionsForRegistration( const FBotRegistrationDispatchSource &Source) */
void DispatchRuntimeActionsForRegistration(
    const FBotRegistrationDispatchSource &Source) {
  DispatchRuntimeActions<FBotRegistrationDispatchSource>(
      Source,
      TBotOrchestratorRegistry<FBotRegistrationActionPlan>::Declarations());
}

/** User Story: As a systems bots orchestrator consumer, I need to invoke run bot action dispatch through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn bool RunBotActionDispatch(const FBotActionDispatchRequest &Request) */
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

/** User Story: As a systems bots orchestrator consumer, I need to invoke bot state observation through a stable signature so the systems bots orchestrator workflow remains explicit and composable. @fn FString BotStateObservation(const FBotObservationSource &Source) */
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
