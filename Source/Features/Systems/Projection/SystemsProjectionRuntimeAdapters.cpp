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

struct FProjectRuntimeWorldPayload {
  ecs::FWorld World;
  const FRuntimeState &State;
};

/**
 * @brief Projects selected player state into the runtime ECS world.
 * @signature ecs::FWorld ProjectSelectedPlayer(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, I need player slice state selected from
 * the root state before handing it to the entity projection adapter.
 */
ecs::FWorld ProjectSelectedPlayer(const FProjectRuntimeWorldPayload &Payload) {
  return EntitiesAdapters::ProjectPlayer(
      {Payload.World, RuntimeSelectors::SelectPlayerState(Payload.State)});
}

/**
 * @brief Projects selected terrain state into the runtime ECS world.
 * @signature ecs::FWorld ProjectSelectedTerrain(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, terrain projection should use the
 * systems/projection/terrain adapter rather than ad hoc reducer logic.
 */
ecs::FWorld ProjectSelectedTerrain(const FProjectRuntimeWorldPayload &Payload) {
  return SystemsProjectionTerrainAdapters::ProjectTerrain(
      {Payload.World, RuntimeSelectors::SelectTerrainState(Payload.State)});
}

/**
 * @brief Projects selected spawn state into the runtime ECS world.
 * @signature ecs::FWorld ProjectSelectedSpawn(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, spawn projection should stay inside the
 * systems/projection/spawn adapter.
 */
ecs::FWorld ProjectSelectedSpawn(const FProjectRuntimeWorldPayload &Payload) {
  return SystemsProjectionSpawnAdapters::ProjectSpawn(
      {Payload.World, RuntimeSelectors::SelectSpawnState(Payload.State)});
}

/**
 * @brief Projects selected interaction state into the runtime ECS world.
 * @signature ecs::FWorld ProjectSelectedInteraction(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, interaction projection should read state
 * through selectors and write ECS through adapter payloads.
 */
ecs::FWorld
ProjectSelectedInteraction(const FProjectRuntimeWorldPayload &Payload) {
  return SystemsProjectionInteractionAdapters::ProjectInteraction(
      {Payload.World, RuntimeSelectors::SelectInteractionState(Payload.State)});
}

/**
 * @brief Projects one selected landmark row into ECS.
 * @signature ecs::FWorld ProjectLandmarkRow(const ComponentsAdapters::TEcsRowProjectionPayload<FLandmark> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, each selected landmark
 * row should enter ECS through a unary row payload.
 */
ecs::FWorld ProjectLandmarkRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FLandmark> &Payload) {
  return EntitiesAdapters::ProjectLandmark({Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected nature-feature row into ECS.
 * @signature ecs::FWorld ProjectNatureFeatureRow(const ComponentsAdapters::TEcsRowProjectionPayload<FNatureFeatureSeed> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, each selected nature row
 * should enter ECS through a unary row payload.
 */
ecs::FWorld ProjectNatureFeatureRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FNatureFeatureSeed>
        &Payload) {
  return EntitiesAdapters::ProjectNatureFeature(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected townsperson row into ECS.
 * @signature ecs::FWorld ProjectTownspersonRow(const ComponentsAdapters::TEcsRowProjectionPayload<FTownspersonSeed> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, each selected townsperson
 * row should enter ECS through a unary row payload.
 */
ecs::FWorld ProjectTownspersonRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FTownspersonSeed>
        &Payload) {
  return EntitiesAdapters::ProjectTownsperson(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected horse row into ECS.
 * @signature ecs::FWorld ProjectHorseRow(const ComponentsAdapters::TEcsRowProjectionPayload<FHorseRouteSeed> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, each selected horse row
 * should enter ECS through a unary row payload.
 */
ecs::FWorld ProjectHorseRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FHorseRouteSeed>
        &Payload) {
  return EntitiesAdapters::ProjectHorse({Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected bot row into ECS.
 * @signature ecs::FWorld ProjectBotRow(const ComponentsAdapters::TEcsRowProjectionPayload<FBotEntity> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, each selected bot row
 * should enter ECS through a unary row payload.
 */
ecs::FWorld ProjectBotRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FBotEntity> &Payload) {
  return EntitiesAdapters::ProjectBot({Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected bot-stats row into ECS.
 * @signature ecs::FWorld ProjectBotStatsRow(const ComponentsAdapters::TEcsRowProjectionPayload<FBotStatsComponent> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, selected bot stats rows
 * should enter ECS through the systems/projection/bots/stats adapter.
 */
ecs::FWorld ProjectBotStatsRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FBotStatsComponent>
        &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotStats(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected bot-position row into ECS.
 * @signature ecs::FWorld ProjectBotPositionRow(const ComponentsAdapters::TEcsRowProjectionPayload<FBotPositionComponent> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, selected bot position rows
 * should enter ECS through the systems/projection/bots/position adapter.
 */
ecs::FWorld ProjectBotPositionRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FBotPositionComponent>
        &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotPosition(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected bot-AI row into ECS.
 * @signature ecs::FWorld ProjectBotAIRow(const ComponentsAdapters::TEcsRowProjectionPayload<FBotAIComponent> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, selected bot AI rows
 * should enter ECS through the systems/projection/bots/ai adapter.
 */
ecs::FWorld ProjectBotAIRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FBotAIComponent>
        &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotAI(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects one selected bot-goal row into ECS.
 * @signature ecs::FWorld ProjectBotGoalRow(const ComponentsAdapters::TEcsRowProjectionPayload<FBotGoalComponent> &Payload)
 *
 * User Story: As RTK entity-adapter projection code, selected bot goal rows
 * should enter ECS through the systems/projection/bots/goals adapter.
 */
ecs::FWorld ProjectBotGoalRow(
    const ComponentsAdapters::TEcsRowProjectionPayload<FBotGoalComponent>
        &Payload) {
  return SystemsProjectionBotAdapters::ProjectBotGoal(
      {Payload.World, Payload.ItemValue});
}

/**
 * @brief Projects selected landmark rows into ECS.
 * @signature ecs::FWorld ProjectSelectedLandmarks(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected landmark rows should project
 * through entity adapters after slice reducers settle.
 */
ecs::FWorld
ProjectSelectedLandmarks(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FLandmark> Rows =
      RuntimeSelectors::SelectLandmarks(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FLandmark>{
          Payload.World, Rows, ProjectLandmarkRow, 0});
}

/**
 * @brief Projects selected nature rows into ECS.
 * @signature ecs::FWorld ProjectSelectedNature(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected nature rows should project
 * through entity adapters after slice reducers settle.
 */
ecs::FWorld ProjectSelectedNature(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FNatureFeatureSeed> Rows =
      RuntimeSelectors::SelectNatureFeatures(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FNatureFeatureSeed>{
          Payload.World, Rows, ProjectNatureFeatureRow, 0});
}

/**
 * @brief Projects selected townsperson rows into ECS.
 * @signature ecs::FWorld ProjectSelectedTownspeople(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected townsperson rows should project
 * through entity adapters after slice reducers settle.
 */
ecs::FWorld
ProjectSelectedTownspeople(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FTownspersonSeed> Rows =
      RuntimeSelectors::SelectTownspeople(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FTownspersonSeed>{
          Payload.World, Rows, ProjectTownspersonRow, 0});
}

/**
 * @brief Projects selected horse rows into ECS.
 * @signature ecs::FWorld ProjectSelectedHorses(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected horse rows should project
 * through entity adapters after slice reducers settle.
 */
ecs::FWorld ProjectSelectedHorses(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FHorseRouteSeed> Rows =
      RuntimeSelectors::SelectHorses(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FHorseRouteSeed>{
          Payload.World, Rows, ProjectHorseRow, 0});
}

/**
 * @brief Projects selected bot rows into ECS.
 * @signature ecs::FWorld ProjectSelectedBots(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected bot rows should project through
 * entity adapters after slice reducers settle.
 */
ecs::FWorld ProjectSelectedBots(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FBotEntity> Rows = RuntimeSelectors::SelectBots(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FBotEntity>{
          Payload.World, Rows, ProjectBotRow, 0});
}

/**
 * @brief Projects selected bot-stats rows into ECS.
 * @signature ecs::FWorld ProjectSelectedBotStats(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected bot stats rows should project
 * through the systems/projection/bots/stats adapter.
 */
ecs::FWorld
ProjectSelectedBotStats(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FBotStatsComponent> Rows =
      RuntimeSelectors::SelectBotStats(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FBotStatsComponent>{
          Payload.World, Rows, ProjectBotStatsRow, 0});
}

/**
 * @brief Projects selected bot-position rows into ECS.
 * @signature ecs::FWorld ProjectSelectedBotPositions(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected bot position rows should
 * project through the systems/projection/bots/position adapter.
 */
ecs::FWorld
ProjectSelectedBotPositions(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FBotPositionComponent> Rows =
      RuntimeSelectors::SelectBotPositions(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FBotPositionComponent>{
          Payload.World, Rows, ProjectBotPositionRow, 0});
}

/**
 * @brief Projects selected bot-AI rows into ECS.
 * @signature ecs::FWorld ProjectSelectedBotAI(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected bot AI rows should project
 * through the systems/projection/bots/ai adapter.
 */
ecs::FWorld ProjectSelectedBotAI(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FBotAIComponent> Rows =
      RuntimeSelectors::SelectBotAI(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FBotAIComponent>{
          Payload.World, Rows, ProjectBotAIRow, 0});
}

/**
 * @brief Projects selected bot-goal rows into ECS.
 * @signature ecs::FWorld ProjectSelectedBotGoals(const FProjectRuntimeWorldPayload &Payload)
 *
 * User Story: As the runtime reducer, selected bot goal rows should project
 * through the systems/projection/bots/goals adapter.
 */
ecs::FWorld ProjectSelectedBotGoals(const FProjectRuntimeWorldPayload &Payload) {
  const TArray<FBotGoalComponent> Rows =
      RuntimeSelectors::SelectBotGoals(Payload.State);
  return ComponentsAdapters::ProjectRows(
      ComponentsAdapters::TProjectRowsRequest<FBotGoalComponent>{
          Payload.World, Rows, ProjectBotGoalRow, 0});
}

/**
 * @brief Marks the runtime ECS projection resource as complete.
 * @signature ecs::FWorld ProjectRuntimeProjectedResource(ecs::FWorld World)
 *
 * User Story: As tests and diagnostics, I need a stable ECS resource proving
 * that the reducer completed the runtime projection pass.
 */
ecs::FWorld ProjectRuntimeProjectedResource(ecs::FWorld World) {
  return ComponentsAdapters::WithResource(
      {World, TEXT("Systems/Runtime/Projected"), ecs::boolValue(true)});
}

} // namespace

ecs::FWorld
ProjectRuntimeEcsWorld(const FProjectRuntimeEcsPayload &Payload) {
  return (func::pipe(ecs::createWorld()) |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedPlayer({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedTerrain({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedSpawn({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedInteraction({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedLandmarks({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedNature({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedTownspeople({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedHorses({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedBots({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedBotStats({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedBotPositions({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedBotAI({World, Payload.State});
          } |
          [&Payload](ecs::FWorld World) {
            return ProjectSelectedBotGoals({World, Payload.State});
          } |
          [](ecs::FWorld World) {
            return ProjectRuntimeProjectedResource(World);
          })
      .val;
}

} // namespace SystemsProjectionRuntimeAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
