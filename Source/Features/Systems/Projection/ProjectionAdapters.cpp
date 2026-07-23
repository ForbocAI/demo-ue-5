#include "Features/Systems/Projection/SystemsProjectionAdapters.h"

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"
#include "Features/Systems/Projection/Interaction/InteractionAdapters.h"
#include "Features/Systems/Projection/Spawn/SpawnAdapters.h"
#include "Features/Systems/Projection/Terrain/ProjectionTerrainAdapters.h"
#include "Features/Systems/SystemsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace SystemsProjectionAdapters {
namespace {

struct FResourceProjectionBinding {
  ecs::ResourceName Name;
  ecs::FComponentValue Value;
};

/** User Story: As a features systems projection consumer, I need to invoke resource projection through a stable signature so the features systems projection workflow remains explicit and composable. @fn FResourceProjectionBinding resourceProjection(const ecs::ResourceName &Name, const ecs::FComponentValue &Value) */
FResourceProjectionBinding resourceProjection(const ecs::ResourceName &Name,
                                              const ecs::FComponentValue &Value) {
  return {Name, Value};
}

/** User Story: As a features systems projection consumer, I need to invoke project payload through a stable signature so the features systems projection workflow remains explicit and composable. @fn template <typename Value, typename Payload> ecs::FWorld projectPayload(ecs::FWorld (*Project)(const Payload &), const ecs::FWorld &World, const Value &SelectedValue) */
template <typename Value, typename Payload>
ecs::FWorld projectPayload(ecs::FWorld (*Project)(const Payload &),
                           const ecs::FWorld &World,
                           const Value &SelectedValue) {
  return Project(Payload{World, SelectedValue});
}

/** User Story: As a features systems projection consumer, I need to invoke apply projection through a stable signature so the features systems projection workflow remains explicit and composable. @fn template <typename Slice, typename Payload> ecs::FWorld applyProjection(const FRuntimeState &State, const ecs::FWorld &World, const Slice &(*SelectSlice)(const FRuntimeState &), ecs::FWorld (*Project)(const Payload &)) */
template <typename Slice, typename Payload>
ecs::FWorld applyProjection(const FRuntimeState &State,
                            const ecs::FWorld &World,
                            const Slice &(*SelectSlice)(const FRuntimeState &),
                            ecs::FWorld (*Project)(const Payload &)) {
  return projectPayload<Slice, Payload>(Project, World, SelectSlice(State));
}

/** User Story: As a features systems projection consumer, I need to invoke apply projection through a stable signature so the features systems projection workflow remains explicit and composable. @fn template <typename Row, typename Payload> ecs::FWorld applyProjection(const FRuntimeState &State, const ecs::FWorld &World, TArray<Row> (*SelectRows)(const FRuntimeState &), ecs::FWorld (*Project)(const Payload &)) */
template <typename Row, typename Payload>
ecs::FWorld applyProjection(const FRuntimeState &State,
                            const ecs::FWorld &World,
                            TArray<Row> (*SelectRows)(const FRuntimeState &),
                            ecs::FWorld (*Project)(const Payload &)) {
  return ecs::projectRowsIntoWorld<Row>(
      SelectRows(State),
      [Project](const ecs::FWorld &Acc, const Row &SelectedRow) {
        return projectPayload<Row, Payload>(Project, Acc, SelectedRow);
      })(World);
}

struct FApplyProjection {
  const FRuntimeState &State;

  /** User Story: As a features systems projection consumer, I need to invoke the callable value through a stable signature so the features systems projection workflow remains explicit and composable. @fn template <typename Select, typename Project> ecs::FWorld operator()(const ecs::FWorld &World, Select SelectValue, Project ProjectValue) const */
  template <typename Select, typename Project>
  ecs::FWorld operator()(const ecs::FWorld &World, Select SelectValue,
                         Project ProjectValue) const {
    return applyProjection(State, World, SelectValue, ProjectValue);
  }
};

/** User Story: As a features systems projection consumer, I need to invoke apply resource projection through a stable signature so the features systems projection workflow remains explicit and composable. @fn ecs::FWorld applyResourceProjection(const ecs::FWorld &World, const FResourceProjectionBinding &Binding) */
ecs::FWorld applyResourceProjection(const ecs::FWorld &World,
                                    const FResourceProjectionBinding &Binding) {
  return ComponentsAdapters::ProjectResource(
      {World, Binding.Name, Binding.Value});
}

/** User Story: As a features systems projection consumer, I need to invoke projection world through a stable signature so the features systems projection workflow remains explicit and composable. @fn ecs::FWorld ProjectionWorld(const FRuntimeState &State) */
ecs::FWorld ProjectionWorld(const FRuntimeState &State) {
  using namespace EntitiesAdapters;
  using namespace RuntimeSelectors;
  using namespace SystemsProjectionBotAdapters;
  using namespace SystemsProjectionInteractionAdapters;
  using namespace SystemsProjectionSpawnAdapters;
  using namespace SystemsProjectionTerrainAdapters;

  const ecs::FWorld Projected = func::zip_catalog_fold(
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
      ecs::createWorld(State.Ecs.World.Domains),
      FApplyProjection{State});
  return applyResourceProjection(
      Projected, resourceProjection(TEXT("Systems/Projection/Projected"),
                                    ecs::createBoolComponentValue(true)));
}

} // namespace

/** User Story: As a features systems projection consumer, I need to invoke project runtime world through a stable signature so the features systems projection workflow remains explicit and composable. @fn ecs::FWorld ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) */
ecs::FWorld
ProjectRuntimeWorld(const FProjectRuntimePayload &Payload) {
  return ProjectionWorld(Payload.State);
}

} // namespace SystemsProjectionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
