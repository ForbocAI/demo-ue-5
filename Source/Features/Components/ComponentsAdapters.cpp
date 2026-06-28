#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace ComponentsAdapters {

ecs::DomainPathKey DomainKey(const TArray<FString> &Segments) {
  return ecs::domainPathKey(ecs::createDomainPath(Segments));
}

ecs::FWorld WithDomain(const FEcsDomainProjectionRequest &Request) {
  return ecs::setEntityDomain(ecs::createSetEntityDomainRequest(
      Request.World, Request.Entity, DomainKey(Request.Segments)));
}

ecs::FWorld WithComponent(const FEcsComponentProjectionRequest &Request) {
  return ecs::setComponent(ecs::createSetComponentRequest(
      Request.World, Request.Entity, Request.Type, Request.Value));
}

ecs::FWorld WithText(const FEcsTextProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::textValue(Request.Value)});
}

ecs::FWorld WithBool(const FEcsBoolProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::boolValue(Request.Value)});
}

ecs::FWorld WithInt(const FEcsIntProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::intValue(Request.Value)});
}

ecs::FWorld WithFloat(const FEcsFloatProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::floatValue(Request.Value)});
}

ecs::FWorld WithVec3(const FEcsVec3ProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::vec3Value(Request.Value)});
}

ecs::FWorld WithList(const FEcsListProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::listValue(Request.Value)});
}

ecs::FWorld WithResource(const FEcsResourceProjectionRequest &Request) {
  return ecs::setResource(ecs::createSetResourceRequest(
      Request.World, Request.Name, Request.Value));
}

/**
 * @brief Applies one domain projection step from a step-list request.
 * @signature ecs::FWorld ProjectDomainStep(const FEcsDomainStepsProjectionRequest &Request)
 *
 * User Story: As an ECS projection adapter, I need batch domain wiring to stay
 * data-driven while preserving unary request flow.
 */
ecs::FWorld ProjectDomainStep(const FEcsDomainStepsProjectionRequest &Request) {
  const FEcsDomainProjectionStep Step = Request.Steps[Request.Index];
  return WithDomain({Request.World, Step.Entity, Step.Segments});
}

/**
 * @brief Recursively applies domain projection steps to a world.
 * @signature ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionRequest &Request)
 *
 * User Story: As a systems feature author, I need repeated ECS domain
 * projection to live in neutral component infrastructure instead of sibling
 * systems helpers.
 */
ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionRequest &Request) {
  return Request.Index >= Request.Steps.Num()
             ? Request.World
             : WithDomainSteps({ProjectDomainStep(Request), Request.Steps,
                                Request.Index + 1});
}

/**
 * @brief Applies one component projection step from a step-list request.
 * @signature ecs::FWorld ProjectComponentStep(const FEcsComponentStepsProjectionRequest &Request)
 *
 * User Story: As an ECS projection adapter, I need component writes to flow
 * through one request object so field batches compose predictably.
 */
ecs::FWorld
ProjectComponentStep(const FEcsComponentStepsProjectionRequest &Request) {
  const FEcsComponentProjectionStep Step = Request.Steps[Request.Index];
  return WithComponent({Request.World, Step.Entity, Step.Type, Step.Value});
}

/**
 * @brief Recursively applies component projection steps to a world.
 * @signature ecs::FWorld WithComponentSteps(const FEcsComponentStepsProjectionRequest &Request)
 *
 * User Story: As feature projection code, I need reusable component batch
 * application that keeps reducers and adapters small.
 */
ecs::FWorld
WithComponentSteps(const FEcsComponentStepsProjectionRequest &Request) {
  return Request.Index >= Request.Steps.Num()
             ? Request.World
             : WithComponentSteps({ProjectComponentStep(Request),
                                   Request.Steps, Request.Index + 1});
}

ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point) {
  TMap<FString, ecs::FComponentValue> Fields;
  Fields.Add(TEXT("eastWest"), ecs::floatValue(Point.EastWest));
  Fields.Add(TEXT("northSouth"), ecs::floatValue(Point.NorthSouth));
  Fields.Add(TEXT("heightOffset"), ecs::floatValue(Point.HeightOffset));
  return ecs::mapValue(Fields);
}

ecs::FComponentValue RotationValue(const FRotator &Rotation) {
  TMap<FString, ecs::FComponentValue> Fields;
  Fields.Add(TEXT("pitch"), ecs::floatValue(Rotation.Pitch));
  Fields.Add(TEXT("yaw"), ecs::floatValue(Rotation.Yaw));
  Fields.Add(TEXT("roll"), ecs::floatValue(Rotation.Roll));
  return ecs::mapValue(Fields);
}

TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points) {
  return MapComponentValues(TMapComponentValuesRequest<FLevelLocalPoint>{
      Points,
      [](const FLevelLocalPoint &Point) { return LocalPointValue(Point); }, 0,
      TArray<ecs::FComponentValue>()});
}

TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values) {
  return MapComponentValues(TMapComponentValuesRequest<FString>{
      Values, [](const FString &Value) { return ecs::textValue(Value); }, 0,
      TArray<ecs::FComponentValue>()});
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
