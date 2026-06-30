#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionBotAdapters {
namespace {

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
  return ecs::mapComponentValues<FBotStrategicGoal>(
      Goals, [](const FBotStrategicGoal &Goal) {
        return StrategicGoalValue(Goal);
      });
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
 * @signature TArray<TArray<FString>> BuildBotProjectionDomains(const FString &BotSystemDomain)
 *
 * User Story: As an ECS inspector, I need each projected bot row connected to
 * its system domain and systems/projection/bots subdomain.
 */
TArray<TArray<FString>>
BuildBotProjectionDomains(const FString &BotSystemDomain) {
  return {{TEXT("Systems"), TEXT("Bots"), BotSystemDomain},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Bots")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
           BotSystemDomain}};
}

/**
 * @brief Builds ECS component steps from one bot stats row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildBotStatsComponentBindings(const FBotStatsComponent &Stats)
 *
 * User Story: As a runtime reducer, bot stats entity-adapter rows should
 * project into ECS stats and bot capability components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildBotStatsComponentBindings(const FBotStatsComponent &Stats) {
  return {{TEXT("Components/Stats/MoveSpeed"),
           ecs::floatValue(Stats.MoveSpeed)},
          {TEXT("Components/Stats/AwarenessRange"),
           ecs::floatValue(Stats.AwarenessRange)},
          {TEXT("Components/Stats/Resolve"), ecs::floatValue(Stats.Resolve)},
          {TEXT("Components/Bots/CanTalk"), ecs::boolValue(Stats.bCanTalk)},
          {TEXT("Components/Bots/MountedRider"),
           ecs::boolValue(Stats.bMountedRider)}};
}

/**
 * @brief Builds ECS component steps from one bot position row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildBotPositionComponentBindings(const FBotPositionComponent &Position)
 *
 * User Story: As a runtime reducer, bot position entity-adapter rows should
 * project into ECS local/world spatial components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildBotPositionComponentBindings(const FBotPositionComponent &Position) {
  return {{TEXT("Components/Spatial/LocalLocation"),
           ComponentsAdapters::LocalPointValue(Position.LocalLocation)},
          {TEXT("Components/Spatial/WorldLocation"),
           ecs::vec3Value(Position.WorldLocation)},
          {TEXT("Components/Spatial/HasWorldLocation"),
           ecs::boolValue(Position.bHasWorldLocation)},
          {TEXT("Components/Rendering/FacingRight"),
           ecs::boolValue(Position.bFacingRight)}};
}

/**
 * @brief Builds ECS component steps from one bot AI row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildBotAIComponentBindings(const FBotAIComponent &AI)
 *
 * User Story: As a runtime reducer, bot AI entity-adapter rows should project
 * behavior, target, and patrol route data into ECS components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildBotAIComponentBindings(const FBotAIComponent &AI) {
  return {{TEXT("Components/Bots/BehaviorState"),
           ecs::textValue(BotBehaviorText(AI.BehaviorState))},
          {TEXT("Components/Bots/TargetEntityId"),
           ecs::textValue(AI.TargetEntityId)},
          {TEXT("Components/Bots/TargetLocation"),
           ComponentsAdapters::LocalPointValue(AI.TargetLocation)},
          {TEXT("Components/Bots/HasTargetLocation"),
           ecs::boolValue(AI.bHasTargetLocation)},
          {TEXT("Components/Bots/PatrolIndex"), ecs::intValue(AI.PatrolIndex)},
          {TEXT("Components/Spatial/PatrolRoute"),
           ecs::listValue(ComponentsAdapters::LocalPointList(AI.PatrolRoute))}};
}

/**
 * @brief Builds ECS component steps from one bot goal row.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildBotGoalComponentBindings(const FBotGoalComponent &Goal)
 *
 * User Story: As a runtime reducer, bot goal entity-adapter rows should project
 * active goals, queued goals, and knowledge lists into ECS components.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildBotGoalComponentBindings(const FBotGoalComponent &Goal) {
  return {{TEXT("Components/Bots/HasActiveGoal"),
           ecs::boolValue(Goal.bHasActiveGoal)},
          {TEXT("Components/Bots/ActiveGoal"),
           StrategicGoalValue(Goal.ActiveGoal)},
          {TEXT("Components/Bots/GoalQueue"),
           ecs::listValue(StrategicGoalList(Goal.GoalQueue))},
          {TEXT("Components/Bots/KnownLandmarkIds"),
           ecs::listValue(
               ComponentsAdapters::StringList(Goal.Knowledge.KnownLandmarkIds))},
          {TEXT("Components/Bots/KnownBotIds"),
           ecs::listValue(
               ComponentsAdapters::StringList(Goal.Knowledge.KnownBotIds))}};
}

template <typename Payload, typename SelectId, typename SelectDomain,
          typename SelectComponents>
ecs::FWorld ProjectBotPayload(const Payload &PayloadValue,
                              SelectId SelectIdValue,
                              SelectDomain SelectDomainValue,
                              SelectComponents SelectComponentValues) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      PayloadValue,
      [SelectIdValue](const Payload &SelectedPayload) {
        return EntitiesAdapters::BotEntityKey(SelectIdValue(SelectedPayload));
      },
      [SelectDomainValue](const Payload &SelectedPayload) {
        return BuildBotProjectionDomains(SelectDomainValue(SelectedPayload));
      },
      SelectComponentValues);
}

} // namespace

ecs::FWorld ProjectBotStats(const FProjectBotStatsEcsPayload &Payload) {
  return ProjectBotPayload(
      Payload,
      [](const FProjectBotStatsEcsPayload &PayloadValue) {
        return PayloadValue.Stats.Id;
      },
      [](const FProjectBotStatsEcsPayload &) { return FString(TEXT("Stats")); },
      [](const FProjectBotStatsEcsPayload &PayloadValue) {
        return BuildBotStatsComponentBindings(PayloadValue.Stats);
      });
}

ecs::FWorld
ProjectBotPosition(const FProjectBotPositionEcsPayload &Payload) {
  return ProjectBotPayload(
      Payload,
      [](const FProjectBotPositionEcsPayload &PayloadValue) {
        return PayloadValue.Position.Id;
      },
      [](const FProjectBotPositionEcsPayload &) {
        return FString(TEXT("Position"));
      },
      [](const FProjectBotPositionEcsPayload &PayloadValue) {
        return BuildBotPositionComponentBindings(PayloadValue.Position);
      });
}

ecs::FWorld ProjectBotAI(const FProjectBotAIEcsPayload &Payload) {
  return ProjectBotPayload(
      Payload,
      [](const FProjectBotAIEcsPayload &PayloadValue) {
        return PayloadValue.AI.Id;
      },
      [](const FProjectBotAIEcsPayload &) { return FString(TEXT("AI")); },
      [](const FProjectBotAIEcsPayload &PayloadValue) {
        return BuildBotAIComponentBindings(PayloadValue.AI);
      });
}

ecs::FWorld ProjectBotGoal(const FProjectBotGoalEcsPayload &Payload) {
  return ProjectBotPayload(
      Payload,
      [](const FProjectBotGoalEcsPayload &PayloadValue) {
        return PayloadValue.Goal.Id;
      },
      [](const FProjectBotGoalEcsPayload &) { return FString(TEXT("Goals")); },
      [](const FProjectBotGoalEcsPayload &PayloadValue) {
        return BuildBotGoalComponentBindings(PayloadValue.Goal);
      });
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
