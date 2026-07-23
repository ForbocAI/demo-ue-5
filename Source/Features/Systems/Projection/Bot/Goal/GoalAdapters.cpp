#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Domains/DomainsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace {

/** User Story: As a projection bot goal consumer, I need to invoke strategic goal value through a stable signature so the projection bot goal workflow remains explicit and composable. @fn ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) */
ecs::FComponentValue StrategicGoalValue(const FBotStrategicGoal &Goal) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Goal, {"id", "type", "priority", "targetEntityId", "targetLocation",
             "hasTargetLocation", "completed"});
}

/** User Story: As a projection bot goal consumer, I need to invoke strategic goal list through a stable signature so the projection bot goal workflow remains explicit and composable. @fn TArray<ecs::FComponentValue> StrategicGoalList(const TArray<FBotStrategicGoal> &Goals) */
TArray<ecs::FComponentValue>
StrategicGoalList(const TArray<FBotStrategicGoal> &Goals) {
  return ecs::mapComponentValues<FBotStrategicGoal>(Goals, StrategicGoalValue);
}

/** User Story: As a projection bot goal consumer, I need to invoke bot knowledge value through a stable signature so the projection bot goal workflow remains explicit and composable. @fn ecs::FComponentValue BotKnowledgeValue(const FBotKnowledgeBase &Knowledge) */
ecs::FComponentValue BotKnowledgeValue(const FBotKnowledgeBase &Knowledge) {
  return ComponentsAdapters::ComponentSourceValueMap(
      Knowledge, {"KnownLandmarkIds", "KnownBotIds"});
}

} // namespace

namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EBotGoalType> {
  /** User Story: As a projection bot goal consumer, I need to invoke declarations through a stable signature so the projection bot goal workflow remains explicit and composable. @fn static const TArray<TComponentTextDeclaration<EBotGoalType>> &Declarations() */
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
  /** User Story: As a projection bot goal consumer, I need to invoke fields through a stable signature so the projection bot goal workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FBotStrategicGoal>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotStrategicGoal>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotStrategicGoal>>
        SourceFields = ComponentSourceFieldDeclarations<FBotStrategicGoal>({
            {"id", &FBotStrategicGoal::Id},
            {"type", &FBotStrategicGoal::Type},
            {"priority", &FBotStrategicGoal::Priority},
            {"targetEntityId", &FBotStrategicGoal::TargetEntityId},
            {"targetLocation", &FBotStrategicGoal::TargetLocation},
            {"hasTargetLocation", &FBotStrategicGoal::bHasTargetLocation},
            {"completed", &FBotStrategicGoal::bCompleted}});
    return SourceFields;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotKnowledgeBase> {
  /** User Story: As a projection bot goal consumer, I need to invoke fields through a stable signature so the projection bot goal workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FBotKnowledgeBase>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotKnowledgeBase>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotKnowledgeBase>>
        SourceFields = ComponentSourceFieldDeclarations<FBotKnowledgeBase>({
            {"KnownLandmarkIds", &FBotKnowledgeBase::KnownLandmarkIds},
            {"KnownBotIds", &FBotKnowledgeBase::KnownBotIds}});
    return SourceFields;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotGoalComponent> {
  /** User Story: As a projection bot goal consumer, I need to invoke fields through a stable signature so the projection bot goal workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FBotGoalComponent>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotGoalComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotGoalComponent>>
        SourceFields = ComponentSourceFieldDeclarations<FBotGoalComponent>({
            {"Id", &FBotGoalComponent::Id},
            {"HasActiveGoal", &FBotGoalComponent::bHasActiveGoal},
            {"ActiveGoal", &FBotGoalComponent::ActiveGoal,
             StrategicGoalValue},
            {"GoalQueue", &FBotGoalComponent::GoalQueue, StrategicGoalList},
            {"Knowledge", &FBotGoalComponent::Knowledge, BotKnowledgeValue}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FBotGoalComponent> {
  /** User Story: As a projection bot goal consumer, I need to invoke the callable value through a stable signature so the projection bot goal workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FBotGoalComponent &Goal) const */
  ecs::FComponentValue operator()(const FBotGoalComponent &Goal) const {
    return ComponentSourceValueMap(
        Goal, {"Id", "HasActiveGoal", "ActiveGoal", "GoalQueue",
               "Knowledge"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a projection bot goal consumer, I need to invoke project bot goal through a stable signature so the projection bot goal workflow remains explicit and composable. @fn ecs::FWorld ProjectBotGoal(const FProjectBotGoalPayload &Payload) */
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
