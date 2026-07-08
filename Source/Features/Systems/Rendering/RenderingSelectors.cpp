#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRenderingState &State) {
  return State.Profile.RuntimeProfile;
}

const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRenderingState &State) {
  return State.Catalog.TextureCatalog;
}

const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRenderingState &State) {
  return State.Catalog.DistanceLodStages;
}

const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRenderingState &State) {
  return State.Profile.Settings;
}

const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State) {
  return State.Profile.AssetPaths;
}

const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State) {
  return State.Presentation.TownspersonPresentation;
}

const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State) {
  return State.Presentation.HorsePresentation;
}

const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRenderingState &State) {
  return State.Presentation.StatsPresentation;
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
