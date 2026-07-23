#include "Features/Systems/Projection/Bot/ProjectionBotAdapters.h"

#include "Features/Systems/Projection/Bot/Domains/DomainsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentSourceValueFieldRegistry<FBotStatsComponent> {
  /** User Story: As a projection bot stats consumer, I need to invoke fields through a stable signature so the projection bot stats workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FBotStatsComponent>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FBotStatsComponent>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FBotStatsComponent>>
        SourceFields = ComponentSourceFieldDeclarations<FBotStatsComponent>(
            {{"Id", &FBotStatsComponent::Id},
             {"MoveSpeed", &FBotStatsComponent::MoveSpeed},
             {"AwarenessRange", &FBotStatsComponent::AwarenessRange},
             {"Resolve", &FBotStatsComponent::Resolve},
             {"CanTalk", &FBotStatsComponent::bCanTalk},
             {"MountedRider", &FBotStatsComponent::bMountedRider}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FBotStatsComponent> {
  /** User Story: As a projection bot stats consumer, I need to invoke the callable value through a stable signature so the projection bot stats workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FBotStatsComponent &Stats) const */
  ecs::FComponentValue operator()(const FBotStatsComponent &Stats) const {
    return ComponentSourceValueMap(
        Stats, {"Id", "MoveSpeed", "AwarenessRange", "Resolve", "CanTalk",
                "MountedRider"});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionBotAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a projection bot stats consumer, I need to invoke project bot stats through a stable signature so the projection bot stats workflow remains explicit and composable. @fn ecs::FWorld ProjectBotStats(const FProjectBotStatsPayload &Payload) */
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
