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
  const func::Maybe<FString> Text =
      func::multi_match<EBotGoalType, FString>(
          Type, {func::when<EBotGoalType, FString>(
                     func::equals(EBotGoalType::Patrol),
                     [](const EBotGoalType &) { return FString(TEXT("Patrol")); }),
                 func::when<EBotGoalType, FString>(
                     func::equals(EBotGoalType::Converse),
                     [](const EBotGoalType &) { return FString(TEXT("Converse")); }),
                 func::when<EBotGoalType, FString>(
                     func::equals(EBotGoalType::Travel),
                     [](const EBotGoalType &) { return FString(TEXT("Travel")); }),
                 func::when<EBotGoalType, FString>(
                     func::equals(EBotGoalType::Idle),
                     [](const EBotGoalType &) { return FString(TEXT("Idle")); })});
  check(Text.hasValue);
  return Text.value;
}

/**
 * @brief Converts bot goal enum state into an ECS component value.
 * @signature ecs::FComponentValue BotGoalTypeValue(EBotGoalType Type)
 *
 * User Story: As a bot-goals adapter, I need enum text conversion wrapped as
 * a component value before building strategic goal maps.
 */
ecs::FComponentValue BotGoalTypeValue(EBotGoalType Type) {
  return ecs::createTextComponentValue(BotGoalTypeText(Type));
}

/**
 * @brief Converts one strategic goal into an ECS map value.
 * @signature ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal)
 *
 * User Story: As an ECS inspector, I need active and queued goals normalized
 * with the same component field layout.
 */
ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) {
  return ComponentsAdapters::ComponentValueMap(
      {{"id", Goal.Id},
       {"type", BotGoalTypeValue(Goal.Type)},
       {"priority", Goal.Priority},
       {"targetEntityId", Goal.TargetEntityId},
       {"targetLocation", Goal.TargetLocation},
       {"hasTargetLocation", Goal.bHasTargetLocation},
       {"completed", Goal.bCompleted}});
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

ecs::FComponentValue BotKnowledgeValue(const FBotKnowledgeBase &Knowledge) {
  return ComponentsAdapters::ComponentValueMap(
      {{"KnownLandmarkIds", Knowledge.KnownLandmarkIds},
       {"KnownBotIds", Knowledge.KnownBotIds}});
}

/**
 * @brief Converts bot behavior enum state into stable ECS text.
 * @signature FString BotBehaviorText(EBotBehaviorState State)
 *
 * User Story: As ECS projection code, I need behavior enum cases rendered
 * explicitly so invalid values do not masquerade as Idle.
 */
FString BotBehaviorText(EBotBehaviorState State) {
  const func::Maybe<FString> Text =
      func::multi_match<EBotBehaviorState, FString>(
          State, {func::when<EBotBehaviorState, FString>(
                      func::equals(EBotBehaviorState::Idle),
                      [](const EBotBehaviorState &) {
                        return FString(TEXT("Idle"));
                      }),
                  func::when<EBotBehaviorState, FString>(
                      func::equals(EBotBehaviorState::Patrol),
                      [](const EBotBehaviorState &) {
                        return FString(TEXT("Patrol"));
                      }),
                  func::when<EBotBehaviorState, FString>(
                      func::equals(EBotBehaviorState::Moving),
                      [](const EBotBehaviorState &) {
                        return FString(TEXT("Moving"));
                      }),
                  func::when<EBotBehaviorState, FString>(
                      func::equals(EBotBehaviorState::Acting),
                      [](const EBotBehaviorState &) {
                        return FString(TEXT("Acting"));
                      })});
  check(Text.hasValue);
  return Text.value;
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FBotStatsComponent> {
  ecs::FComponentValue operator()(const FBotStatsComponent &Stats) const {
    return ComponentValueMap({{"Id", Stats.Id},
                              {"MoveSpeed", Stats.MoveSpeed},
                              {"AwarenessRange", Stats.AwarenessRange},
                              {"Resolve", Stats.Resolve},
                              {"CanTalk", Stats.bCanTalk},
                              {"MountedRider", Stats.bMountedRider}});
  }
};

template <>
struct TComponentSourceProjector<FBotPositionComponent> {
  ecs::FComponentValue
  operator()(const FBotPositionComponent &Position) const {
    return ComponentValueMap(
        {{"Id", Position.Id},
         {"LocalLocation", Position.LocalLocation},
         {"WorldLocation", Position.WorldLocation},
         {"HasWorldLocation", Position.bHasWorldLocation},
         {"FacingRight", Position.bFacingRight}});
  }
};

template <>
struct TComponentSourceProjector<FBotAIComponent> {
  ecs::FComponentValue operator()(const FBotAIComponent &AI) const {
    return ComponentValueMap({{"Id", AI.Id},
                              {"BehaviorState",
                               BotBehaviorText(AI.BehaviorState)},
                              {"TargetEntityId", AI.TargetEntityId},
                              {"TargetLocation", AI.TargetLocation},
                              {"HasTargetLocation", AI.bHasTargetLocation},
                              {"PatrolIndex", AI.PatrolIndex},
                              {"PatrolRoute", AI.PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FBotGoalComponent> {
  ecs::FComponentValue operator()(const FBotGoalComponent &Goal) const {
    return ComponentValueMap(
        {{"Id", Goal.Id},
         {"HasActiveGoal", Goal.bHasActiveGoal},
         {"ActiveGoal", StrategicGoalValue(Goal.ActiveGoal)},
         {"GoalQueue", StrategicGoalList(Goal.GoalQueue)},
         {"Knowledge", BotKnowledgeValue(Goal.Knowledge)}});
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
  return {{TEXT("Systems"), TEXT("Bots"), BotSystemDomain},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Bots")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
           BotSystemDomain}};
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
      Payload, TEXT("Stats"),
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
      Payload, TEXT("Position"),
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
      Payload, TEXT("AI"),
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
      Payload, TEXT("Goals"),
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
