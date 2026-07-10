#include "Store.h"

#include "Core/redux_logger.hpp"
#include "Features/Components/ComponentsSlice.h"
#include "Features/Components/Data/Settings/DataSettingsAdapters.h"
#include "Features/Entities/EntitiesSlice.h"
#include "Features/Systems/Initialization/InitializationAdapters.h"
#include "Features/Systems/SystemsListeners.h"
#include "Features/Systems/State/StateSlice.h"

#include <vector>

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeRedux, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace Store {
namespace detail {

using FReduxLogSettings = ForbocAI::Game::Data::FReduxLogSettings;

FReduxLogSettings ReduxLogSettings() {
  return ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
      .ReduxLog;
}

bool IsSampledRuntimeLogAction(
    const rtk::AnyAction &Action,
    const FReduxLogSettings &Settings) {
  return func::any_indexed(
      Settings.SampledActionTypes,
      static_cast<size_t>(Settings.SampledActionTypes.Num()),
      [&Action](const FString &Type) { return Action.Type == Type; });
}

bool ShouldLogSampledRuntimeAction(
    const rtk::AnyAction &Action,
    const FReduxLogSettings &Settings) {
  static TMap<FString, int32> CountsByType;
  const int32 NextCount = CountsByType.FindRef(Action.Type) + 1;
  CountsByType.Add(Action.Type, NextCount);
  return NextCount == 1 || NextCount % Settings.SampleInterval == 0;
}

bool ShouldLogRuntimeAction(const rtk::AnyAction &Action,
                            const FReduxLogSettings &Settings) {
  return IsSampledRuntimeLogAction(Action, Settings)
             ? ShouldLogSampledRuntimeAction(Action, Settings)
             : true;
}

/**
 * @brief Builds the UE console sink used by redux-logger middleware.
 * @signature std::function<void(const FString &)> CreateReduxLoggerSink()
 * @return Logger sink that writes each redux-logger line to the UE log system.
 *
 * User Story: As a developer running the runtime or automation tests, I need RTK
 * action flow to be visible in the UE console and captured test logs.
 */
std::function<void(const FString &)> CreateReduxLoggerSink() {
  return [](const FString &Message) -> void {
    UE_LOG(LogForbocRuntimeRedux, Display, TEXT("%s"), *Message);
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
  const FReduxLogSettings LogSettings = ReduxLogSettings();
  Options.Logger = CreateReduxLoggerSink();
  Options.Predicate = [LogSettings](
                          const std::function<const FRuntimeState &()> &,
                          const rtk::AnyAction &Action) {
    return ShouldLogRuntimeAction(Action, LogSettings);
  };
  Options.bTimestamp = false;
  Options.bDuration = false;
  return Options;
}

/**
 * @brief Builds RTK middleware for the runtime store.
 * @signature std::vector<rtk::Middleware<FRuntimeState>> CreateRuntimeMiddleware()
 * @return Middleware chain containing the SDK redux-logger middleware.
 *
 * User Story: As runtime store configuration, I need instrumentation to wrap
 * dispatch through RTK middleware while reducers remain the only state writers.
 */
std::vector<rtk::Middleware<FRuntimeState>> CreateRuntimeMiddleware() {
  // Listener middleware is prepended (outermost) so reactive effects observe
  // post-reducer state, ahead of the redux-logger instrumentation.
  return std::vector<rtk::Middleware<FRuntimeState>>{
      RuntimeListeners::CreateRuntimeListenerMiddleware(),
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
} // namespace Game
} // namespace ForbocAI
