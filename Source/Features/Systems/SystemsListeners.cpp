#include "Features/Systems/SystemsListeners.h"

#include "Features/Systems/SystemsThunks.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeListener, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeListeners {
namespace {

// Reactive effect: a runtime bootstrap workflow failed. createAsyncThunk now
// dispatches the rejected lifecycle action instead of swallowing the error, so
// this listener is where the failure becomes observable.
/** User Story: As a features systems consumer, I need to invoke observe runtime workflow rejected through a stable signature so the features systems workflow remains explicit and composable. @fn void ObserveRuntimeWorkflowRejected( const rtk::AnyAction &Action, const rtk::MiddlewareApi<FRuntimeState> &) */
void ObserveRuntimeWorkflowRejected(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Warning,
         TEXT("runtime workflow rejected: %s"), *Action.Type);
}

// Reactive effect: a runtime bootstrap workflow completed. Kept as a lifecycle
// trace so runtime readiness is observable without a view owning it.
/** User Story: As a features systems consumer, I need to invoke observe runtime workflow fulfilled through a stable signature so the features systems workflow remains explicit and composable. @fn void ObserveRuntimeWorkflowFulfilled( const rtk::AnyAction &Action, const rtk::MiddlewareApi<FRuntimeState> &) */
void ObserveRuntimeWorkflowFulfilled(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Verbose,
         TEXT("runtime workflow fulfilled: %s"), *Action.Type);
}

} // namespace

/** User Story: As a features systems consumer, I need to invoke create runtime listener middleware through a stable signature so the features systems workflow remains explicit and composable. @fn rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware() */
rtk::Middleware<FRuntimeState> CreateRuntimeListenerMiddleware() {
  const FString PlayerSpawnPrefix =
      RuntimeThunks::RequestPlayerSpawnTypePrefix();
  const FString LevelViewPrefix =
      RuntimeThunks::RequestLevelViewPayloadTypePrefix();
  return rtk::buildListenerMiddleware<FRuntimeState>(rtk::addListener<
      FRuntimeState>(
      rtk::addListener<FRuntimeState>(
          rtk::addListener<FRuntimeState>(
              rtk::createListenerMiddleware<FRuntimeState>(),
              PlayerSpawnPrefix + TEXT("/rejected"),
              &ObserveRuntimeWorkflowRejected),
          LevelViewPrefix + TEXT("/rejected"),
          &ObserveRuntimeWorkflowRejected),
      LevelViewPrefix + TEXT("/fulfilled"),
      &ObserveRuntimeWorkflowFulfilled));
}

} // namespace RuntimeListeners
} // namespace Level
} // namespace Game
} // namespace ForbocAI
