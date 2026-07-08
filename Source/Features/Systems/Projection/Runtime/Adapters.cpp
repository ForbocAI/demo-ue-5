#include "Features/Systems/Projection/Runtime/Adapters.h"

#include "Features/Components/Adapters.h"
#include "Features/Entities/Adapters.h"
#include "Features/Systems/Projection/Bot/Adapters.h"
#include "Features/Systems/Projection/Interaction/Adapters.h"
#include "Features/Systems/Projection/Spawn/Adapters.h"
#include "Features/Systems/Projection/Terrain/Adapters.h"
#include "Features/Systems/Selectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionRuntimeAdapters {
namespace {

struct FResourceProjectionBinding {
  ecs::ResourceName Name;
  ecs::FComponentValue Value;
};

FResourceProjectionBinding resourceProjection(const ecs::ResourceName &Name,
                                              const ecs::FComponentValue &Value) {
  return {Name, Value};
}

template <typename Value, typename Payload>
ecs::FWorld projectPayload(ecs::FWorld (*Project)(const Payload &),
                           const ecs::FWorld &World,
                           const Value &SelectedValue) {
  return Project(Payload{World, SelectedValue});
}

template <typename Slice, typename Payload>
ecs::FWorld applyProjection(const FRuntimeState &State,
                            const ecs::FWorld &World,
                            const Slice &(*SelectSlice)(const FRuntimeState &),
                            ecs::FWorld (*Project)(const Payload &)) {
  return projectPayload<Slice, Payload>(Project, World, SelectSlice(State));
}

template <typename Row, typename Payload>
ecs::FWorld applyProjection(const FRuntimeState &State,
                            const ecs::FWorld &World,
                            TArray<Row> (*SelectRows)(const FRuntimeState &),
                            ecs::FWorld (*Project)(const Payload &)) {
  return ecs::projectRowsIntoWorld<Row>(
      World, SelectRows(State),
      [Project](const ecs::FWorld &Acc, const Row &SelectedRow) {
        return projectPayload<Row, Payload>(Project, Acc, SelectedRow);
      });
}

struct FApplyRuntimeProjection {
  const FRuntimeState &State;

  template <typename Select, typename Project>
  ecs::FWorld operator()(const ecs::FWorld &World, Select SelectValue,
                         Project ProjectValue) const {
    return applyProjection(State, World, SelectValue, ProjectValue);
  }
};

ecs::FWorld applyResourceProjection(const ecs::FWorld &World,
                                    const FResourceProjectionBinding &Binding) {
  return ComponentsAdapters::ProjectResource(
      {World, Binding.Name, Binding.Value});
}

ecs::FWorld RuntimeProjectionWorld(const FRuntimeState &State) {
  using namespace EntitiesAdapters;
  using namespace RuntimeSelectors;
  using namespace SystemsProjectionBotAdapters;
  using namespace SystemsProjectionInteractionAdapters;
  using namespace SystemsProjectionSpawnAdapters;
  using namespace SystemsProjectionTerrainAdapters;

	  const ecs::FWorld Projected = ecs::projectWorldCatalogPairs(
	      ecs::createWorld(State.Ecs.World.Domains),
      func::catalog(SelectPlayerState, SelectTerrainState, SelectSpawnState,
                    SelectInteractionState, SelectLandmarks,
                    SelectNatureFeatures, SelectTownspeople, SelectHorses,
                    SelectBots, SelectBotStats, SelectBotPositions,
                    SelectBotAI, SelectBotGoals),
      func::catalog(ProjectPlayer, ProjectTerrain, ProjectSpawn,
                    ProjectInteraction, ProjectLandmark, ProjectNatureFeature,
                    ProjectTownsperson, ProjectHorse, ProjectBot,
                    ProjectBotStats, ProjectBotPosition, ProjectBotAI,
                    ProjectBotGoal),
      FApplyRuntimeProjection{State});
  return applyResourceProjection(
      Projected, resourceProjection(TEXT("Systems/Runtime/Projected"),
                                    ecs::createBoolComponentValue(true)));
}

} // namespace

ecs::FWorld
ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) {
  return RuntimeProjectionWorld(Payload.State);
}

} // namespace SystemsProjectionRuntimeAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
