#include "Features/Systems/Rendering/RenderingSlice.h"

#include "Features/Systems/Rendering/RenderingReducers.h"
#include "Features/Systems/Rendering/RenderingThunks.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSlice {

FRenderingState CreateInitialState() {
  return (func::pipe(FRenderingState{}) |
          [](FRenderingState State) -> FRenderingState {
            State.LastActionId = func::nothing<FString>();
            State.bReady = false;
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
                  RenderingActions::TownspersonPresentationRequested(),
                  RenderingReducers::ReduceTownspersonPresentationRequested);
              Builder.addCase(
                  RenderingActions::HorsePresentationRequested(),
                  RenderingReducers::ReduceHorsePresentationRequested);
            });
      });
  return func::eval(Slice);
}

void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) {
  RenderingThunks::ApplyRuntimeProfile(Request);
}

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimeBudgetScreenshotIntervalSeconds(
      Settings);
}

double SelectRuntimeBudgetClockSeconds() {
  return RenderingThunks::SelectRuntimeBudgetClockSeconds();
}

bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) {
  return RenderingThunks::ShouldRunRuntimeBudgetWallInterval(Params);
}

bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::ShouldRunRuntimeBudgetScreenshot(Params, Settings);
}

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  RenderingThunks::RequestRuntimeBudgetScreenshot(Settings, Index);
}

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimeMemoryStats(Settings);
}

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimeFrameTimingStats(Settings);
}

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimePolyCountStats(World, Settings);
}

FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimeStats(
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
