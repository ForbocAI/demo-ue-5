#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Domains/DomainsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentSourceValueFieldRegistry<FBotPositionComponent> {
  /** User Story: As a projection bot position consumer, I need to invoke fields through a stable signature so the projection bot position workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FBotPositionComponent>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotPositionComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotPositionComponent>>
        SourceFields = ComponentSourceFieldDeclarations<FBotPositionComponent>({
            {"Id", &FBotPositionComponent::Id},
            {"LocalLocation", &FBotPositionComponent::LocalLocation},
            {"WorldLocation", &FBotPositionComponent::WorldLocation},
            {"HasWorldLocation", &FBotPositionComponent::bHasWorldLocation},
            {"FacingRight", &FBotPositionComponent::bFacingRight}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FBotPositionComponent> {
  /** User Story: As a projection bot position consumer, I need to invoke the callable value through a stable signature so the projection bot position workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FBotPositionComponent &Position) const */
  ecs::FComponentValue
  operator()(const FBotPositionComponent &Position) const {
    return ComponentSourceValueMap(
        Position, {"Id", "LocalLocation", "WorldLocation",
                   "HasWorldLocation", "FacingRight"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a projection bot position consumer, I need to invoke project bot position through a stable signature so the projection bot position workflow remains explicit and composable. @fn ecs::FWorld ProjectBotPosition(const FProjectBotPositionPayload &Payload) */
ecs::FWorld
ProjectBotPosition(const FProjectBotPositionPayload &Payload) {
  return ProjectBotPayload<FBotPositionComponent>(
      Payload,
      MakeBotProjectionRequest(
          ComponentsAdapters::ComponentAtom("Position"),
          RegisteredComponentGroups<FBotPositionComponent>(
              {{"Components/Spatial",
                {"LocalLocation", "WorldLocation", "HasWorldLocation"}},
               {"Components/Rendering", {"FacingRight"}}})),
          [](const FProjectBotPositionPayload &PayloadValue) {
            return PayloadValue.Position.Id;
          },
          [](const FProjectBotPositionPayload &PayloadValue)
              -> const FBotPositionComponent & {
            return PayloadValue.Position;
          });
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
