#include "Features/Systems/Runtime/RuntimeAdapters.h"

#include "Features/Components/Level/LevelTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Entities/Characters/Bots/BotsAdapters.h"
#include "Features/Entities/Environments/Landmarks/LandmarksAdapters.h"
#include "Features/Entities/Environments/Nature/NatureSeedAdapters.h"
#include "Features/Systems/Bots/Horses/HorseSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspersonSlice.h"
#include "Features/Systems/Landmarks/LandmarkSlice.h"
#include "Features/Systems/Nature/NatureSlice.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/Spawn/SpawnSlice.h"
#include "Features/Systems/Terrain/TerrainSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace RuntimeAdapters {

struct FRuntimeLevelSession::FStore {
  rtk::EnhancedStore<FRuntimeState> Value = RuntimeStore::ConfigureStore();
};

FRuntimeLevelSession::FRuntimeLevelSession()
    : Store(MakeShared<FStore>()) {}

void FRuntimeLevelSession::Seed(const FLevelTerrainData &TerrainData,
                                const FLevelOrthoData &OrthoData) {
  Store->Value.dispatch(TerrainActions::TerrainLoaded()(
      TerrainFactories::LoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));
  Store->Value.dispatch(LandmarkActions::LandmarksSeeded()(
      LandmarksAdapters::Build1899LandmarkSeed(TerrainData)));
  Store->Value.dispatch(SpawnActions::PlayerSpawnAnchored()(
      SpawnFactories::SpawnPointPayload(
          {LevelLayoutSlice::ToWorld(TerrainData,
                                     LevelLayoutSlice::PlayerSpawnPoint()),
           LevelLayoutSlice::PlayerSpawnRotation(),
           LevelLayoutSlice::PlayerSpawnAnchorLabel()})));
  Store->Value.dispatch(TownspersonActions::TownspeopleSeeded()(
      BotsAdapters::Build1899TownspersonSeed()));
  Store->Value.dispatch(
      HorseActions::HorsesSeeded()(BotsAdapters::Build1899HorseRouteSeed()));
  Store->Value.dispatch(NatureActions::NatureSeeded()(
      NatureAdapters::BuildClearCreekNatureSeed()));
}

TArray<FLandmark> FRuntimeLevelSession::SelectLandmarks() const {
  return RuntimeSelectors::SelectLandmarks(Store->Value.getState());
}

TArray<FNatureFeatureSeed>
FRuntimeLevelSession::SelectNatureFeatures() const {
  return RuntimeSelectors::SelectNatureFeatures(Store->Value.getState());
}

TArray<FTownspersonSeed> FRuntimeLevelSession::SelectTownspeople() const {
  return RuntimeSelectors::SelectTownspeople(Store->Value.getState());
}

TArray<FHorseRouteSeed> FRuntimeLevelSession::SelectHorses() const {
  return RuntimeSelectors::SelectHorses(Store->Value.getState());
}

} // namespace RuntimeAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
