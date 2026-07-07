#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

// Forward includes for split reducers
#include "Features/Systems/Rendering/RenderingTextureReducers.h"
#include "Features/Systems/Rendering/RenderingDistanceLodReducers.h"
#include "Features/Systems/Rendering/RenderingProfileReducers.h"
#include "Features/Systems/Rendering/RenderingPresentationReducers.h"

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
