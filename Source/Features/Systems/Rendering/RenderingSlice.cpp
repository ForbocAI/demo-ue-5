#include "Features/Systems/Rendering/SystemsRenderingSlice.h"

#include "Core/ue_fp.hpp"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) {
  return {Settings.LevelCubeMeshPath, Settings.BlockoutMaterialPath};
}

FRenderingPayload ReduceRenderingPayload(
    const FPayloadRequest &Request) {
  return {Request.Id, Request.RuntimeProfile, Request.TextureCatalog,
          Request.DistanceLodStages, Request.Settings};
}

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

FRenderingState CreateInitialState() {
  return (func::pipe(FRenderingState{}) |
          [](FRenderingState State) -> FRenderingState {
            State.Profile.ActionId = func::nothing<FString>();
            State.Profile.bReady = false;
            return State;
          })
      .val;
}

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

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  RenderingThunks::ApplyRuntimeProfile(Request);
}

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeBudgetScreenshotIntervalSeconds(
      Settings);
}

double SelectRuntimeBudgetClockSeconds() {
  return RenderingAdapters::SelectRuntimeBudgetClockSeconds();
}

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) {
  return RenderingSelectors::ShouldRunRuntimeBudgetWallInterval(Params);
}

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingSelectors::ShouldRunRuntimeBudgetScreenshot(Params, Settings);
}

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index) {
  RenderingAdapters::RequestRuntimeBudgetScreenshot(Settings, Index);
}

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeMemoryStats(Settings);
}

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeFrameTimingStats(Settings);
}

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimePolyCountStats(World, Settings);
}

FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return RenderingAdapters::SelectRuntimeStats(
      World, DeltaSeconds, WallDeltaSeconds, PolyCount,
      PolyCountMilliseconds, Settings);
}

UMaterialInterface *LoadBlockoutMaterial(const FString &Path) {
  return RenderingThunks::LoadBlockoutMaterial(Path);
}

void ApplyTexture(const FLevelRetroTextureApply &Request) {
  RenderingThunks::ApplyTexture(Request);
}

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
