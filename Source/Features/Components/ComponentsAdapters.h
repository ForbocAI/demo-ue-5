#pragma once

#include "Core/ecs.hpp"

#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace ComponentsAdapters {

struct FEcsDomainProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  TArray<FString> Segments;
};

struct FEcsComponentProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  ecs::FComponentValue Value;
};

struct FEcsTextProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  FString Value;
};

struct FEcsBoolProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  bool Value = false;
};

struct FEcsIntProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  int64 Value = 0;
};

struct FEcsFloatProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  float Value = 0.0f;
};

struct FEcsVec3ProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  FVector Value = FVector::ZeroVector;
};

struct FEcsListProjectionRequest {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  TArray<ecs::FComponentValue> Value;
};

struct FEcsResourceProjectionRequest {
  ecs::FWorld World;
  ecs::ResourceName Name;
  ecs::FComponentValue Value;
};

ecs::DomainPathKey DomainKey(const TArray<FString> &Segments);

ecs::FWorld WithDomain(const FEcsDomainProjectionRequest &Request);
ecs::FWorld WithDomain(ecs::FWorld World, const ecs::EntityKey &Entity,
                       const TArray<FString> &Segments);
ecs::FWorld WithComponent(const FEcsComponentProjectionRequest &Request);
ecs::FWorld WithComponent(ecs::FWorld World, const ecs::EntityKey &Entity,
                          const ecs::ComponentType &Type,
                          const ecs::FComponentValue &Value);
ecs::FWorld WithText(const FEcsTextProjectionRequest &Request);
ecs::FWorld WithText(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, const FString &Value);
ecs::FWorld WithBool(const FEcsBoolProjectionRequest &Request);
ecs::FWorld WithBool(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, bool Value);
ecs::FWorld WithInt(const FEcsIntProjectionRequest &Request);
ecs::FWorld WithInt(ecs::FWorld World, const ecs::EntityKey &Entity,
                    const ecs::ComponentType &Type, int64 Value);
ecs::FWorld WithFloat(const FEcsFloatProjectionRequest &Request);
ecs::FWorld WithFloat(ecs::FWorld World, const ecs::EntityKey &Entity,
                      const ecs::ComponentType &Type, float Value);
ecs::FWorld WithVec3(const FEcsVec3ProjectionRequest &Request);
ecs::FWorld WithVec3(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type, const FVector &Value);
ecs::FWorld WithList(const FEcsListProjectionRequest &Request);
ecs::FWorld WithList(ecs::FWorld World, const ecs::EntityKey &Entity,
                     const ecs::ComponentType &Type,
                     const TArray<ecs::FComponentValue> &Value);
ecs::FWorld WithResource(const FEcsResourceProjectionRequest &Request);

ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point);
ecs::FComponentValue RotationValue(const FRotator &Rotation);
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points, int32 Index = 0,
               TArray<ecs::FComponentValue> Acc =
                   TArray<ecs::FComponentValue>());
TArray<ecs::FComponentValue>
StringList(const TArray<FString> &Values, int32 Index = 0,
           TArray<ecs::FComponentValue> Acc =
               TArray<ecs::FComponentValue>());

template <typename T>
struct TAppendValueRequest {
  TArray<T> Values;
  T Value;
};

template <typename T>
TAppendValueRequest<T> CreateAppendValueRequest(TArray<T> Values,
                                                const T &Value) {
  return {Values, Value};
}

template <typename T>
TArray<T> AppendValue(const TAppendValueRequest<T> &Request) {
  TArray<T> Values = Request.Values;
  Values.Add(Request.Value);
  return Values;
}

template <typename Item, typename Projector>
struct TProjectManyRequest {
  ecs::FWorld World;
  const TArray<Item> *Items = nullptr;
  Projector Project;
  int32 Index = 0;
};

template <typename Item, typename Projector>
TProjectManyRequest<Item, Projector>
CreateProjectManyRequest(ecs::FWorld World, const TArray<Item> &Items,
                         Projector Project, int32 Index = 0) {
  return {World, &Items, Project, Index};
}

template <typename Item, typename Projector>
ecs::FWorld ProjectMany(const TProjectManyRequest<Item, Projector> &Request) {
  return !Request.Items || Request.Index >= Request.Items->Num()
             ? Request.World
             : ProjectMany(TProjectManyRequest<Item, Projector>{
                   Request.Project(Request.World,
                                   (*Request.Items)[Request.Index]),
                   Request.Items, Request.Project, Request.Index + 1});
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
