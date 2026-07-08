#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Domains/DomainsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EBotBehaviorState> {
  static const TArray<TComponentTextDeclaration<EBotBehaviorState>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotBehaviorState>>
        RegisteredCases = {{EBotBehaviorState::Idle, "Idle"},
                           {EBotBehaviorState::Patrol, "Patrol"},
                           {EBotBehaviorState::Moving, "Moving"},
                           {EBotBehaviorState::Acting, "Acting"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotAIComponent> {
  static const TArray<TComponentSourceValueFieldDeclaration<FBotAIComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FBotAIComponent>>
        SourceFields = ComponentSourceFieldDeclarations<FBotAIComponent>({
            {"Id", &FBotAIComponent::Id},
            {"BehaviorState", &FBotAIComponent::BehaviorState},
            {"TargetEntityId", &FBotAIComponent::TargetEntityId},
            {"TargetLocation", &FBotAIComponent::TargetLocation},
            {"HasTargetLocation", &FBotAIComponent::bHasTargetLocation},
            {"PatrolIndex", &FBotAIComponent::PatrolIndex},
            {"PatrolRoute", &FBotAIComponent::PatrolRoute}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FBotAIComponent> {
  ecs::FComponentValue operator()(const FBotAIComponent &AI) const {
    return ComponentSourceValueMap(
        AI, {"Id", "BehaviorState", "TargetEntityId", "TargetLocation",
             "HasTargetLocation", "PatrolIndex", "PatrolRoute"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::FWorld ProjectBotAI(const FProjectBotAIPayload &Payload) {
  return ProjectBotPayload<FBotAIComponent>(
      Payload,
      MakeBotProjectionRequest(
          ComponentsAdapters::ComponentAtom("AI"),
          RegisteredComponentGroups<FBotAIComponent>(
              {{"Components/Bots",
                {"BehaviorState", "TargetEntityId", "TargetLocation",
                 "HasTargetLocation", "PatrolIndex"}},
               {"Components/Spatial", {"PatrolRoute"}}})),
          [](const FProjectBotAIPayload &PayloadValue) {
            return PayloadValue.AI.Id;
          },
          [](const FProjectBotAIPayload &PayloadValue) -> const FBotAIComponent & {
            return PayloadValue.AI;
          });
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
