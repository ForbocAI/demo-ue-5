#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingSelectors {

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRenderingState &State) {
  return State.RuntimeProfile;
}

const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRenderingState &State) {
  return State.TextureCatalog;
}

const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRenderingState &State) {
  return State.DistanceLodStages;
}

const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRenderingState &State) {
  return State.Settings;
}

const FRenderingAssetPaths &SelectRenderingAssetPaths(
    const FRenderingState &State) {
  return State.AssetPaths;
}

const FTownspersonPresentationViewModel &
SelectTownspersonPresentation(const FRenderingState &State) {
  return State.TownspersonPresentation;
}

const FHorsePresentationViewModel &
SelectHorsePresentation(const FRenderingState &State) {
  return State.HorsePresentation;
}

const FRuntimeStatsPresentationModel &
SelectRuntimeStatsPresentation(const FRenderingState &State) {
  return State.StatsPresentation;
}

} // namespace RenderingSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
