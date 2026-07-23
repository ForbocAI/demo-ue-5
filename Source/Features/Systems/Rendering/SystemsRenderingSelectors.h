#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"
#include "Features/Systems/Rendering/RenderingTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

/** User Story: As a features systems rendering consumer, I need to invoke select runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FLevelRetroRenderProfile & SelectRuntimeProfile(const FRenderingState &State) */
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select texture catalog through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> & SelectTextureCatalog(const FRenderingState &State) */
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select distance lod stages through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const TArray<FLevelDistanceLodStage> & SelectDistanceLodStages(const FRenderingState &State) */
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select rendering settings through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings & SelectRenderingSettings(const FRenderingState &State) */
const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select rendering asset paths through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FRenderingAssetPaths &SelectRenderingAssetPaths( const FRenderingState &State) */
const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select townsperson presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel & SelectTownspersonPresentation(const FRenderingState &State) */
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select horse presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FHorsePresentationViewModel & SelectHorsePresentation(const FRenderingState &State) */
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State);
/** User Story: As a features systems rendering consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel & SelectRuntimeStatsPresentation(const FRenderingState &State) */
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRenderingState &State);

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
