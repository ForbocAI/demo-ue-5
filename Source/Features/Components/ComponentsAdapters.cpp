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

ecs::FWorld ProjectDomainStep(const ecs::FWorld &World,
                              const FEcsDomainProjectionStep &Step) {
  return WithDomain({World, Step.Entity, Step.Segments});
}

/**
 * @brief Applies domain projection steps to a world through the ECS fold.
 * @signature ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionPayload &Payload)
 *
 * User Story: As a systems feature author, I need repeated ECS domain
 * projection to live in neutral component infrastructure instead of sibling
 * systems helpers.
 */
ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionPayload &Payload) {
  return ecs::foldArray<FEcsDomainProjectionStep, ecs::FWorld>(
      Payload.World, Payload.Steps,
      [](const ecs::FWorld &Acc, const FEcsDomainProjectionStep &Step) {
        return ProjectDomainStep(Acc, Step);
      });
}

ecs::FWorld ProjectComponentStep(const ecs::FWorld &World,
                                 const FEcsComponentProjectionStep &Step) {
  return WithComponent({World, Step.Entity, Step.Type, Step.Value});
}

/**
 * @brief Applies component projection steps to a world through the ECS fold.
 * @signature ecs::FWorld WithComponentSteps(const FEcsComponentStepsProjectionPayload &Payload)
 *
 * User Story: As feature projection code, I need reusable component batch
 * application that keeps reducers and adapters small.
 */
ecs::FWorld
WithComponentSteps(const FEcsComponentStepsProjectionPayload &Payload) {
  return ecs::foldArray<FEcsComponentProjectionStep, ecs::FWorld>(
      Payload.World, Payload.Steps,
      [](const ecs::FWorld &Acc, const FEcsComponentProjectionStep &Step) {
        return ProjectComponentStep(Acc, Step);
      });
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
  return ecs::mapComponentValues<FLevelLocalPoint>(
      Points, [](const FLevelLocalPoint &Point) {
        return LocalPointValue(Point);
      });
}

TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values) {
  return ecs::mapComponentValues<FString>(
      Values, [](const FString &Value) { return ecs::textValue(Value); });
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
