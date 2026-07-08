#include "Features/Systems/Projection/Interaction/Adapters.h"

#include "Features/Components/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace {

/**
 * @brief Converts one interaction candidate into an ECS map value.
 * @signature ecs::FComponentValue InteractionCandidateValue(const FInteractionCandidate &Candidate)
 *
 * User Story: As an ECS inspector, I need candidate details normalized as
 * component map fields instead of view-owned transient structs.
 */
ecs::FComponentValue
InteractionCandidateValue(const FInteractionCandidate &Candidate) {
  return ComponentsAdapters::ComponentValueMap(
      {{"index", Candidate.Index},
       {"entityId", Candidate.EntityId},
       {"location", Candidate.Location},
       {"canInteract", Candidate.bCanInteract}});
}

/**
 * @brief Converts selected interaction candidates into ECS list values.
 * @signature TArray<ecs::FComponentValue> InteractionCandidateList(const TArray<FInteractionCandidate> &Candidates)
 *
 * User Story: As systems projection code, interaction candidate mapping should
 * reuse neutral list projection instead of a sibling-domain helper.
 */
TArray<ecs::FComponentValue> InteractionCandidateList(
    const TArray<FInteractionCandidate> &Candidates) {
  return ecs::mapComponentValues<FInteractionCandidate>(
      Candidates, [](const FInteractionCandidate &Candidate) {
        return InteractionCandidateValue(Candidate);
      });
}

ecs::FComponentValue
InteractionSelectionValue(const FInteractionSelection &Selection) {
  return ComponentsAdapters::ComponentValueMap(
      {{"Found", Selection.bFound},
       {"EntityId", Selection.EntityId},
       {"CandidateIndex", Selection.CandidateIndex}});
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FInteractionState> {
  ecs::FComponentValue
  operator()(const FInteractionState &Interaction) const {
    return ComponentValueMap(
        {{"Origin", Interaction.LastOrigin},
         {"MaxDistance", Interaction.LastMaxDistance},
         {"Candidates", InteractionCandidateList(Interaction.LastCandidates)},
         {"SelectedCandidate",
          InteractionSelectionValue(Interaction.SelectedCandidate)}});
  }
};

} // namespace ComponentsAdapters

namespace SystemsProjectionInteractionAdapters {
namespace {

using ComponentsAdapters::RegisteredComponentGroups;

/**
 * @brief Returns the stable ECS entity key for interaction projection.
 * @signature ecs::EntityKey InteractionEntityKey()
 *
 * User Story: As interaction projection code, I need one focus entity so each
 * reducer pass updates the same ECS interaction record.
 */
ecs::EntityKey InteractionEntityKey() {
  return TEXT("systems:interaction:focus");
}

/**
 * @brief Builds ECS domain steps for interaction projection.
 * @signature TArray<TArray<FString>> BuildInteractionDomains()
 *
 * User Story: As an ECS inspector, I need interaction state visible in both
 * systems/interaction and systems/projection/interaction subdomains.
 */
TArray<TArray<FString>> BuildInteractionDomains() {
  return {{TEXT("Systems"), TEXT("Interaction")},
          {TEXT("Systems"), TEXT("Projection"), TEXT("Interaction")}};
}

} // namespace

ecs::FWorld
ProjectInteraction(const FProjectInteractionPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
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
