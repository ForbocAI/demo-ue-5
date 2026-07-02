#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace {

/**
 * @brief Converts bot goal enum state into stable ECS text.
 * @signature FString BotGoalTypeText(EBotGoalType Type)
 *
 * User Story: As ECS projection code, I need closed enum cases rendered
 * explicitly so unknown values fail instead of becoming silent fallbacks.
 */
FString BotGoalTypeText(EBotGoalType Type) {
  return ComponentsAdapters::ComponentText(
      Type, {{EBotGoalType::Patrol, "Patrol"},
             {EBotGoalType::Converse, "Converse"},
             {EBotGoalType::Travel, "Travel"},
             {EBotGoalType::Idle, "Idle"}});
}

/**
 * @brief Converts one strategic goal into an ECS map value.
 * @signature ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal)
 *
 * User Story: As an ECS inspector, I need active and queued goals normalized
 * with the same component field layout.
 */
ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Goal, {{"id", &FBotStrategicGoal::Id},
             {"type", &FBotStrategicGoal::Type, BotGoalTypeText},
             {"priority", &FBotStrategicGoal::Priority},
             {"targetEntityId", &FBotStrategicGoal::TargetEntityId},
             {"targetLocation", &FBotStrategicGoal::TargetLocation},
             {"hasTargetLocation", &FBotStrategicGoal::bHasTargetLocation},
             {"completed", &FBotStrategicGoal::bCompleted}});
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
  return ecs::mapComponentValues<FBotStrategicGoal>(Goals, StrategicGoalValue);
}

ecs::FComponentValue BotKnowledgeValue(const FBotKnowledgeBase &Knowledge) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Knowledge,
      {{"KnownLandmarkIds", &FBotKnowledgeBase::KnownLandmarkIds},
       {"KnownBotIds", &FBotKnowledgeBase::KnownBotIds}});
}

/**
 * @brief Converts bot behavior enum state into stable ECS text.
 * @signature FString BotBehaviorText(EBotBehaviorState State)
 *
 * User Story: As ECS projection code, I need behavior enum cases rendered
 * explicitly so invalid values do not masquerade as Idle.
 */
FString BotBehaviorText(EBotBehaviorState State) {
  return ComponentsAdapters::ComponentText(
      State, {{EBotBehaviorState::Idle, "Idle"},
              {EBotBehaviorState::Patrol, "Patrol"},
              {EBotBehaviorState::Moving, "Moving"},
              {EBotBehaviorState::Acting, "Acting"}});
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FBotStatsComponent> {
  ecs::FComponentValue operator()(const FBotStatsComponent &Stats) const {
    return ComponentSourceValueMap(
        Stats, {{"Id", &FBotStatsComponent::Id},
                {"MoveSpeed", &FBotStatsComponent::MoveSpeed},
                {"AwarenessRange", &FBotStatsComponent::AwarenessRange},
                {"Resolve", &FBotStatsComponent::Resolve},
                {"CanTalk", &FBotStatsComponent::bCanTalk},
                {"MountedRider", &FBotStatsComponent::bMountedRider}});
  }
};

template <>
struct TComponentSourceProjector<FBotPositionComponent> {
  ecs::FComponentValue
  operator()(const FBotPositionComponent &Position) const {
    return ComponentSourceValueMap(
        Position, {{"Id", &FBotPositionComponent::Id},
                   {"LocalLocation", &FBotPositionComponent::LocalLocation},
                   {"WorldLocation", &FBotPositionComponent::WorldLocation},
                   {"HasWorldLocation",
                    &FBotPositionComponent::bHasWorldLocation},
                   {"FacingRight", &FBotPositionComponent::bFacingRight}});
  }
};

template <>
struct TComponentSourceProjector<FBotAIComponent> {
  ecs::FComponentValue operator()(const FBotAIComponent &AI) const {
    return ComponentSourceValueMap(
        AI, {{"Id", &FBotAIComponent::Id},
             {"BehaviorState", &FBotAIComponent::BehaviorState,
              BotBehaviorText},
             {"TargetEntityId", &FBotAIComponent::TargetEntityId},
             {"TargetLocation", &FBotAIComponent::TargetLocation},
             {"HasTargetLocation", &FBotAIComponent::bHasTargetLocation},
             {"PatrolIndex", &FBotAIComponent::PatrolIndex},
             {"PatrolRoute", &FBotAIComponent::PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FBotGoalComponent> {
  ecs::FComponentValue operator()(const FBotGoalComponent &Goal) const {
    return ComponentSourceValueMap(
        Goal, {{"Id", &FBotGoalComponent::Id},
               {"HasActiveGoal", &FBotGoalComponent::bHasActiveGoal},
               {"ActiveGoal", &FBotGoalComponent::ActiveGoal,
                StrategicGoalValue},
               {"GoalQueue", &FBotGoalComponent::GoalQueue,
                StrategicGoalList},
               {"Knowledge", &FBotGoalComponent::Knowledge,
                BotKnowledgeValue}});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {
namespace {

using ComponentsAdapters::RegisteredComponentGroups;

/**
 * @brief Builds ECS bot projection domain steps for one bot subdomain.
 * @signature TArray<TArray<FString>> BuildBotProjectionDomains(const FString &BotSystemDomain)
 *
 * User Story: As an ECS inspector, I need each projected bot row connected to
 * its system domain and systems/projection/bots subdomain.
 */
TArray<TArray<FString>>
BuildBotProjectionDomains(const FString &BotSystemDomain) {
  return {ComponentsAdapters::ComponentDomain({"Systems", "Bots"},
                                              BotSystemDomain),
          ComponentsAdapters::ComponentDomain(
              {"Systems", "Projection", "Bots"}),
          ComponentsAdapters::ComponentDomain(
              {"Systems", "Projection", "Bots"}, BotSystemDomain)};
}

template <typename Payload, typename SelectId, typename SelectSource,
          typename ComponentCatalog>
ecs::FWorld ProjectBotPayload(const Payload &PayloadValue,
                              const FString &BotSystemDomain,
                              SelectId SelectIdValue,
                              SelectSource SelectSourceValue,
                              const ComponentCatalog &Components) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      PayloadValue,
      [SelectIdValue](const Payload &SelectedPayload) {
        return EntitiesAdapters::BotEntityKey(SelectIdValue(SelectedPayload));
      },
      func::constant<TArray<TArray<FString>>>(
          BuildBotProjectionDomains(BotSystemDomain)),
      SelectSourceValue, Components);
}

} // namespace

ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) {
  return ProjectBotPayload(
      Payload, ComponentsAdapters::ComponentAtom("Stats"),
      [](const FProjectBotStatsPayload &PayloadValue) {
        return PayloadValue.Stats.Id;
      },
      [](const FProjectBotStatsPayload &PayloadValue) -> const FBotStatsComponent & {
        return PayloadValue.Stats;
      },
      RegisteredComponentGroups<FBotStatsComponent>(
          {{"Components/Stats", {"MoveSpeed", "AwarenessRange", "Resolve"}},
           {"Components/Bots", {"CanTalk", "MountedRider"}}}));
}

ecs::FWorld
ProjectBotPosition(const FProjectBotPositionPayload &Payload) {
  return ProjectBotPayload(
      Payload, ComponentsAdapters::ComponentAtom("Position"),
      [](const FProjectBotPositionPayload &PayloadValue) {
        return PayloadValue.Position.Id;
      },
      [](const FProjectBotPositionPayload &PayloadValue)
          -> const FBotPositionComponent & {
        return PayloadValue.Position;
      },
      RegisteredComponentGroups<FBotPositionComponent>(
          {{"Components/Spatial",
            {"LocalLocation", "WorldLocation", "HasWorldLocation"}},
           {"Components/Rendering", {"FacingRight"}}}));
}

ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload) {
  return ProjectBotPayload(
      Payload, ComponentsAdapters::ComponentAtom("AI"),
      [](const FProjectBotAIPayload &PayloadValue) {
        return PayloadValue.AI.Id;
      },
      [](const FProjectBotAIPayload &PayloadValue) -> const FBotAIComponent & {
        return PayloadValue.AI;
      },
      RegisteredComponentGroups<FBotAIComponent>(
          {{"Components/Bots",
            {"BehaviorState", "TargetEntityId", "TargetLocation",
             "HasTargetLocation", "PatrolIndex"}},
           {"Components/Spatial", {"PatrolRoute"}}}));
}

ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) {
  return ProjectBotPayload(
      Payload, ComponentsAdapters::ComponentAtom("Goals"),
      [](const FProjectBotGoalPayload &PayloadValue) {
        return PayloadValue.Goal.Id;
      },
      [](const FProjectBotGoalPayload &PayloadValue)
          -> const FBotGoalComponent & {
        return PayloadValue.Goal;
      },
      RegisteredComponentGroups<FBotGoalComponent>(
          {{"Components/Bots",
            {"HasActiveGoal",
             "ActiveGoal",
             "GoalQueue",
             {"KnownLandmarkIds", {"Knowledge", "KnownLandmarkIds"}},
             {"KnownBotIds", {"Knowledge", "KnownBotIds"}}}}}));
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
