#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Rendering/RenderingTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRenderingState &State);
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRenderingState &State);
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRenderingState &State);
const ForbocAI::Game::Data::FRenderingRuntimeSettings &
SelectRenderingRuntimeSettings(const FRenderingState &State);
const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State);
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State);
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
