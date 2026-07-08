#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Rendering/Distance/Lod/Types.h"
#include "Features/Components/Rendering/Texture/Types.h"
#include "Features/Components/Rendering/Profile/Types.h"
#include "Features/Systems/Rendering/Types.h"

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
const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRenderingState &State);
const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State);
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State);
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State);
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRenderingState &State);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
