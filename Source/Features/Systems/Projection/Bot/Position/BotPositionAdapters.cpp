#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Shared/SharedAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentSourceValueFieldRegistry<FBotPositionComponent> {
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
