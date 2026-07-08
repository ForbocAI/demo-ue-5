#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/Types.h"
#include "Features/Systems/Rendering/Types.h"

// Forward includes for split reducers
#include "Features/Systems/Rendering/Texture/Slice.h"
#include "Features/Systems/Rendering/Distance/Lod/Slice.h"
#include "Features/Systems/Rendering/Profile/Slice.h"
#include "Features/Systems/Rendering/Presentation/Slice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings);

FRenderingPayload ReduceRenderingPayload(
    const FRenderingPayloadRequest &Request);

FRenderingState
ReduceRenderingProfileObserved(const FRenderingState &State,
                               const rtk::PayloadAction<FRenderingPayload>
                                   &Action);
FRenderingState ReduceRuntimeStatsSampled(
    const FRenderingState &State,
    const rtk::PayloadAction<FRuntimeStatsSamplePayload> &Action);

} // namespace RenderingReducers
} // namespace Level
} // namespace Game
} // namespace ForbocAI

#include "Core/rtk.hpp"

#include "Features/Components/Data/Settings/UI/Types.h"
#include "Features/Systems/Rendering/Actions.h"
#include "Features/Systems/Rendering/Types.h"

class UMaterialInterface;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSlice {

FRenderingState CreateInitialState();
const rtk::Slice<FRenderingState> &GetSlice();
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
double SelectRuntimeBudgetClockSeconds();
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings,
    int32 Index);
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FStatsOverlaySettings &Settings);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
