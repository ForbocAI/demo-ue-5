#include "Features/Systems/Listeners.h"

#include "Features/Systems/Thunks.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeListener, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeListeners {
namespace {

// Reactive effect: a runtime bootstrap workflow failed. createAsyncThunk now
// dispatches the rejected lifecycle action instead of swallowing the error, so
// this listener is where the failure becomes observable.
void ObserveRuntimeWorkflowRejected(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Warning,
         TEXT("runtime workflow rejected: %s"), *Action.Type);
}

// Reactive effect: a runtime bootstrap workflow completed. Kept as a lifecycle
// trace so runtime readiness is observable without a view owning it.
void ObserveRuntimeWorkflowFulfilled(
    const rtk::AnyAction &Action,
    const rtk::MiddlewareApi<FRuntimeState> &) {
  UE_LOG(LogForbocRuntimeListener, Verbose,
         TEXT("runtime workflow fulfilled: %s"), *Action.Type);
}

} // namespace

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
