#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionBotAdapters {
namespace {

struct FBuildBotStatsComponentStepsPayload {
  ecs::EntityKey Entity;
  const FBotStatsComponent &Stats;
};

struct FBuildBotPositionComponentStepsPayload {
  ecs::EntityKey Entity;
  const FBotPositionComponent &Position;
};

struct FBuildBotAIComponentStepsPayload {
  ecs::EntityKey Entity;
  const FBotAIComponent &AI;
};

struct FBuildBotGoalComponentStepsPayload {
  ecs::EntityKey Entity;
  const FBotGoalComponent &Goal;
};

struct FBuildBotProjectionDomainStepsPayload {
  ecs::EntityKey Entity;
  FString BotSystemDomain;
};

/**
 * @brief Converts bot goal enum state into stable ECS text.
 * @signature FString BotGoalTypeText(EBotGoalType Type)
 *
 * User Story: As ECS projection code, I need closed enum cases rendered
 * explicitly so unknown values fail instead of becoming silent fallbacks.
 */
FString BotGoalTypeText(EBotGoalType Type) {
  switch (Type) {
  case EBotGoalType::Patrol:
    return TEXT("Patrol");
  case EBotGoalType::Converse:
    return TEXT("Converse");
  case EBotGoalType::Travel:
    return TEXT("Travel");
  case EBotGoalType::Idle:
    return TEXT("Idle");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Converts bot goal enum state into an ECS component value.
 * @signature ecs::FComponentValue BotGoalTypeValue(EBotGoalType Type)
 *
 * User Story: As a bot-goals adapter, I need enum text conversion wrapped as
 * a component value before building strategic goal maps.
 */
ecs::FComponentValue BotGoalTypeValue(EBotGoalType Type) {
  return ecs::textValue(BotGoalTypeText(Type));
}

/**
 * @brief Converts one strategic goal into an ECS map value.
 * @signature ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal)
 *
 * User Story: As an ECS inspector, I need active and queued goals normalized
 * with the same component field layout.
 */
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

/**
 * @brief Converts strategic goal arrays into ECS list values.
 * @signature TArray<ecs::FComponentValue> StrategicGoalList(const TArray<FBotStrategicGoal> &Goals)
 *
 * User Story: As bot-goals projection code, I need queued goal list mapping to
 * use the neutral component mapper shared with other systems.
 */
TArray<ecs::FComponentValue>
StrategicGoalList(const TArray<FBotStrategicGoal> &Goals) {
  return ComponentsAdapters::MapComponentValues(
      ComponentsAdapters::TMapComponentValuesRequest<FBotStrategicGoal>{
          Goals,
          [](const FBotStrategicGoal &Goal) {
            return StrategicGoalValue(Goal);
          },
          0, TArray<ecs::FComponentValue>()});
}

/**
 * @brief Converts bot behavior enum state into stable ECS text.
 * @signature FString BotBehaviorText(EBotBehaviorState State)
 *
 * User Story: As ECS projection code, I need behavior enum cases rendered
 * explicitly so invalid values do not masquerade as Idle.
 */
FString BotBehaviorText(EBotBehaviorState State) {
  switch (State) {
  case EBotBehaviorState::Idle:
    return TEXT("Idle");
  case EBotBehaviorState::Patrol:
    return TEXT("Patrol");
  case EBotBehaviorState::Moving:
    return TEXT("Moving");
  case EBotBehaviorState::Acting:
    return TEXT("Acting");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Builds ECS bot projection domain steps for one bot subdomain.
 * @signature TArray<ComponentsAdapters::FEcsDomainProjectionStep> BuildBotProjectionDomainSteps(const FBuildBotProjectionDomainStepsPayload &Payload)
 *
 * User Story: As an ECS inspector, I need each projected bot row connected to
 * its system domain and systems/projection/bots subdomain.
 */
TArray<ComponentsAdapters::FEcsDomainProjectionStep>
BuildBotProjectionDomainSteps(
    const FBuildBotProjectionDomainStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Steps;
  Steps.Add({Payload.Entity,
             {TEXT("Systems"), TEXT("Bots"), Payload.BotSystemDomain}});
  Steps.Add(
      {Payload.Entity, {TEXT("Systems"), TEXT("Projection"), TEXT("Bots")}});
  Steps.Add({Payload.Entity,
             {TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
              Payload.BotSystemDomain}});
  return Steps;
}

/**
 * @brief Builds ECS component steps from one bot stats row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildBotStatsComponentSteps(const FBuildBotStatsComponentStepsPayload &Payload)
 *
 * User Story: As a runtime reducer, bot stats entity-adapter rows should
 * project into ECS stats and bot capability components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildBotStatsComponentSteps(
    const FBuildBotStatsComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Stats/MoveSpeed"),
             ecs::floatValue(Payload.Stats.MoveSpeed)});
  Steps.Add({Payload.Entity, TEXT("Components/Stats/AwarenessRange"),
             ecs::floatValue(Payload.Stats.AwarenessRange)});
  Steps.Add({Payload.Entity, TEXT("Components/Stats/Resolve"),
             ecs::floatValue(Payload.Stats.Resolve)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/CanTalk"),
             ecs::boolValue(Payload.Stats.bCanTalk)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/MountedRider"),
             ecs::boolValue(Payload.Stats.bMountedRider)});
  return Steps;
}

/**
 * @brief Builds ECS component steps from one bot position row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildBotPositionComponentSteps(const FBuildBotPositionComponentStepsPayload &Payload)
 *
 * User Story: As a runtime reducer, bot position entity-adapter rows should
 * project into ECS local/world spatial components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildBotPositionComponentSteps(
    const FBuildBotPositionComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/LocalLocation"),
             ComponentsAdapters::LocalPointValue(
                 Payload.Position.LocalLocation)});
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/WorldLocation"),
             ecs::vec3Value(Payload.Position.WorldLocation)});
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/HasWorldLocation"),
             ecs::boolValue(Payload.Position.bHasWorldLocation)});
  Steps.Add({Payload.Entity, TEXT("Components/Rendering/FacingRight"),
             ecs::boolValue(Payload.Position.bFacingRight)});
  return Steps;
}

/**
 * @brief Builds ECS component steps from one bot AI row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildBotAIComponentSteps(const FBuildBotAIComponentStepsPayload &Payload)
 *
 * User Story: As a runtime reducer, bot AI entity-adapter rows should project
 * behavior, target, and patrol route data into ECS components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildBotAIComponentSteps(const FBuildBotAIComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Bots/BehaviorState"),
             ecs::textValue(BotBehaviorText(Payload.AI.BehaviorState))});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/TargetEntityId"),
             ecs::textValue(Payload.AI.TargetEntityId)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/TargetLocation"),
             ComponentsAdapters::LocalPointValue(Payload.AI.TargetLocation)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/HasTargetLocation"),
             ecs::boolValue(Payload.AI.bHasTargetLocation)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/PatrolIndex"),
             ecs::intValue(Payload.AI.PatrolIndex)});
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/PatrolRoute"),
             ecs::listValue(
                 ComponentsAdapters::LocalPointList(Payload.AI.PatrolRoute))});
  return Steps;
}

/**
 * @brief Builds ECS component steps from one bot goal row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildBotGoalComponentSteps(const FBuildBotGoalComponentStepsPayload &Payload)
 *
 * User Story: As a runtime reducer, bot goal entity-adapter rows should project
 * active goals, queued goals, and knowledge lists into ECS components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildBotGoalComponentSteps(const FBuildBotGoalComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Bots/HasActiveGoal"),
             ecs::boolValue(Payload.Goal.bHasActiveGoal)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/ActiveGoal"),
             StrategicGoalValue(Payload.Goal.ActiveGoal)});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/GoalQueue"),
             ecs::listValue(StrategicGoalList(Payload.Goal.GoalQueue))});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/KnownLandmarkIds"),
             ecs::listValue(ComponentsAdapters::StringList(
                 Payload.Goal.Knowledge.KnownLandmarkIds))});
  Steps.Add({Payload.Entity, TEXT("Components/Bots/KnownBotIds"),
             ecs::listValue(ComponentsAdapters::StringList(
                 Payload.Goal.Knowledge.KnownBotIds))});
  return Steps;
}

} // namespace

ecs::FWorld ProjectBotStats(const FProjectBotStatsEcsPayload &Payload) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Payload.Stats.Id);
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World,
                 BuildBotProjectionDomainSteps(
                     {Entity, FString(TEXT("Stats"))}),
                 0});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World, BuildBotStatsComponentSteps({Entity, Payload.Stats}),
                 0});
          })
      .val;
}

ecs::FWorld
ProjectBotPosition(const FProjectBotPositionEcsPayload &Payload) {
  const ecs::EntityKey Entity =
      EntitiesAdapters::BotEntityKey(Payload.Position.Id);
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World,
                 BuildBotProjectionDomainSteps(
                     {Entity, FString(TEXT("Position"))}),
                 0});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World,
                 BuildBotPositionComponentSteps({Entity, Payload.Position}),
                 0});
          })
      .val;
}

ecs::FWorld ProjectBotAI(const FProjectBotAIEcsPayload &Payload) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Payload.AI.Id);
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World,
                 BuildBotProjectionDomainSteps({Entity, FString(TEXT("AI"))}),
                 0});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World, BuildBotAIComponentSteps({Entity, Payload.AI}), 0});
          })
      .val;
}

ecs::FWorld ProjectBotGoal(const FProjectBotGoalEcsPayload &Payload) {
  const ecs::EntityKey Entity = EntitiesAdapters::BotEntityKey(Payload.Goal.Id);
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World,
                 BuildBotProjectionDomainSteps(
                     {Entity, FString(TEXT("Goals"))}),
                 0});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World, BuildBotGoalComponentSteps({Entity, Payload.Goal}),
                 0});
          })
      .val;
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
