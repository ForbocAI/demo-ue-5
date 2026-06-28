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

struct FEcsDomainProjectionStep {
  ecs::EntityKey Entity;
  TArray<FString> Segments;
};

struct FEcsComponentProjectionStep {
  ecs::EntityKey Entity;
  ecs::ComponentType Type;
  ecs::FComponentValue Value;
};

struct FEcsDomainStepsProjectionRequest {
  ecs::FWorld World;
  TArray<FEcsDomainProjectionStep> Steps;
  int32 Index = 0;
};

struct FEcsComponentStepsProjectionRequest {
  ecs::FWorld World;
  TArray<FEcsComponentProjectionStep> Steps;
  int32 Index = 0;
};

ecs::DomainPathKey DomainKey(const TArray<FString> &Segments);

ecs::FWorld WithDomain(const FEcsDomainProjectionRequest &Request);
ecs::FWorld WithComponent(const FEcsComponentProjectionRequest &Request);
ecs::FWorld WithText(const FEcsTextProjectionRequest &Request);
ecs::FWorld WithBool(const FEcsBoolProjectionRequest &Request);
ecs::FWorld WithInt(const FEcsIntProjectionRequest &Request);
ecs::FWorld WithFloat(const FEcsFloatProjectionRequest &Request);
ecs::FWorld WithVec3(const FEcsVec3ProjectionRequest &Request);
ecs::FWorld WithList(const FEcsListProjectionRequest &Request);
ecs::FWorld WithResource(const FEcsResourceProjectionRequest &Request);
ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionRequest &Request);
ecs::FWorld
WithComponentSteps(const FEcsComponentStepsProjectionRequest &Request);

ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point);
ecs::FComponentValue RotationValue(const FRotator &Rotation);
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points);
TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values);

template <typename T>
struct TAppendValueRequest {
  TArray<T> Values;
  T Value;
};

template <typename T>
TArray<T> AppendValue(const TAppendValueRequest<T> &Request) {
  TArray<T> Values = Request.Values;
  Values.Add(Request.Value);
  return Values;
}

template <typename Item> struct TMapComponentValuesRequest {
  const TArray<Item> &Items;
  std::function<ecs::FComponentValue(const Item &)> Project;
  int32 Index;
  TArray<ecs::FComponentValue> Acc;
};

/**
 * @brief Maps one array into ECS component values through a recursive unary payload.
 * @signature template <typename Item> TArray<ecs::FComponentValue> MapComponentValues(const TMapComponentValuesRequest<Item> &Request)
 *
 * User Story: As projection code, I need one neutral mapper for strategic
 * goals, interaction candidates, string lists, and local-point lists.
 */
template <typename Item>
TArray<ecs::FComponentValue>
MapComponentValues(const TMapComponentValuesRequest<Item> &Request) {
  return Request.Index >= Request.Items.Num()
             ? Request.Acc
             : MapComponentValues(TMapComponentValuesRequest<Item>{
                   Request.Items, Request.Project, Request.Index + 1,
                   AppendValue(TAppendValueRequest<ecs::FComponentValue>{
                       Request.Acc,
                       Request.Project(Request.Items[Request.Index])})});
}

template <typename Item> struct TEcsRowProjectionPayload {
  ecs::FWorld World;
  const Item &ItemValue;
};

template <typename Item> struct TProjectRowsRequest {
  ecs::FWorld World;
  const TArray<Item> &Items;
  std::function<ecs::FWorld(const TEcsRowProjectionPayload<Item> &)> Project;
  int32 Index;
};

/**
 * @brief Recursively projects selected rows into an ECS world.
 * @signature template <typename Item> ecs::FWorld ProjectRows(const TProjectRowsRequest<Item> &Request)
 *
 * User Story: As systems projection code, I need RTK selector results to
 * become ECS components while preserving unary payload semantics.
 */
template <typename Item>
ecs::FWorld ProjectRows(const TProjectRowsRequest<Item> &Request) {
  return Request.Index >= Request.Items.Num()
             ? Request.World
             : ProjectRows(TProjectRowsRequest<Item>{
                   Request.Project(TEcsRowProjectionPayload<Item>{
                       Request.World, Request.Items[Request.Index]}),
                   Request.Items, Request.Project, Request.Index + 1});
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
