#include "Store.h"

#include "Core/redux_logger.hpp"
#include "Features/Systems/Runtime/RuntimeFactories.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"

#include <vector>

DEFINE_LOG_CATEGORY_STATIC(LogForbocDemoRedux, Log, All);

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace Store {
namespace detail {

/**
 * @brief Builds the UE console sink used by redux-logger middleware.
 * @signature std::function<void(const FString &)> CreateReduxLoggerSink()
 * @return Logger sink that writes each redux-logger line to the UE log system.
 *
 * User Story: As a developer running the demo or automation tests, I need RTK
 * action flow to be visible in the UE console and captured test logs.
 */
std::function<void(const FString &)> CreateReduxLoggerSink() {
  return [](const FString &Message) -> void {
    UE_LOG(LogForbocDemoRedux, Display, TEXT("%s"), *Message);
  };
}

/**
 * @brief Creates redux-logger options for the runtime root store.
 * @signature rtk::logger::ReduxLoggerOptions<FRuntimeState> CreateReduxLoggerOptions()
 * @return Logger options with an explicit UE output sink and deterministic title formatting.
 *
 * User Story: As a maintainer inspecting unidirectional data flow, I need
 * dispatched RTK actions to log consistently without moving logic into views.
 */
rtk::logger::ReduxLoggerOptions<FRuntimeState> CreateReduxLoggerOptions() {
  rtk::logger::ReduxLoggerOptions<FRuntimeState> Options;
  Options.Logger = CreateReduxLoggerSink();
  Options.bTimestamp = false;
  Options.bDuration = false;
  return Options;
}

/**
 * @brief Builds RTK middleware for the demo runtime store.
 * @signature std::vector<rtk::Middleware<FRuntimeState>> CreateRuntimeMiddleware()
 * @return Middleware chain containing the SDK redux-logger middleware.
 *
 * User Story: As runtime store configuration, I need instrumentation to wrap
 * dispatch through RTK middleware while reducers remain the only state writers.
 */
std::vector<rtk::Middleware<FRuntimeState>> CreateRuntimeMiddleware() {
  return std::vector<rtk::Middleware<FRuntimeState>>{
      rtk::logger::createLogger<FRuntimeState>(CreateReduxLoggerOptions())};
}

} // namespace detail

/**
 * @brief Creates the configured root store from RuntimeSlice.
 * @signature rtk::EnhancedStore<FRuntimeState> ConfigureStore()
 * @return Enhanced store with RuntimeSlice reducer, initial state, and RTK middleware.
 *
 * User Story: As runtime bootstrapping, I need the single authoritative store
 * to compose reducer-owned state transitions and middleware-owned diagnostics.
 */
rtk::EnhancedStore<FRuntimeState> ConfigureStore() {
  return rtk::configureStore<FRuntimeState>(
      RuntimeSlice::GetSlice().Reducer, RuntimeFactories::CreateInitialState(),
      detail::CreateRuntimeMiddleware());
}

/**
 * @brief Provides the single store instance used by runtime systems and views.
 * @signature rtk::EnhancedStore<FRuntimeState> &GetStore()
 * @return Process-local singleton enhanced store.
 *
 * User Story: As UE boundary code, I need one shared store so events dispatch
 * into the same RTK action, reducer, selector, and render loop.
 */
rtk::EnhancedStore<FRuntimeState> &GetStore() {
  static rtk::EnhancedStore<FRuntimeState> SingletonStore = ConfigureStore();
  return SingletonStore;
}

} // namespace Store
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
