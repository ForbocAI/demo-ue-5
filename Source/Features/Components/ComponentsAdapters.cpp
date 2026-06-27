#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace ComponentsAdapters {

ecs::DomainPathKey DomainKey(const TArray<FString> &Segments) {
  return ecs::domainPathKey(ecs::createDomainPath(Segments));
}

ecs::FWorld WithDomain(const FEcsDomainProjectionRequest &Request) {
  return ecs::setEntityDomain(
      {Request.World, Request.Entity, DomainKey(Request.Segments)});
}

ecs::FWorld WithDomain(ecs::FWorld World, const ecs::EntityKey &Entity,
                       const TArray<FString> &Segments) {
  return WithDomain({World, Entity, Segments});
}

ecs::FWorld WithComponent(const FEcsComponentProjectionRequest &Request) {
  return ecs::setComponent(
      {Request.World, Request.Entity, Request.Type, Request.Value});
}

ecs::FWorld WithComponent(ecs::FWorld World, const ecs::EntityKey &Entity,
                          const ecs::ComponentType &Type,
                          const ecs::FComponentValue &Value) {
  return WithComponent({World, Entity, Type, Value});
}

ecs::FWorld WithText(const FEcsTextProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::textValue(Request.Value)});
}

ecs::FWorld WithText(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, const FString &Value) {
  return WithText({World, Entity, Type, Value});
}

ecs::FWorld WithBool(const FEcsBoolProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::boolValue(Request.Value)});
}

ecs::FWorld WithBool(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, bool Value) {
  return WithBool({World, Entity, Type, Value});
}

ecs::FWorld WithInt(const FEcsIntProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::intValue(Request.Value)});
}

ecs::FWorld WithInt(ecs::FWorld World, const ecs::EntityKey &Entity,
                    const ecs::ComponentType &Type, int64 Value) {
  return WithInt({World, Entity, Type, Value});
}

ecs::FWorld WithFloat(const FEcsFloatProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::floatValue(Request.Value)});
}

ecs::FWorld WithFloat(ecs::FWorld World, const ecs::EntityKey &Entity,
                      const ecs::ComponentType &Type, float Value) {
  return WithFloat({World, Entity, Type, Value});
}

ecs::FWorld WithVec3(const FEcsVec3ProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::vec3Value(Request.Value)});
}

ecs::FWorld WithVec3(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, const FVector &Value) {
  return WithVec3({World, Entity, Type, Value});
}

ecs::FWorld WithList(const FEcsListProjectionRequest &Request) {
  return WithComponent(
      {Request.World, Request.Entity, Request.Type,
       ecs::listValue(Request.Value)});
}

ecs::FWorld WithList(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type,
                     const TArray<ecs::FComponentValue> &Value) {
  return WithList({World, Entity, Type, Value});
}

ecs::FWorld WithResource(const FEcsResourceProjectionRequest &Request) {
  return ecs::setResource({Request.World, Request.Name, Request.Value});
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
LocalPointList(const TArray<FLevelLocalPoint> &Points, int32 Index,
               TArray<ecs::FComponentValue> Acc) {
  return Index >= Points.Num()
             ? Acc
             : LocalPointList(Points, Index + 1,
                              AppendValue(CreateAppendValueRequest(
                                  Acc, LocalPointValue(Points[Index]))));
}

TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values,
                                        int32 Index,
                                        TArray<ecs::FComponentValue> Acc) {
  return Index >= Values.Num()
             ? Acc
             : StringList(Values, Index + 1,
                          AppendValue(CreateAppendValueRequest(
                              Acc, ecs::textValue(Values[Index]))));
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
