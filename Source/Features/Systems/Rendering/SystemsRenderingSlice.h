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

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering asset paths through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingAssetPaths ReduceRenderingAssetPaths( const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings) */
FRenderingAssetPaths ReduceRenderingAssetPaths(
    const ForbocAI::Game::Data::FRenderingAssetPathSettings &Settings);

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering payload through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingPayload ReduceRenderingPayload( const FPayloadRequest &Request) */
FRenderingPayload ReduceRenderingPayload(
    const FPayloadRequest &Request);

/** User Story: As a features systems rendering consumer, I need to invoke reduce rendering profile observed through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingState ReduceRenderingProfileObserved(const FRenderingState &State, const rtk::PayloadAction<FRenderingPayload> &Action) */
FRenderingState
ReduceRenderingProfileObserved(const FRenderingState &State,
                               const rtk::PayloadAction<FRenderingPayload>
                                   &Action);
/** User Story: As a features systems rendering consumer, I need to invoke reduce runtime stats sampled through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingState ReduceRuntimeStatsSampled( const FRenderingState &State, const rtk::PayloadAction<FRuntimeStatsSamplePayload> &Action) */
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

/** User Story: As a features systems rendering consumer, I need to invoke create initial state through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRenderingState CreateInitialState() */
FRenderingState CreateInitialState();
/** User Story: As a features systems rendering consumer, I need to invoke get slice through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const rtk::Slice<FRenderingState> &GetSlice() */
const rtk::Slice<FRenderingState> &GetSlice();
/** User Story: As a features systems rendering consumer, I need to invoke apply runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request) */
void ApplyRuntimeProfile(const FRuntimeProfileApplyRequest &Request);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime budget screenshot interval seconds through a stable signature so the features systems rendering workflow remains explicit and composable. @fn float SelectRuntimeBudgetScreenshotIntervalSeconds( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime budget clock seconds through a stable signature so the features systems rendering workflow remains explicit and composable. @fn double SelectRuntimeBudgetClockSeconds() */
double SelectRuntimeBudgetClockSeconds();
/** User Story: As a features systems rendering consumer, I need to invoke should run runtime budget wall interval through a stable signature so the features systems rendering workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params) */
bool ShouldRunRuntimeBudgetWallInterval(const FBudgetCheckParams &Params);
/** User Story: As a features systems rendering consumer, I need to invoke should run runtime budget screenshot through a stable signature so the features systems rendering workflow remains explicit and composable. @fn bool ShouldRunRuntimeBudgetScreenshot( const FBudgetCheckParams &Params, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
bool ShouldRunRuntimeBudgetScreenshot(
    const FBudgetCheckParams &Params,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke request runtime budget screenshot through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void RequestRuntimeBudgetScreenshot( const ForbocAI::Game::Data::FOverlaySettings &Settings, int32 Index) */
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime memory stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeMemoryStats SelectRuntimeMemoryStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime frame timing stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime poly count stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimePolyCountStats SelectRuntimePolyCountStats( UWorld *World, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime stats through a stable signature so the features systems rendering workflow remains explicit and composable. @fn FRuntimeStatsViewModel SelectRuntimeStats( UWorld *World, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount, double PolyCountMilliseconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *World, float DeltaSeconds, double WallDeltaSeconds,
    int64 PolyCount, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings);
/** User Story: As a features systems rendering consumer, I need to invoke load blockout material through a stable signature so the features systems rendering workflow remains explicit and composable. @fn UMaterialInterface *LoadBlockoutMaterial(const FString &Path) */
UMaterialInterface *LoadBlockoutMaterial(const FString &Path);
/** User Story: As a features systems rendering consumer, I need to invoke apply texture through a stable signature so the features systems rendering workflow remains explicit and composable. @fn void ApplyTexture(const FLevelRetroTextureApply &Request) */
void ApplyTexture(const FLevelRetroTextureApply &Request);

} // namespace RenderingSlice
} // namespace Level
} // namespace Game
} // namespace ForbocAI
