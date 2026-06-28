#include "Features/Systems/Projection/SystemsProjectionInteractionAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace SystemsProjectionInteractionAdapters {
namespace {

struct FBuildInteractionComponentStepsPayload {
  ecs::EntityKey Entity;
  const FInteractionState &Interaction;
};

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
 * @signature TArray<ComponentsAdapters::FEcsDomainProjectionStep> BuildInteractionDomainSteps(const ecs::EntityKey &Entity)
 *
 * User Story: As an ECS inspector, I need interaction state visible in both
 * systems/interaction and systems/projection/interaction subdomains.
 */
TArray<ComponentsAdapters::FEcsDomainProjectionStep>
BuildInteractionDomainSteps(const ecs::EntityKey &Entity) {
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Steps;
  Steps.Add({Entity, {TEXT("Systems"), TEXT("Interaction")}});
  Steps.Add(
      {Entity, {TEXT("Systems"), TEXT("Projection"), TEXT("Interaction")}});
  return Steps;
}

/**
 * @brief Builds ECS component steps from interaction slice state.
 * @signature TArray<ComponentsAdapters::FEcsComponentProjectionStep> BuildInteractionComponentSteps(const FBuildInteractionComponentStepsPayload &Payload)
 *
 * User Story: As a runtime reducer, interaction origin, range, candidates, and
 * selected result should project as ECS component data in one adapter pass.
 */
TArray<ComponentsAdapters::FEcsComponentProjectionStep>
BuildInteractionComponentSteps(
    const FBuildInteractionComponentStepsPayload &Payload) {
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Steps;
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/Origin"),
             ecs::vec3Value(Payload.Interaction.LastOrigin)});
  Steps.Add({Payload.Entity, TEXT("Components/Spatial/MaxDistance"),
             ecs::floatValue(Payload.Interaction.LastMaxDistance)});
  Steps.Add({Payload.Entity, TEXT("Components/Interaction/Candidates"),
             ecs::listValue(
                 InteractionCandidateList(Payload.Interaction.LastCandidates))});
  Steps.Add({Payload.Entity, TEXT("Components/Interaction/Found"),
             ecs::boolValue(Payload.Interaction.SelectedCandidate.bFound)});
  Steps.Add({Payload.Entity, TEXT("Components/Interaction/EntityId"),
             ecs::textValue(Payload.Interaction.SelectedCandidate.EntityId)});
  Steps.Add(
      {Payload.Entity, TEXT("Components/Interaction/CandidateIndex"),
       ecs::intValue(Payload.Interaction.SelectedCandidate.CandidateIndex)});
  return Steps;
}

} // namespace

ecs::FWorld
ProjectInteraction(const FProjectInteractionEcsPayload &Payload) {
  const ecs::EntityKey Entity = InteractionEntityKey();
  return (func::pipe(Payload.World) |
          [Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithDomainSteps(
                {World, BuildInteractionDomainSteps(Entity)});
          } |
          [&Payload, Entity](ecs::FWorld World) {
            return ComponentsAdapters::WithComponentSteps(
                {World, BuildInteractionComponentSteps(
                            {Entity, Payload.Interaction})});
          })
      .val;
}

} // namespace SystemsProjectionInteractionAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
