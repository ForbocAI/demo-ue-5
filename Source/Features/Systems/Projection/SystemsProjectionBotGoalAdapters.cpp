#include "Features/Systems/Projection/SystemsProjectionBotAdapters.h"

#include "Features/Systems/Projection/SystemsProjectionBotSharedAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace {

ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Goal, {"id", "type", "priority", "targetEntityId", "targetLocation",
             "hasTargetLocation", "completed"});
}

TArray<ecs::FComponentValue>
StrategicGoalList(const TArray<FBotStrategicGoal> &Goals) {
  return ecs::mapComponentValues<FBotStrategicGoal>(Goals, StrategicGoalValue);
}

ecs::FComponentValue BotKnowledgeValue(const FBotKnowledgeBase &Knowledge) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Knowledge, {"KnownLandmarkIds", "KnownBotIds"});
}

} // namespace

namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EBotGoalType> {
  static const TArray<TComponentTextDeclaration<EBotGoalType>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotGoalType>>
        RegisteredCases = {{EBotGoalType::Patrol, "Patrol"},
                           {EBotGoalType::Converse, "Converse"},
                           {EBotGoalType::Travel, "Travel"},
                           {EBotGoalType::Idle, "Idle"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotStrategicGoal> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotStrategicGoal>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotStrategicGoal>>
        RegisteredFields = {
            {"id", &FBotStrategicGoal::Id},
            {"type", &FBotStrategicGoal::Type},
            {"priority", &FBotStrategicGoal::Priority},
            {"targetEntityId", &FBotStrategicGoal::TargetEntityId},
            {"targetLocation", &FBotStrategicGoal::TargetLocation},
            {"hasTargetLocation", &FBotStrategicGoal::bHasTargetLocation},
            {"completed", &FBotStrategicGoal::bCompleted}};
    return RegisteredFields;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotKnowledgeBase> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotKnowledgeBase>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotKnowledgeBase>>
        RegisteredFields = {
            {"KnownLandmarkIds", &FBotKnowledgeBase::KnownLandmarkIds},
            {"KnownBotIds", &FBotKnowledgeBase::KnownBotIds}};
    return RegisteredFields;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotGoalComponent> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotGoalComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotGoalComponent>>
        RegisteredFields = {
            {"Id", &FBotGoalComponent::Id},
            {"HasActiveGoal", &FBotGoalComponent::bHasActiveGoal},
            {"ActiveGoal", &FBotGoalComponent::ActiveGoal,
             StrategicGoalValue},
            {"GoalQueue", &FBotGoalComponent::GoalQueue, StrategicGoalList},
            {"Knowledge", &FBotGoalComponent::Knowledge, BotKnowledgeValue}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FBotGoalComponent> {
  ecs::FComponentValue operator()(const FBotGoalComponent &Goal) const {
    return ComponentSourceValueMap(
        Goal, {"Id", "HasActiveGoal", "ActiveGoal", "GoalQueue",
               "Knowledge"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) {
  return ProjectBotPayload<FBotGoalComponent>(
      Payload,
      MakeBotProjectionRequest(
          ComponentsAdapters::ComponentAtom("Goals"),
          RegisteredComponentGroups<FBotGoalComponent>(
              {{"Components/Bots",
                {"HasActiveGoal",
                 "ActiveGoal",
                 "GoalQueue",
                 {"KnownLandmarkIds", {"Knowledge", "KnownLandmarkIds"}},
                 {"KnownBotIds", {"Knowledge", "KnownBotIds"}}}}})),
          [](const FProjectBotGoalPayload &PayloadValue) {
            return PayloadValue.Goal.Id;
          },
          [](const FProjectBotGoalPayload &PayloadValue) -> const FBotGoalComponent & {
            return PayloadValue.Goal;
          });
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
