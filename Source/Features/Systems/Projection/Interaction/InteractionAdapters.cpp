#include "Features/Systems/Projection/Interaction/InteractionAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace {

/**
 * @fn ecs::FComponentValue InteractionCandidateValue(const FCandidate &Candidate)
 * @brief Converts one interaction candidate into an ECS map value.
 *
 * User Story: As an ECS inspector, I need candidate details normalized as
 * component map fields instead of view-owned transient structs.
 */
ecs::FComponentValue
InteractionCandidateValue(const FCandidate &Candidate) {
  return ComponentsAdapters::ComponentValueMap(
      {{"index", Candidate.Index},
       {"entityId", Candidate.EntityId},
       {"location", Candidate.Location},
       {"canInteract", Candidate.bCanInteract}});
}

/**
 * @fn TArray<ecs::FComponentValue> InteractionCandidateList( const TArray<FCandidate> &Candidates)
 * @brief Converts selected interaction candidates into ECS list values.
 *
 * User Story: As systems projection code, interaction candidate mapping should
 * reuse neutral list projection instead of a sibling-domain helper.
 */
TArray<ecs::FComponentValue> InteractionCandidateList(
    const TArray<FCandidate> &Candidates) {
  return ecs::mapComponentValues<FCandidate>(
      Candidates, [](const FCandidate &Candidate) {
        return InteractionCandidateValue(Candidate);
      });
}

/** User Story: As a systems projection interaction consumer, I need to invoke interaction selection value through a stable signature so the systems projection interaction workflow remains explicit and composable. @fn ecs::FComponentValue InteractionSelectionValue(const FSelection &Selection) */
ecs::FComponentValue
InteractionSelectionValue(const FSelection &Selection) {
  return ComponentsAdapters::ComponentValueMap(
      {{"Found", Selection.bFound},
       {"EntityId", Selection.EntityId},
       {"CandidateIndex", Selection.CandidateIndex}});
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FInteractionState> {
  /** User Story: As a systems projection interaction consumer, I need to invoke the callable value through a stable signature so the systems projection interaction workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FInteractionState &Interaction) const */
  ecs::FComponentValue
  operator()(const FInteractionState &Interaction) const {
    return ComponentValueMap(
        {{"Origin", Interaction.Origin},
         {"MaxDistance", Interaction.MaxDistance},
         {"Candidates", InteractionCandidateList(Interaction.Candidates)},
         {"SelectedCandidate",
          InteractionSelectionValue(Interaction.SelectedCandidate)}});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionInteractionAdapters {
namespace {

using ComponentsAdapters::RegisteredComponentGroups;

/**
 * @fn ecs::EntityKey InteractionEntityKey()
 * @brief Returns the stable ECS entity key for interaction projection.
 *
 * User Story: As interaction projection code, I need one focus entity so each
 * reducer pass updates the same ECS interaction record.
 */
ecs::EntityKey InteractionEntityKey() {
  return TEXT("systems:interaction:focus");
}

/**
 * @fn TArray<TArray<FString>> BuildInteractionDomains()
 * @brief Builds ECS domain steps for interaction projection.
 *
 * User Story: As an ECS inspector, I need interaction state visible in both
 * systems/interaction and systems/projection/interaction subdomains.
 */
TArray<TArray<FString>> BuildInteractionDomains() {
  return {{TEXT("Systems"), TEXT("Interaction")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Interaction")}};
}

} // namespace

/** User Story: As a systems projection interaction consumer, I need to invoke project interaction through a stable signature so the systems projection interaction workflow remains explicit and composable. @fn ecs::FWorld ProjectInteraction(const FProjectInteractionPayload &Payload) */
ecs::FWorld
ProjectInteraction(const FProjectInteractionPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          func::constant<ecs::EntityKey>(InteractionEntityKey()),
          func::constant<TArray<TArray<FString>>>(BuildInteractionDomains()),
          [](const FProjectInteractionPayload &PayloadValue)
              -> const FInteractionState & {
            return PayloadValue.Interaction;
          },
          RegisteredComponentGroups<FInteractionState>(
              {{"Components/Spatial", {"Origin", "MaxDistance"}},
               {"Components/Interaction",
                {"Candidates",
                 {"Found", {"SelectedCandidate", "Found"}},
                 {"EntityId", {"SelectedCandidate", "EntityId"}},
                 {"CandidateIndex", {"SelectedCandidate", "CandidateIndex"}}}}
              })});
}

} // namespace SystemsProjectionInteractionAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
