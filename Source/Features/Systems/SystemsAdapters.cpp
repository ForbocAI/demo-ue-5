#include "Features/Systems/SystemsAdapters.h"

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsAdapters {
namespace {

ecs::FComponentValue BotGoalTypeValue(EBotGoalType Type) {
  return ecs::textValue(Type == EBotGoalType::Patrol
                            ? TEXT("Patrol")
                            : Type == EBotGoalType::Converse
                                  ? TEXT("Converse")
                                  : Type == EBotGoalType::Travel
                                        ? TEXT("Travel")
                                        : TEXT("Idle"));
}

ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) {
  TMap<FString, ecs::FComponentValue> Fields;
  Fields.Add(TEXT("id"), ecs::textValue(Goal.Id));
  Fields.Add(TEXT("type"), BotGoalTypeValue(Goal.Type));
  Fields.Add(TEXT("priority"), ecs::intValue(Goal.Priority));
  Fields.Add(TEXT("targetEntityId"), ecs::textValue(Goal.TargetEntityId));
  Fields.Add(TEXT("targetLocation"),
             ComponentsAdapters::LocalPointValue(Goal.TargetLocation));
  Fields.Add(TEXT("hasTargetLocation"),
             ecs::boolValue(Goal.bHasTargetLocation));
  Fields.Add(TEXT("completed"), ecs::boolValue(Goal.bCompleted));
  return ecs::mapValue(Fields);
}

TArray<ecs::FComponentValue>
StrategicGoalList(const TArray<FBotStrategicGoal> &Goals, int32 Index = 0,
                  TArray<ecs::FComponentValue> Acc =
                      TArray<ecs::FComponentValue>()) {
  return Index >= Goals.Num()
             ? Acc
             : StrategicGoalList(
                   Goals, Index + 1,
                   ComponentsAdapters::AppendValue(
                       ComponentsAdapters::CreateAppendValueRequest(
                           Acc, StrategicGoalValue(Goals[Index]))));
}

FString BotBehaviorText(EBotBehaviorState State) {
  return State == EBotBehaviorState::Patrol
             ? TEXT("Patrol")
             : State == EBotBehaviorState::Moving
                   ? TEXT("Moving")
                   : State == EBotBehaviorState::Acting ? TEXT("Acting")
                                                        : TEXT("Idle");
}

ecs::FComponentValue InteractionCandidateValue(
    const FInteractionCandidate &Candidate) {
  TMap<FString, ecs::FComponentValue> Fields;
  Fields.Add(TEXT("index"), ecs::intValue(Candidate.Index));
  Fields.Add(TEXT("entityId"), ecs::textValue(Candidate.EntityId));
  Fields.Add(TEXT("location"), ecs::vec3Value(Candidate.Location));
  Fields.Add(TEXT("canInteract"), ecs::boolValue(Candidate.bCanInteract));
  return ecs::mapValue(Fields);
}

TArray<ecs::FComponentValue> InteractionCandidateList(
    const TArray<FInteractionCandidate> &Candidates, int32 Index = 0,
    TArray<ecs::FComponentValue> Acc = TArray<ecs::FComponentValue>()) {
  return Index >= Candidates.Num()
             ? Acc
             : InteractionCandidateList(
                   Candidates, Index + 1,
                   ComponentsAdapters::AppendValue(
                       ComponentsAdapters::CreateAppendValueRequest(
                           Acc, InteractionCandidateValue(Candidates[Index]))));
}

} // namespace

ecs::FWorld ProjectPlayer(ecs::FWorld World, const FPlayerState &Player) {
  return EntitiesAdapters::ProjectPlayer(World, Player);
}

ecs::FWorld ProjectTerrain(ecs::FWorld World, const FTerrainState &Terrain) {
  const ecs::EntityKey Entity = TEXT("level:terrain");
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Components"), TEXT("Level")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Terrain")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Level/TerrainLoaded"),
                Terrain.bTerrainLoaded);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/TerrainSource"),
                Terrain.TerrainSource);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/OrthoSource"),
                Terrain.OrthoSource);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithInt(
                Next, Entity, TEXT("Components/Level/GridSize"),
                Terrain.GridSize);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Level/MinElevationMeters"),
                Terrain.MinElevationMeters);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Level/MaxElevationMeters"),
                Terrain.MaxElevationMeters);
          })
      .val;
}

ecs::FWorld ProjectSpawn(ecs::FWorld World, const FSpawnState &Spawn) {
  const ecs::EntityKey Entity = TEXT("level:spawn:player");
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Spawn")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Characters"), TEXT("Player")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/Location"),
                Spawn.PlayerSpawn.Location);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponent(
                Next, Entity, TEXT("Components/Spatial/Rotation"),
                ComponentsAdapters::RotationValue(Spawn.PlayerSpawn.Rotation));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Level/AnchorLabel"),
                Spawn.PlayerSpawn.AnchorLabel);
          })
      .val;
}

ecs::FWorld ProjectBotStats(ecs::FWorld World,
                            const FBotStatsComponent &Stats) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Stats.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Bots"), TEXT("Stats")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Stats/MoveSpeed"),
                Stats.MoveSpeed);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Stats/AwarenessRange"),
                Stats.AwarenessRange);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Stats/Resolve"), Stats.Resolve);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/CanTalk"), Stats.bCanTalk);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/MountedRider"),
                Stats.bMountedRider);
          })
      .val;
}

ecs::FWorld ProjectBotPosition(ecs::FWorld World,
                               const FBotPositionComponent &Position) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Position.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Systems"), TEXT("Bots"), TEXT("Position")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponent(
                Next, Entity, TEXT("Components/Spatial/LocalLocation"),
                ComponentsAdapters::LocalPointValue(Position.LocalLocation));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/WorldLocation"),
                Position.WorldLocation);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Spatial/HasWorldLocation"),
                Position.bHasWorldLocation);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Rendering/FacingRight"),
                Position.bFacingRight);
          })
      .val;
}

ecs::FWorld ProjectBotAI(ecs::FWorld World, const FBotAIComponent &AI) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(AI.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Bots"), TEXT("AI")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/BehaviorState"),
                BotBehaviorText(AI.BehaviorState));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/TargetEntityId"),
                AI.TargetEntityId);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponent(
                Next, Entity, TEXT("Components/Bots/TargetLocation"),
                ComponentsAdapters::LocalPointValue(AI.TargetLocation));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/HasTargetLocation"),
                AI.bHasTargetLocation);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithInt(
                Next, Entity, TEXT("Components/Bots/PatrolIndex"),
                AI.PatrolIndex);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Spatial/PatrolRoute"),
                ComponentsAdapters::LocalPointList(AI.PatrolRoute));
          })
      .val;
}

ecs::FWorld ProjectBotGoal(ecs::FWorld World,
                           const FBotGoalComponent &Goal) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Goal.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Bots"), TEXT("Goals")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/HasActiveGoal"),
                Goal.bHasActiveGoal);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponent(
                Next, Entity, TEXT("Components/Bots/ActiveGoal"),
                StrategicGoalValue(Goal.ActiveGoal));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Bots/GoalQueue"),
                StrategicGoalList(Goal.GoalQueue));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Bots/KnownLandmarkIds"),
                ComponentsAdapters::StringList(
                    Goal.Knowledge.KnownLandmarkIds));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Bots/KnownBotIds"),
                ComponentsAdapters::StringList(Goal.Knowledge.KnownBotIds));
          })
      .val;
}

ecs::FWorld ProjectInteraction(ecs::FWorld World,
                               const FInteractionState &Interaction) {
  const ecs::EntityKey Entity = TEXT("systems:interaction:focus");
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Interaction")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/Origin"),
                Interaction.LastOrigin);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithFloat(
                Next, Entity, TEXT("Components/Spatial/MaxDistance"),
                Interaction.LastMaxDistance);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Interaction/Candidates"),
                InteractionCandidateList(Interaction.LastCandidates));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Interaction/Found"),
                Interaction.SelectedCandidate.bFound);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Interaction/EntityId"),
                Interaction.SelectedCandidate.EntityId);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithInt(
                Next, Entity, TEXT("Components/Interaction/CandidateIndex"),
                Interaction.SelectedCandidate.CandidateIndex);
          })
      .val;
}

ecs::FWorld ProjectRuntimeEcsWorld(const FRuntimeState &State) {
  return (func::pipe(ecs::createWorld()) |
          [&State](ecs::FWorld World) {
            return ProjectPlayer(World, RuntimeSelectors::SelectPlayerState(State));
          } |
          [&State](ecs::FWorld World) {
            return ProjectTerrain(World, State.Terrain);
          } |
          [&State](ecs::FWorld World) {
            return ProjectSpawn(World, State.Spawn);
          } |
          [&State](ecs::FWorld World) {
            return ProjectInteraction(
                World, RuntimeSelectors::SelectInteractionState(State));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FLandmark> Landmarks =
                RuntimeSelectors::SelectLandmarks(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Landmarks, EntitiesAdapters::ProjectLandmark));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FNatureFeatureSeed> Features =
                RuntimeSelectors::SelectNatureFeatures(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Features, EntitiesAdapters::ProjectNatureFeature));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FTownspersonSeed> Townspeople =
                RuntimeSelectors::SelectTownspeople(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Townspeople, EntitiesAdapters::ProjectTownsperson));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FHorseRouteSeed> Horses =
                RuntimeSelectors::SelectHorses(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Horses, EntitiesAdapters::ProjectHorse));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FBotEntity> Bots = RuntimeSelectors::SelectBots(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Bots, EntitiesAdapters::ProjectBot));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FBotStatsComponent> Stats =
                RuntimeSelectors::SelectBotStats(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Stats, ProjectBotStats));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FBotPositionComponent> Positions =
                RuntimeSelectors::SelectBotPositions(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(
                    World, Positions, ProjectBotPosition));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FBotAIComponent> AI =
                RuntimeSelectors::SelectBotAI(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(World, AI,
                                                             ProjectBotAI));
          } |
          [&State](ecs::FWorld World) {
            const TArray<FBotGoalComponent> Goals =
                RuntimeSelectors::SelectBotGoals(State);
            return ComponentsAdapters::ProjectMany(
                ComponentsAdapters::CreateProjectManyRequest(World, Goals,
                                                             ProjectBotGoal));
          } |
          [](ecs::FWorld World) {
            return ComponentsAdapters::WithResource(
                {World, TEXT("Systems/Runtime/Projected"),
                 ecs::boolValue(true)});
          })
      .val;
}

} // namespace SystemsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
