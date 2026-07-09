#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/ComponentsDataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

// Forward includes for split reducers
#include "Features/Systems/Rendering/Texture/TextureSlice.h"
#include "Features/Systems/Rendering/Distance/Lod/LodSlice.h"
#include "Features/Systems/Rendering/Profile/ProfileSlice.h"
#include "Features/Systems/Rendering/Presentation/RenderingPresentationSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingReducers {

FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings);

FRenderingPayload ReduceRenderingPayload(
    const FPayloadRequest &Request);

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

#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"
#include "Features/Systems/Rendering/SystemsRenderingActions.h"
#include "Features/Systems/Rendering/RenderingTypes.h"
#include "Features/Systems/Rendering/Payload/Application/PayloadApplicationAdapters.h"
#include "Features/Systems/Rendering/Texture/Application/TextureApplicationAdapters.h"

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
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
double SelectRuntimeBudgetClockSeconds();
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index);
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
