#include "Features/Systems/Rendering/SystemsRenderingSlice.h"

#include "Core/fp.hpp"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering asset paths through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingAssetPaths ReduceRenderingAssetPaths( const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) */
FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering payload through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingPayload ReduceRenderingPayload( const FPayloadRequest &Request) */
FRenderingPayload ReduceRenderingPayload(
    const FPayloadRequest &Request) {
  return {Request.Id, Request.RuntimeProfile, Request.TextureCatalog,
          Request.DistanceLodStages, Request.Settings};
}

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering profile observed through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingState ReduceRenderingProfileObserved( const FRenderingState &State, const rtk::PayloadAction<FRenderingPayload> &Action) */
FRenderingState ReduceRenderingProfileObserved(
    const FRenderingState &State,
    const rtk::PayloadAction<FRenderingPayload> &Action) {
  return (func::pipe(State) |
          [&Action](FRenderingState Next) -> FRenderingState {
            Next.Profile.ActionId = func::just(Action.PayloadValue.Id);
            Next.Profile.RuntimeProfile = Action.PayloadValue.RuntimeProfile;
            Next.Catalog.TextureCatalog = Action.PayloadValue.TextureCatalog;
            Next.Catalog.DistanceLodStages =
                Action.PayloadValue.DistanceLodStages;
            Next.Profile.Settings = Action.PayloadValue.Settings;
            Next.Profile.bReady = true;
            return Next;
          })
      .val;
}

} // namespace RenderingReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"
#include "Features/Systems/Rendering/Diagnostics/DiagnosticsSelectors.h"
#include "Features/Systems/Rendering/RenderingThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSlice {

/** User Story: As a features systems rendering consumer, I need to invoke create initial state through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingState CreateInitialState() */
FRenderingState CreateInitialState() {
  return (func::pipe(FRenderingState{}) |
          [](FRenderingState State) -> FRenderingState {
            State.Profile.ActionId = func::nothing<FString>();
            State.Profile.bReady = false;
            return State;
          })
      .val;
}

/** User Story: As a features systems rendering consumer, I need to invoke get slice through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::Slice<FRenderingState> &GetSlice() */
const rtk::Slice<FRenderingState> &GetSlice() {
  static const func::Lazy<rtk::Slice<FRenderingState>> Slice =
      func::lazy([]() -> rtk::Slice<FRenderingState> {
        // RTK guidance: slice names are reducer/action metadata, not JSON-authored runtime data.
        return rtk::createSlice<FRenderingState>(
            TEXT("systems/rendering"), CreateInitialState(),
            [](rtk::ActionReducerMapBuilder<FRenderingState> &Builder) {
              Builder.addCase(
                  RenderingActions::RenderingProfileObserved(),
                  RenderingReducers::ReduceRenderingProfileObserved);
              Builder.addCase(
                  RenderingActions::RuntimeStatsSampled(),
                  RenderingReducers::ReduceRuntimeStatsSampled);
            });
      });
  return func::eval(Slice);
}

/** User Story: As a features systems rendering consumer, I need to invoke apply runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) */
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  RenderingThunks::ApplyRuntimeProfile(Request);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime budget screenshot interval seconds through a stable signature so the features systems rendering workflow remains explicit and composable. @fn float SelectRuntimeBudgetScreenshotIntervalSeconds( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeBudgetScreenshotIntervalSeconds(
      Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime budget clock seconds through a stable signature so the features systems rendering workflow remains explicit and composable. @fn double SelectRuntimeBudgetClockSeconds() */
double SelectRuntimeBudgetClockSeconds() {
  return RenderingAdapters::SelectRuntimeBudgetClockSeconds();
}

/** User Story: As a features systems rendering consumer, I need to invoke should run runtime budget wall interval through a stable signature so the features systems rendering workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) */
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) {
  return RenderingSelectors::ShouldRunRuntimeBudgetWallInterval(Params);
}

/** User Story: As a features systems rendering consumer, I need to invoke should run runtime budget screenshot through a stable signature so the features systems rendering workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetScreenshot( const FBudgetCheckParams &Params, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingSelectors::ShouldRunRuntimeBudgetScreenshot(Params, Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke request runtime budget screenshot through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void RequestRuntimeBudgetScreenshot( const ForbocAI::Game::Data::FOverlaySettings &Settings, int32 Index) */
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index) {
  RenderingAdapters::RequestRuntimeBudgetScreenshot(Settings, Index);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime memory stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeMemoryStats SelectRuntimeMemoryStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeMemoryStats(Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime frame timing stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeFrameTimingStats(Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime poly count stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimePolyCountStats SelectRuntimePolyCountStats( UWorld *World, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimePolyCountStats(World, Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeStatsViewModel SelectRuntimeStats( UWorld *World, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount, double PolyCountMilliseconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeStats(
      World, DeltaSeconds, WallDeltaSeconds, PolyCount,
      PolyCountMilliseconds, Settings);
}

/** User Story: As a features systems rendering consumer, I need to invoke load blockout material through a stable signature so the features systems rendering workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return RenderingThunks::LoadBlockoutMaterial(Path);
}

/** User Story: As a features systems rendering consumer, I need to invoke apply texture through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingThunks::ApplyTexture(Request);
}

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
