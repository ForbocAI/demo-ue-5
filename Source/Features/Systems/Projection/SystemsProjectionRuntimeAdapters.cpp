#include "Features/Systems/Projection/SystemsProjectionRuntimeAdapters.h"

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionInteractionAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionSpawnAdapters.h"
#include "Features/Systems/Projection/SystemsProjectionTerrainAdapters.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

namespace ForbocAI {
namespace Demo {
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
  return ecs::projectRows<Row>(
      World, SelectRows(State),
      [Project](const ecs::FWorld &Acc, const Row &SelectedRow) {
        return projectPayload<Row, Payload>(Project, Acc, SelectedRow);
      });
}

template <typename... Values> struct TProjectionCatalog {
  std::tuple<Values...> ValuesTuple;
};

template <typename... Values>
TProjectionCatalog<Values...> projectionCatalog(Values... ValuesList) {
  return {std::make_tuple(ValuesList...)};
}

template <typename Head, typename... Tail>
std::tuple<Tail...> tailTuple(const std::tuple<Head, Tail...> &Values);

ecs::FWorld runProjectionCatalog(const FRuntimeState &State,
                                 const ecs::FWorld &World,
                                 std::tuple<> Selectors,
                                 std::tuple<> Projectors) {
  return World;
}

template <typename Select, typename Project, typename... SelectRest,
          typename... ProjectRest>
ecs::FWorld runProjectionCatalog(
    const FRuntimeState &State, const ecs::FWorld &World,
    std::tuple<Select, SelectRest...> Selectors,
    std::tuple<Project, ProjectRest...> Projectors) {
  const Select SelectValue = std::get<0>(Selectors);
  const Project ProjectValue = std::get<0>(Projectors);
  return runProjectionCatalog(
      State, applyProjection(State, World, SelectValue, ProjectValue),
      tailTuple(Selectors), tailTuple(Projectors));
}

template <typename Tuple, size_t... Indices>
auto tailTupleImpl(const Tuple &Values, func::seq<Indices...>)
    -> decltype(std::make_tuple(std::get<Indices + 1>(Values)...)) {
  return std::make_tuple(std::get<Indices + 1>(Values)...);
}

template <typename Head, typename... Tail>
std::tuple<Tail...> tailTuple(const std::tuple<Head, Tail...> &Values) {
  return tailTupleImpl(Values, func::gen_seq<sizeof...(Tail)>());
}

ecs::FWorld applyResourceProjection(const ecs::FWorld &World,
                                    const FResourceProjectionBinding &Binding) {
  return ComponentsAdapters::WithResource(
      {World, Binding.Name, Binding.Value});
}

ecs::FWorld RuntimeProjectionWorld(const FRuntimeState &State) {
  using namespace EntitiesAdapters;
  using namespace RuntimeSelectors;
  using namespace SystemsProjectionBotAdapters;
  using namespace SystemsProjectionInteractionAdapters;
  using namespace SystemsProjectionSpawnAdapters;
  using namespace SystemsProjectionTerrainAdapters;

  const ecs::FWorld Projected = runProjectionCatalog(
      State, ecs::createWorld(),
      projectionCatalog(SelectPlayerState, SelectTerrainState,
                        SelectSpawnState, SelectInteractionState,
                        SelectLandmarks, SelectNatureFeatures,
                        SelectTownspeople, SelectHorses, SelectBots,
                        SelectBotStats, SelectBotPositions, SelectBotAI,
                        SelectBotGoals)
          .ValuesTuple,
      projectionCatalog(ProjectPlayer, ProjectTerrain, ProjectSpawn,
                        ProjectInteraction, ProjectLandmark,
                        ProjectNatureFeature, ProjectTownsperson,
                        ProjectHorse, ProjectBot, ProjectBotStats,
                        ProjectBotPosition, ProjectBotAI, ProjectBotGoal)
          .ValuesTuple);
  return applyResourceProjection(
      Projected, resourceProjection(TEXT("Systems/Runtime/Projected"),
                                    ecs::boolValue(true)));
}

} // namespace

ecs::FWorld
ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload) {
  return RuntimeProjectionWorld(Payload.State);
}

} // namespace SystemsProjectionRuntimeAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
