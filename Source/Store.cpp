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

/** User Story: As a store consumer, I need to invoke redux log settings through a stable signature so the store workflow remains explicit and composable. @fn FReduxLogSettings ReduxLogSettings() */
FReduxLogSettings ReduxLogSettings() {
  return ForbocAI::Game::Data::SettingsAdapters::LoadSettings()
      .ReduxLog;
}

/** User Story: As a store consumer, I need to invoke is sampled runtime log action through a stable signature so the store workflow remains explicit and composable. @fn bool IsSampledRuntimeLogAction( const rtk::AnyAction &Action, const FReduxLogSettings &Settings) */
bool IsSampledRuntimeLogAction(
    const rtk::AnyAction &Action,
    const FReduxLogSettings &Settings) {
  return func::any_indexed(
      Settings.SampledActionTypes,
      static_cast<size_t>(Settings.SampledActionTypes.Num()),
      [&Action](const FString &Type) { return Action.Type == Type; });
}

/** User Story: As a store consumer, I need to invoke should log sampled runtime action through a stable signature so the store workflow remains explicit and composable. @fn bool ShouldLogSampledRuntimeAction( const rtk::AnyAction &Action, const FReduxLogSettings &Settings) */
bool ShouldLogSampledRuntimeAction(
    const rtk::AnyAction &Action,
    const FReduxLogSettings &Settings) {
  static TMap<FString, int32> CountsByType;
  const int32 NextCount = CountsByType.FindRef(Action.Type) + 1;
  CountsByType.Add(Action.Type, NextCount);
  return NextCount == 1 || NextCount % Settings.SampleInterval == 0;
}

/** User Story: As a store consumer, I need to invoke should log runtime action through a stable signature so the store workflow remains explicit and composable. @fn bool ShouldLogRuntimeAction(const rtk::AnyAction &Action, const FReduxLogSettings &Settings) */
bool ShouldLogRuntimeAction(const rtk::AnyAction &Action,
                            const FReduxLogSettings &Settings) {
  return IsSampledRuntimeLogAction(Action, Settings)
             ? ShouldLogSampledRuntimeAction(Action, Settings)
             : true;
}

/**
 * @fn std::function<void(const FString &)> CreateReduxLoggerSink()
 * @brief Builds the UE console sink used by redux-logger middleware.
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
 * @fn rtk::logger::ReduxLoggerOptions<FRuntimeState> CreateReduxLoggerOptions()
 * @brief Creates redux-logger options for the runtime root store.
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
 * @fn std::vector<rtk::Middleware<FRuntimeState>> CreateRuntimeMiddleware()
 * @brief Builds RTK middleware for the runtime store.
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
 * @fn rtk::EnhancedStore<FRuntimeState> ConfigureStore()
 * @brief Creates the configured root store from RuntimeSlice.
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
 * @fn rtk::EnhancedStore<FRuntimeState> &GetStore()
 * @brief Provides the single store instance used by runtime systems and views.
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
