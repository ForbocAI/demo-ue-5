#include "Features/Systems/SystemsListeners.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Systems/SystemsThunks.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeListener, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeListeners {
namespace {

using FRuntimeListener = rtk::ListenerMiddleware<FRuntimeState>;
using FRuntimeListenerEffect = FRuntimeListener::EffectCallback;

struct FListenerSubscription {
  FString ActionType;
  FRuntimeListenerEffect Effect;
};

// Reactive effect: a runtime bootstrap workflow failed. createAsyncThunk now
// dispatches the rejected lifecycle action instead of swallowing the error, so
// this listener is where the failure becomes observable.
/** User Story: As a features systems consumer, I need to invoke observe runtime workflow rejected through a stable signature so the features systems workflow remains explicit and composable. @fn void ObserveRuntimeWorkflowRejected( const rtk::AnyAction &Action, const rtk::MiddlewareApi<FRuntimeState> &) */
void ObserveRuntimeWorkflowRejected(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Warning,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVF1F7C4FF9496), *Action.Type);
}

// Reactive effect: a runtime bootstrap workflow completed. Kept as a lifecycle
// trace so runtime readiness is observable without a view owning it.
/** User Story: As a features systems consumer, I need to invoke observe runtime workflow fulfilled through a stable signature so the features systems workflow remains explicit and composable. @fn void ObserveRuntimeWorkflowFulfilled( const rtk::AnyAction &Action, const rtk::MiddlewareApi<FRuntimeState> &) */
void ObserveRuntimeWorkflowFulfilled(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Verbose,
         TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGVFE02DD25E6F8), *Action.Type);
}

/**
 * @fn TArray<FListenerSubscription> RuntimeSubscriptions()
 * @brief Declares runtime action subscriptions and their effect boundaries.
 * @return Listener subscriptions in deterministic registration order.
 *
 * User Story: As a runtime maintainer, I need reactive effects declared in one
 * catalog so adding a workflow does not deepen a hand-wired listener chain.
 */
TArray<FListenerSubscription> RuntimeSubscriptions() {
  const FString PlayerSpawnPrefix =
      RuntimeThunks::RequestPlayerSpawnTypePrefix();
  const FString LevelViewPrefix =
      RuntimeThunks::RequestLevelViewPayloadTypePrefix();
  return {
      {PlayerSpawnPrefix +
           TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV626EB0E3DA73),
       &ObserveRuntimeWorkflowRejected},
      {LevelViewPrefix +
           TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV626EB0E3DA73),
       &ObserveRuntimeWorkflowRejected},
      {LevelViewPrefix +
           TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV0DFF16731112),
       &ObserveRuntimeWorkflowFulfilled}};
}

/**
 * @fn FRuntimeListener AppendSubscription(const FRuntimeListener &Listener, const FListenerSubscription &Subscription)
 * @brief Adds one declared subscription to an immutable listener accumulator.
 * @param Listener Current listener middleware value.
 * @param Subscription Action/effect declaration to append.
 * @return Listener middleware containing the appended subscription.
 *
 * User Story: As a runtime maintainer, I need one composer for every listener
 * declaration so reactive behavior remains flat and reviewable.
 */
FRuntimeListener
AppendSubscription(const FRuntimeListener &Listener,
                   const FListenerSubscription &Subscription) {
  return rtk::addListener<FRuntimeState>(
      Listener, Subscription.ActionType, Subscription.Effect);
}

} // namespace

/** User Story: As a features systems consumer, I need to invoke create runtime listener middleware through a stable signature so the features systems workflow remains explicit and composable. @fn rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware() */
rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware() {
  return rtk::buildListenerMiddleware<FRuntimeState>(
      func::fold_array<FListenerSubscription, FRuntimeListener>(
          RuntimeSubscriptions(), rtk::createListenerMiddleware<FRuntimeState>(),
          &AppendSubscription));
}

} // namespace RuntimeListeners
} // namespace Level
} // namespace Game
} // namespace ForbocAI
