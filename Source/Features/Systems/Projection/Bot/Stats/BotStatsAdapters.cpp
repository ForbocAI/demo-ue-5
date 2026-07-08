#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Shared/SharedAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentSourceValueFieldRegistry<FBotStatsComponent> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotStatsComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotStatsComponent>>
        RegisteredFields = {{"Id", &FBotStatsComponent::Id},
                            {"MoveSpeed", &FBotStatsComponent::MoveSpeed},
                            {"AwarenessRange",
                             &FBotStatsComponent::AwarenessRange},
                            {"Resolve", &FBotStatsComponent::Resolve},
                            {"CanTalk", &FBotStatsComponent::bCanTalk},
                            {"MountedRider",
                             &FBotStatsComponent::bMountedRider}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FBotStatsComponent> {
  ecs::FComponentValue operator()(const FBotStatsComponent &Stats) const {
    return ComponentSourceValueMap(
        Stats, {"Id", "MoveSpeed", "AwarenessRange", "Resolve", "CanTalk",
                "MountedRider"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) {
  return ProjectBotPayload<FBotStatsComponent>(
      Payload,
      MakeBotProjectionRequest(
          ComponentsAdapters::ComponentAtom("Stats"),
          RegisteredComponentGroups<FBotStatsComponent>(
              {{"Components/Stats", {"MoveSpeed", "AwarenessRange", "Resolve"}},
               {"Components/Bots", {"CanTalk", "MountedRider"}}})),
          [](const FProjectBotStatsPayload &PayloadValue) {
            return PayloadValue.Stats.Id;
          },
          [](const FProjectBotStatsPayload &PayloadValue) -> const FBotStatsComponent & {
            return PayloadValue.Stats;
          });
}

} // namespace SystemsProjectionBotAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
