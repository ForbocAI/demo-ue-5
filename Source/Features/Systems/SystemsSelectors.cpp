#include "Features/Systems/SystemsSelectors.h"

#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Systems/Bots/AI/AISelectors.h"
#include "Features/Systems/Bots/SystemsBotsSelectors.h"
#include "Features/Systems/Bots/Goals/GoalsSelectors.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Position/PositionSelectors.h"
#include "Features/Systems/Bots/Stats/BotsStatsSelectors.h"
#include "Features/Systems/Bots/Horses/HorsesSelectors.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSelectors.h"
#include "Features/Systems/Level/SystemsLevelSelectors.h"
#include "Features/Systems/Nature/SystemsNatureSelectors.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSelectors.h"
#include "Features/Systems/Interaction/InteractionSelectors.h"
#include "Features/Systems/UI/UISelectors.h"

#include "Store.h"

#include "Features/Systems/State/StateSelectors.h"
#include "Features/Systems/Presentation/PresentationSelectors.h"
#include "Features/Systems/Route/RouteSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

const FRuntimeState &SelectState() { return Store::GetStore().getState(); }

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
