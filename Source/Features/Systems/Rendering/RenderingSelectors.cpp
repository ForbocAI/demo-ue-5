#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

/** User Story: As a features systems rendering consumer, I need to invoke select runtime profile through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FLevelRetroRenderProfile & SelectRuntimeProfile(const FRenderingState &State) */
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRenderingState &State) {
  return State.Profile.RuntimeProfile;
}

/** User Story: As a features systems rendering consumer, I need to invoke select texture catalog through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> & SelectTextureCatalog(const FRenderingState &State) */
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRenderingState &State) {
  return State.Catalog.TextureCatalog;
}

/** User Story: As a features systems rendering consumer, I need to invoke select distance lod stages through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const TArray<FLevelDistanceLodStage> & SelectDistanceLodStages(const FRenderingState &State) */
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRenderingState &State) {
  return State.Catalog.DistanceLodStages;
}

/** User Story: As a features systems rendering consumer, I need to invoke select rendering settings through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings & SelectRenderingSettings(const FRenderingState &State) */
const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRenderingState &State) {
  return State.Profile.Settings;
}

/** User Story: As a features systems rendering consumer, I need to invoke select rendering asset paths through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FRenderingAssetPaths &SelectRenderingAssetPaths( const FRenderingState &State) */
const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State) {
  return State.Profile.AssetPaths;
}

/** User Story: As a features systems rendering consumer, I need to invoke select townsperson presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FTownspersonPresentationViewModel & SelectTownspersonPresentation(const FRenderingState &State) */
const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State) {
  return State.Presentation.TownspersonPresentation;
}

/** User Story: As a features systems rendering consumer, I need to invoke select horse presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FHorsePresentationViewModel & SelectHorsePresentation(const FRenderingState &State) */
const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State) {
  return State.Presentation.HorsePresentation;
}

/** User Story: As a features systems rendering consumer, I need to invoke select runtime stats presentation through a stable signature so the features systems rendering workflow remains explicit and composable. @fn const FRuntimeStatsPresentationModel & SelectRuntimeStatsPresentation(const FRenderingState &State) */
const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRenderingState &State) {
  return State.Presentation.StatsPresentation;
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
