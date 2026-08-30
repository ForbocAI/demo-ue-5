#include "Features/Systems/Rendering/Stats/Sampling/SamplingAdapters.h"

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingStatsAdapters {
namespace {

/** User Story: As a stats observer, I need poly count sources registered by sample mode so cache retention and engine measurement share one dispatch boundary. @fn func::Dispatcher<ERuntimeStatsSampleMode, FRuntimePolyCountStats> RuntimePolyCountCatalog(const FRuntimeStatsObservationRequest &Request) */
func::Dispatcher<ERuntimeStatsSampleMode, FRuntimePolyCountStats>
RuntimePolyCountCatalog(const FRuntimeStatsObservationRequest &Request) {
  return func::createDispatcher<ERuntimeStatsSampleMode,
                                FRuntimePolyCountStats>(
      {{ERuntimeStatsSampleMode::Retain,
        [&Request]() {
          return FRuntimePolyCountStats{
              Request.State->PolyCache.CachedPolyCount,
              Request.State->PolyCache.CachedPolyCountMilliseconds};
        }},
       {ERuntimeStatsSampleMode::Refresh,
        [&Request]() {
          return RenderingAdapters::SelectRuntimePolyCountStats(
              Request.World, *Request.Settings);
        }}});
}

/** User Story: As a stats observer, I need view-model sources registered by sample mode so deferred ticks avoid engine diagnostics without local branching. @fn func::Dispatcher<ERuntimeStatsSampleMode, func::Maybe<FRuntimeStatsViewModel>> RuntimeStatsCatalog(const FRuntimeStatsObservationRequest &Request, const FRuntimePolyCountStats &PolyCount) */
func::Dispatcher<ERuntimeStatsSampleMode,
                 func::Maybe<FRuntimeStatsViewModel>>
RuntimeStatsCatalog(const FRuntimeStatsObservationRequest &Request,
                    const FRuntimePolyCountStats &PolyCount) {
  return func::createDispatcher<
      ERuntimeStatsSampleMode, func::Maybe<FRuntimeStatsViewModel>>(
      {{ERuntimeStatsSampleMode::Retain,
        []() { return func::nothing<FRuntimeStatsViewModel>(); }},
       {ERuntimeStatsSampleMode::Refresh,
        [&Request, &PolyCount]() {
          return func::just(RenderingAdapters::SelectRuntimeStats(
              Request.World, Request.DeltaSeconds,
              Request.Refresh->WallDeltaSeconds, PolyCount.PolyCount,
              PolyCount.MeasurementMilliseconds, *Request.Settings));
        }}});
}

} // namespace

/**
 * User Story: As a rendering stats thunk, I need live or retained engine observations selected through registered sample-mode handlers so timing predicates stay in the pure plan.
 * @fn FRuntimeStatsObservation ObserveRuntimeStats( const FRuntimeStatsObservationRequest &Request)
 */
FRuntimeStatsObservation ObserveRuntimeStats(
    const FRuntimeStatsObservationRequest &Request) {
  const func::Maybe<FRuntimePolyCountStats> PolyCount = func::dispatch(
      RuntimePolyCountCatalog(Request), Request.Refresh->PolyCountMode);
  check(PolyCount.hasValue);
  const func::Maybe<func::Maybe<FRuntimeStatsViewModel>> Stats =
      func::dispatch(RuntimeStatsCatalog(Request, PolyCount.value),
                     Request.Refresh->StatsMode);
  check(Stats.hasValue);
  return {PolyCount.value, Stats.value};
}

} // namespace RenderingStatsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
