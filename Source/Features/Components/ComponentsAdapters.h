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

struct FEcsDomainStepsProjectionPayload {
  ecs::FWorld World;
  TArray<FEcsDomainProjectionStep> Steps;
};

struct FEcsComponentStepsProjectionPayload {
  ecs::FWorld World;
  TArray<FEcsComponentProjectionStep> Steps;
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
ecs::FWorld WithDomainSteps(const FEcsDomainStepsProjectionPayload &Payload);
ecs::FWorld
WithComponentSteps(const FEcsComponentStepsProjectionPayload &Payload);

ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point);
ecs::FComponentValue RotationValue(const FRotator &Rotation);
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points);
TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values);

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
