#include "Features/Systems/Projection/SystemsProjectionInteractionAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionInteractionAdapters {
namespace {

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
 * @brief Converts one interaction candidate into an ECS map value.
 * @signature ecs::FComponentValue InteractionCandidateValue(const FInteractionCandidate &Candidate)
 *
 * User Story: As an ECS inspector, I need candidate details normalized as
 * component map fields instead of view-owned transient structs.
 */
ecs::FComponentValue
InteractionCandidateValue(const FInteractionCandidate &Candidate) {
  TMap<FString, ecs::FComponentValue> Fields;
  Fields.Add(TEXT("index"), ecs::intValue(Candidate.Index));
  Fields.Add(TEXT("entityId"), ecs::textValue(Candidate.EntityId));
  Fields.Add(TEXT("location"), ecs::vec3Value(Candidate.Location));
  Fields.Add(TEXT("canInteract"), ecs::boolValue(Candidate.bCanInteract));
  return ecs::mapValue(Fields);
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

/**
 * @brief Builds ECS component steps from interaction slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionBinding> BuildInteractionComponentBindings(const FInteractionState &Interaction)
 *
 * User Story: As a runtime reducer, interaction origin, range, candidates, and
 * selected result should project as ECS component data in one adapter pass.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BuildInteractionComponentBindings(const FInteractionState &Interaction) {
  return {{TEXT("Components/Spatial/Origin"),
           ecs::vec3Value(Interaction.LastOrigin)},
          {TEXT("Components/Spatial/MaxDistance"),
           ecs::floatValue(Interaction.LastMaxDistance)},
          {TEXT("Components/Interaction/Candidates"),
           ecs::listValue(InteractionCandidateList(Interaction.LastCandidates))},
          {TEXT("Components/Interaction/Found"),
           ecs::boolValue(Interaction.SelectedCandidate.bFound)},
          {TEXT("Components/Interaction/EntityId"),
           ecs::textValue(Interaction.SelectedCandidate.EntityId)},
          {TEXT("Components/Interaction/CandidateIndex"),
           ecs::intValue(Interaction.SelectedCandidate.CandidateIndex)}};
}

} // namespace

ecs::FWorld
ProjectInteraction(const FProjectInteractionEcsPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectInteractionEcsPayload &) {
        return InteractionEntityKey();
      },
      [](const FProjectInteractionEcsPayload &) {
        return BuildInteractionDomains();
      },
      [](const FProjectInteractionEcsPayload &PayloadValue) {
        return BuildInteractionComponentBindings(PayloadValue.Interaction);
      });
}

} // namespace SystemsProjectionInteractionAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
