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

void ApplyRuntimeProfile(
    UWorld *World,
    const FLevelRetroRenderProfile &Profile,
    const ForbocAI::Game::Data::FRenderingRuntimeSettings &RuntimeSettings) {
  RenderingThunks::ApplyRuntimeProfile(World, Profile, RuntimeSettings);
}

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::SelectRuntimeBudgetScreenshotIntervalSeconds(
      Settings);
}

double SelectRuntimeBudgetClockSeconds() {
  return RenderingThunks::SelectRuntimeBudgetClockSeconds();
}

bool ShouldRunRuntimeBudgetWallInterval(double CurrentSeconds,
                                        double LastSeconds,
                                        float IntervalSeconds) {
  return RenderingThunks::ShouldRunRuntimeBudgetWallInterval(
      CurrentSeconds, LastSeconds, IntervalSeconds);
}

bool ShouldRunRuntimeBudgetScreenshot(
    double CurrentSeconds, double LastSeconds, float IntervalSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return RenderingThunks::ShouldRunRuntimeBudgetScreenshot(
      CurrentSeconds, LastSeconds, IntervalSeconds, Settings);
}

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  RenderingThunks::RequestRuntimeBudgetScreenshot(Settings, Index);
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
