#pragma once
#ifndef DEMO_PROJECT_CORE_ECS_HPP
#define DEMO_PROJECT_CORE_ECS_HPP

#include "CoreMinimal.h"
#include "Core/functional_core.hpp"

/**
 * @file ecs.hpp
 * @brief Demo-project ECS primitives that depend on FP only and never on RTK.
 *
 * User Stories:
 * - As a gameplay feature author, I need neutral Components / Entities /
 *   Systems taxonomy so Unreal features can share ECS vocabulary with
 *   Rust and GDScript game projects.
 * - As a reducer author, I need ECS worlds to be plain value structs so project
 *   files can compose ECS transitions with RTK state without ECS importing RTK.
 * - As a maintainer, I need this demo header outside the SDK core because the
 *   SDK should not own game-project ECS primitives.
 *
 * Functional-core cookbook boundary:
 * - ECS helpers are pure value transforms over request structs.
 * - RTK slices/actions/selectors own store semantics and unidirectional data
 *   flow.
 * - functional_core.hpp is used for composition, Maybe/Either, and lazy values;
 *   it must not model a replacement action, reducer, selector, or store layer.
 */
namespace ecs {

typedef FString EntityKey;
typedef FString ComponentType;
typedef FString Tag;
typedef FString ResourceName;
typedef FString EventType;
typedef FString DomainPathKey;
typedef FString SystemName;

struct FEntityId {
  int64 Index;
  int32 Generation;
};

/**
 * @brief Creates a generation-counted entity id.
 * @signature inline FEntityId createEntityId(int64 Index, int32 Generation)
 *
 * User Story: As an ECS caller, I need stable id values so recycled entity slots
 * do not accidentally resolve stale handles.
 */
inline FEntityId createEntityId(int64 Index, int32 Generation) {
  FEntityId Id;
  Id.Index = Index;
  Id.Generation = Generation;
  return Id;
}

/**
 * @brief Converts an entity id into the shared string key format.
 * @signature inline EntityKey entityKey(const FEntityId &Id)
 *
 * User Story: As component storage, I need a deterministic key for TMap-based
 * component tables.
 */
inline EntityKey entityKey(const FEntityId &Id) {
  return FString::Printf(TEXT("%lld:%d"), Id.Index, Id.Generation);
}

inline bool operator==(const FEntityId &Left, const FEntityId &Right) {
  return Left.Index == Right.Index && Left.Generation == Right.Generation;
}

inline bool operator!=(const FEntityId &Left, const FEntityId &Right) {
  return !(Left == Right);
}

struct FAllocator {
  int64 NextIndex;
  TArray<FEntityId> Freed;
  TMap<int64, int32> Generations;
};

struct FAllocatedEntity {
  FAllocator Allocator;
  FEntityId Entity;
};

/**
 * @brief Creates an empty entity allocator.
 * @signature inline FAllocator createAllocator()
 *
 * User Story: As a world author, I need isolated allocator state for each world
 * so tests, levels, and runtime sessions do not share entity id counters.
 */
inline FAllocator createAllocator() {
  FAllocator Allocator;
  Allocator.NextIndex = 0;
  return Allocator;
}

inline bool operator==(const FAllocator &Left, const FAllocator &Right) {
  return Left.NextIndex == Right.NextIndex && Left.Freed == Right.Freed &&
         Left.Generations.OrderIndependentCompareEqual(Right.Generations);
}

inline bool operator!=(const FAllocator &Left, const FAllocator &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAllocatedEntity &Left,
                       const FAllocatedEntity &Right) {
  return Left.Allocator == Right.Allocator && Left.Entity == Right.Entity;
}

inline bool operator!=(const FAllocatedEntity &Left,
                       const FAllocatedEntity &Right) {
  return !(Left == Right);
}

/**
 * @brief Allocates a fresh or recycled entity id.
 * @signature inline FAllocatedEntity allocateEntity(FAllocator Allocator)
 *
 * User Story: As a spawn mechanic, I need entity allocation to return new
 * allocator state plus the allocated id so state transitions stay value based.
 */
inline FAllocatedEntity allocateEntity(FAllocator Allocator) {
  FAllocatedEntity Result;
  if (Allocator.Freed.Num() > 0) {
    Result.Entity = Allocator.Freed.Last();
    Allocator.Freed.Pop();
  } else {
    Result.Entity = createEntityId(Allocator.NextIndex, 0);
    Allocator.Generations.Add(Allocator.NextIndex, 0);
    Allocator.NextIndex += 1;
  }
  Result.Allocator = Allocator;
  return Result;
}

/**
 * @brief Frees an entity id and increments its generation for safe reuse.
 * @signature inline FAllocator freeEntity(FAllocator Allocator, const FEntityId &Id)
 *
 * User Story: As a despawn mechanic, I need old handles to stop resolving after
 * their slot has been released.
 */
inline FAllocator freeEntity(FAllocator Allocator, const FEntityId &Id) {
  const int32 *Current = Allocator.Generations.Find(Id.Index);
  if (Current && *Current == Id.Generation) {
    const int32 NextGeneration = *Current + 1;
    Allocator.Generations.Add(Id.Index, NextGeneration);
    Allocator.Freed.Add(createEntityId(Id.Index, NextGeneration));
  }
  return Allocator;
}

/**
 * @brief Checks whether an entity id is still alive in the allocator.
 * @signature inline bool isAlive(const FAllocator &Allocator, const FEntityId &Id)
 *
 * User Story: As a query system, I need to reject stale handles before reading
 * component data.
 */
inline bool isAlive(const FAllocator &Allocator, const FEntityId &Id) {
  const int32 *Current = Allocator.Generations.Find(Id.Index);
  return Current && *Current == Id.Generation;
}

enum class EComponentValueKind {
  None,
  Bool,
  Int,
  Float,
  Text,
  Vec2,
  Vec3,
  Map,
  List
};

struct FComponentValue {
  EComponentValueKind Kind;
  bool BoolValue;
  int64 IntValue;
  float FloatValue;
  FString TextValue;
  FVector2D Vec2Value;
  FVector Vec3Value;
  TMap<FString, TSharedPtr<FComponentValue>> MapValue;
  TArray<TSharedPtr<FComponentValue>> ListValue;
};

inline FComponentValue noneValue() {
  FComponentValue Value;
  Value.Kind = EComponentValueKind::None;
  Value.BoolValue = false;
  Value.IntValue = 0;
  Value.FloatValue = 0.0F;
  return Value;
}

inline FComponentValue boolValue(bool RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Bool;
  Value.BoolValue = RawValue;
  return Value;
}

inline FComponentValue intValue(int64 RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Int;
  Value.IntValue = RawValue;
  return Value;
}

inline FComponentValue floatValue(float RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Float;
  Value.FloatValue = RawValue;
  return Value;
}

inline FComponentValue textValue(const FString &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Text;
  Value.TextValue = RawValue;
  return Value;
}

inline FComponentValue vec3Value(const FVector &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Vec3;
  Value.Vec3Value = RawValue;
  return Value;
}

inline FComponentValue vec2Value(const FVector2D &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Vec2;
  Value.Vec2Value = RawValue;
  return Value;
}

inline FComponentValue
mapValue(const TMap<FString, FComponentValue> &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::Map;
  for (const TPair<FString, FComponentValue> &Entry : RawValue) {
    Value.MapValue.Add(Entry.Key, MakeShared<FComponentValue>(Entry.Value));
  }
  return Value;
}

inline FComponentValue listValue(const TArray<FComponentValue> &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::List;
  for (const FComponentValue &Entry : RawValue) {
    Value.ListValue.Add(MakeShared<FComponentValue>(Entry));
  }
  return Value;
}

inline bool componentValuePtrEquals(const TSharedPtr<FComponentValue> &Left,
                                    const TSharedPtr<FComponentValue> &Right);

inline bool componentMapEquals(
    const TMap<FString, TSharedPtr<FComponentValue>> &Left,
    const TMap<FString, TSharedPtr<FComponentValue>> &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<FString, TSharedPtr<FComponentValue>> &Entry : Left) {
    const TSharedPtr<FComponentValue> *RightValue = Right.Find(Entry.Key);
    if (!RightValue || !componentValuePtrEquals(Entry.Value, *RightValue)) {
      return false;
    }
  }
  return true;
}

inline bool componentListEquals(
    const TArray<TSharedPtr<FComponentValue>> &Left,
    const TArray<TSharedPtr<FComponentValue>> &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (int32 Index = 0; Index < Left.Num(); ++Index) {
    if (!componentValuePtrEquals(Left[Index], Right[Index])) {
      return false;
    }
  }
  return true;
}

inline bool operator==(const FComponentValue &Left,
                       const FComponentValue &Right) {
  return Left.Kind == Right.Kind && Left.BoolValue == Right.BoolValue &&
         Left.IntValue == Right.IntValue &&
         FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue) &&
         Left.TextValue == Right.TextValue &&
         Left.Vec2Value == Right.Vec2Value &&
         Left.Vec3Value == Right.Vec3Value &&
         componentMapEquals(Left.MapValue, Right.MapValue) &&
         componentListEquals(Left.ListValue, Right.ListValue);
}

inline bool operator!=(const FComponentValue &Left,
                       const FComponentValue &Right) {
  return !(Left == Right);
}

inline bool componentValuePtrEquals(const TSharedPtr<FComponentValue> &Left,
                                    const TSharedPtr<FComponentValue> &Right) {
  return (!Left.IsValid() && !Right.IsValid()) ||
         (Left.IsValid() && Right.IsValid() && *Left == *Right);
}

typedef TMap<EntityKey, FComponentValue> ComponentTable;
typedef TMap<ComponentType, ComponentTable> ComponentStore;
typedef TMap<EntityKey, TArray<Tag>> TagStore;
typedef TMap<ResourceName, FComponentValue> ResourceStore;
typedef TMap<EventType, TArray<FComponentValue>> EventQueue;

enum class EDomainKind {
  Component,
  Entity,
  System,
  Ui,
  Service,
  Session,
  Data,
  Input,
  Audio,
  Rendering,
  Physics,
  Ai,
  Narrative,
  Procgen,
  Unknown
};

struct FDomainPath {
  TArray<FString> Segments;
};

struct FFieldSchema {
  FString Name;
  FString Kind;
  bool bRequired;
  FString Description;
};

struct FComponentSchema {
  ComponentType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  TArray<Tag> Tags;
  FString Description;
};

struct FCapabilitySpec {
  FString Name;
  DomainPathKey Domain;
  TArray<ComponentType> Reads;
  TArray<ComponentType> Writes;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FSystemSpec {
  SystemName Name;
  DomainPathKey Domain;
  TArray<ComponentType> RequiredComponents;
  TArray<Tag> RequiredTags;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FResourceSpec {
  ResourceName Name;
  DomainPathKey Domain;
  FString Kind;
  FString Description;
};

struct FEventSpec {
  EventType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  FString Description;
};

struct FDomainNode {
  FDomainPath Path;
  EDomainKind Kind;
  TArray<DomainPathKey> Children;
  TArray<ComponentType> ComponentSchemas;
  TArray<FString> Capabilities;
  TArray<SystemName> Systems;
  TArray<ResourceName> Resources;
  TArray<EventType> Events;
};

struct FDomainRegistry {
  TMap<DomainPathKey, FDomainNode> Nodes;
  TMap<ComponentType, FComponentSchema> ComponentSchemas;
  TMap<FString, FCapabilitySpec> Capabilities;
  TMap<SystemName, FSystemSpec> Systems;
  TMap<ResourceName, FResourceSpec> ResourceSpecs;
  TMap<EventType, FEventSpec> EventSpecs;
};

struct FCreateDomainNodeRequest {
  FDomainPath Path;
  EDomainKind Kind = EDomainKind::Unknown;
};

struct FPushUniqueStringRequest {
  TArray<FString> Values;
  FString Value;
};

struct FRegisterDomainRequest {
  FDomainRegistry Registry;
  FDomainNode Node;
};

struct FRegisterDomainPathRequest {
  FDomainRegistry Registry;
  TArray<FString> Segments;
  EDomainKind Kind = EDomainKind::Unknown;
};

struct FDomainPathRegistration {
  TArray<FString> Segments;
  EDomainKind Kind = EDomainKind::Unknown;
};

struct FRegisterDomainPathsRequest {
  FDomainRegistry Registry;
  TArray<FDomainPathRegistration> Registrations;
  int32 Index = 0;
};

struct FRegisterComponentSchemaRequest {
  FDomainRegistry Registry;
  FComponentSchema Schema;
};

struct FRegisterCapabilitySpecRequest {
  FDomainRegistry Registry;
  FCapabilitySpec Spec;
};

struct FRegisterSystemSpecRequest {
  FDomainRegistry Registry;
  FSystemSpec Spec;
};

struct FRegisterResourceSpecRequest {
  FDomainRegistry Registry;
  FResourceSpec Spec;
};

struct FRegisterEventSpecRequest {
  FDomainRegistry Registry;
  FEventSpec Spec;
};

/**
 * @brief Creates a neutral ECS domain path from ordered path segments.
 * @signature inline FDomainPath createDomainPath(const TArray<FString> &Segments)
 *
 * User Story: As a feature author, I need stable domain paths so higher
 * gameplay domains can import downward into shared ECS primitives.
 */
inline FDomainPath createDomainPath(const TArray<FString> &Segments) {
  FDomainPath Path;
  Path.Segments = Segments;
  return Path;
}

inline bool operator==(const FDomainPath &Left, const FDomainPath &Right) {
  return Left.Segments == Right.Segments;
}

inline bool operator!=(const FDomainPath &Left, const FDomainPath &Right) {
  return !(Left == Right);
}

/**
 * @brief Converts a domain path into its registry key.
 * @signature inline DomainPathKey domainPathKey(const FDomainPath &Path)
 */
inline DomainPathKey domainPathKey(const FDomainPath &Path) {
  return FString::Join(Path.Segments, TEXT("/"));
}

/**
 * @brief Creates a registry node describing a domain/subdomain boundary.
 * @signature inline FDomainNode createDomainNode(const FCreateDomainNodeRequest &Request)
 *
 * User Story: As a feature maintainer, I need domain-node construction to use
 * one request payload so ECS taxonomy helpers stay unary and composable.
 */
inline FDomainNode createDomainNode(const FCreateDomainNodeRequest &Request) {
  FDomainNode Node;
  Node.Path = Request.Path;
  Node.Kind = Request.Kind;
  return Node;
}

/**
 * @brief Creates a field schema for component/resource/event contracts.
 */
inline FFieldSchema createFieldSchema(const FString &Name, const FString &Kind,
                                      bool bRequired,
                                      const FString &Description) {
  FFieldSchema Schema;
  Schema.Name = Name;
  Schema.Kind = Kind;
  Schema.bRequired = bRequired;
  Schema.Description = Description;
  return Schema;
}

/**
 * @brief Creates an ECS component schema owned by one domain path.
 */
inline FComponentSchema
createComponentSchema(const ComponentType &Type, const DomainPathKey &Domain,
                      const TArray<FFieldSchema> &Fields,
                      const TArray<Tag> &Tags, const FString &Description) {
  FComponentSchema Schema;
  Schema.Type = Type;
  Schema.Domain = Domain;
  Schema.Fields = Fields;
  Schema.Tags = Tags;
  Schema.Description = Description;
  return Schema;
}

/**
 * @brief Creates a capability contract describing component/resource access.
 */
inline FCapabilitySpec
createCapabilitySpec(const FString &Name, const DomainPathKey &Domain,
                     const TArray<ComponentType> &Reads,
                     const TArray<ComponentType> &Writes,
                     const TArray<ResourceName> &Resources,
                     const TArray<EventType> &Emits) {
  FCapabilitySpec Spec;
  Spec.Name = Name;
  Spec.Domain = Domain;
  Spec.Reads = Reads;
  Spec.Writes = Writes;
  Spec.Resources = Resources;
  Spec.Emits = Emits;
  return Spec;
}

/**
 * @brief Creates a system contract for registry inspection and validation.
 */
inline FSystemSpec createSystemSpec(const SystemName &Name,
                                    const DomainPathKey &Domain,
                                    const TArray<ComponentType> &Components,
                                    const TArray<Tag> &Tags,
                                    const TArray<ResourceName> &Resources,
                                    const TArray<EventType> &Emits) {
  FSystemSpec Spec;
  Spec.Name = Name;
  Spec.Domain = Domain;
  Spec.RequiredComponents = Components;
  Spec.RequiredTags = Tags;
  Spec.Resources = Resources;
  Spec.Emits = Emits;
  return Spec;
}

/**
 * @brief Creates a named ECS resource contract owned by one domain.
 */
inline FResourceSpec createResourceSpec(const ResourceName &Name,
                                        const DomainPathKey &Domain,
                                        const FString &Kind,
                                        const FString &Description) {
  FResourceSpec Spec;
  Spec.Name = Name;
  Spec.Domain = Domain;
  Spec.Kind = Kind;
  Spec.Description = Description;
  return Spec;
}

/**
 * @brief Creates a typed event contract owned by one domain.
 */
inline FEventSpec createEventSpec(const EventType &Type,
                                  const DomainPathKey &Domain,
                                  const TArray<FFieldSchema> &Fields,
                                  const FString &Description) {
  FEventSpec Spec;
  Spec.Type = Type;
  Spec.Domain = Domain;
  Spec.Fields = Fields;
  Spec.Description = Description;
  return Spec;
}

inline bool operator==(const FFieldSchema &Left, const FFieldSchema &Right) {
  return Left.Name == Right.Name && Left.Kind == Right.Kind &&
         Left.bRequired == Right.bRequired &&
         Left.Description == Right.Description;
}

inline bool operator!=(const FFieldSchema &Left, const FFieldSchema &Right) {
  return !(Left == Right);
}

inline bool operator==(const FComponentSchema &Left,
                       const FComponentSchema &Right) {
  return Left.Type == Right.Type && Left.Domain == Right.Domain &&
         Left.Fields == Right.Fields && Left.Tags == Right.Tags &&
         Left.Description == Right.Description;
}

inline bool operator!=(const FComponentSchema &Left,
                       const FComponentSchema &Right) {
  return !(Left == Right);
}

inline bool operator==(const FCapabilitySpec &Left,
                       const FCapabilitySpec &Right) {
  return Left.Name == Right.Name && Left.Domain == Right.Domain &&
         Left.Reads == Right.Reads && Left.Writes == Right.Writes &&
         Left.Resources == Right.Resources && Left.Emits == Right.Emits;
}

inline bool operator!=(const FCapabilitySpec &Left,
                       const FCapabilitySpec &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSystemSpec &Left, const FSystemSpec &Right) {
  return Left.Name == Right.Name && Left.Domain == Right.Domain &&
         Left.RequiredComponents == Right.RequiredComponents &&
         Left.RequiredTags == Right.RequiredTags &&
         Left.Resources == Right.Resources && Left.Emits == Right.Emits;
}

inline bool operator!=(const FSystemSpec &Left, const FSystemSpec &Right) {
  return !(Left == Right);
}

inline bool operator==(const FResourceSpec &Left,
                       const FResourceSpec &Right) {
  return Left.Name == Right.Name && Left.Domain == Right.Domain &&
         Left.Kind == Right.Kind && Left.Description == Right.Description;
}

inline bool operator!=(const FResourceSpec &Left,
                       const FResourceSpec &Right) {
  return !(Left == Right);
}

inline bool operator==(const FEventSpec &Left, const FEventSpec &Right) {
  return Left.Type == Right.Type && Left.Domain == Right.Domain &&
         Left.Fields == Right.Fields && Left.Description == Right.Description;
}

inline bool operator!=(const FEventSpec &Left, const FEventSpec &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDomainNode &Left, const FDomainNode &Right) {
  return Left.Path == Right.Path && Left.Kind == Right.Kind &&
         Left.Children == Right.Children &&
         Left.ComponentSchemas == Right.ComponentSchemas &&
         Left.Capabilities == Right.Capabilities &&
         Left.Systems == Right.Systems && Left.Resources == Right.Resources &&
         Left.Events == Right.Events;
}

inline bool operator!=(const FDomainNode &Left, const FDomainNode &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDomainRegistry &Left,
                       const FDomainRegistry &Right) {
  return Left.Nodes.OrderIndependentCompareEqual(Right.Nodes) &&
         Left.ComponentSchemas.OrderIndependentCompareEqual(
             Right.ComponentSchemas) &&
         Left.Capabilities.OrderIndependentCompareEqual(Right.Capabilities) &&
         Left.Systems.OrderIndependentCompareEqual(Right.Systems) &&
         Left.ResourceSpecs.OrderIndependentCompareEqual(Right.ResourceSpecs) &&
         Left.EventSpecs.OrderIndependentCompareEqual(Right.EventSpecs);
}

inline bool operator!=(const FDomainRegistry &Left,
                       const FDomainRegistry &Right) {
  return !(Left == Right);
}

/**
 * @brief Creates an empty ECS domain registry.
 * @signature inline FDomainRegistry createDomainRegistry()
 *
 * User Story: As an ECS feature author, I need a fresh registry value before
 * composing domain registrations into a world.
 */
inline FDomainRegistry createDomainRegistry() { return FDomainRegistry(); }

/**
 * @brief Returns an array with one value present at most once.
 * @signature inline TArray<FString> pushUniqueString(const FPushUniqueStringRequest &Request)
 *
 * User Story: As a registry maintainer, I need set-like index updates to stay
 * value-returning so ECS helpers remain composable with functional-core
 * pipelines.
 */
inline TArray<FString>
pushUniqueString(const FPushUniqueStringRequest &Request) {
  TArray<FString> Values = Request.Values;
  Values.AddUnique(Request.Value);
  return Values;
}

/**
 * @brief Registers one domain node and wires it to its parent when present.
 * @signature inline FDomainRegistry registerDomain(const FRegisterDomainRequest &Request)
 *
 * Architecture: This keeps neutral domain topology below feature slices so
 * feature code can import downward without borrowing sibling-domain helpers.
 */
inline FDomainRegistry registerDomain(const FRegisterDomainRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FDomainNode Node = Request.Node;
  const DomainPathKey Key = domainPathKey(Node.Path);
  Registry.Nodes.Add(Key, Node);
  return Node.Path.Segments.Num() > 1
             ? ([Registry, Node, Key]() mutable {
                 FDomainPath ParentPath = Node.Path;
                 ParentPath.Segments.RemoveAt(ParentPath.Segments.Num() - 1);
                 const DomainPathKey ParentKey = domainPathKey(ParentPath);
                 FDomainNode *Parent = Registry.Nodes.Find(ParentKey);
                 Parent != nullptr
                     ? (Parent->Children =
                            pushUniqueString({Parent->Children, Key}),
                        void())
                     : void();
                 return Registry;
               })()
             : Registry;
}

/**
 * @brief Unary helper for registering a domain from path segments and kind.
 * @signature inline FDomainRegistry registerDomainPath(const FRegisterDomainPathRequest &Request)
 *
 * User Story: As a feature author, I can register ECS subdomains through one
 * payload, matching the unary functional-core cookbook pattern.
 */
inline FDomainRegistry
registerDomainPath(const FRegisterDomainPathRequest &Request) {
  return registerDomain(
      {Request.Registry,
       createDomainNode({createDomainPath(Request.Segments), Request.Kind})});
}

/**
 * @brief Recursively registers a list of domain path declarations.
 * @signature inline FDomainRegistry registerDomainPaths(FRegisterDomainPathsRequest Request)
 *
 * User Story: As a maintainer, I can extend the ECS taxonomy by appending data
 * declarations without spreading imperative registration statements through
 * the registry factory.
 */
inline FDomainRegistry
registerDomainPaths(FRegisterDomainPathsRequest Request) {
  return Request.Index >= Request.Registrations.Num()
             ? Request.Registry
             : ([Request]() mutable {
                 const FDomainPathRegistration Registration =
                     Request.Registrations[Request.Index];
                 Request.Registry =
                     registerDomainPath({Request.Registry,
                                         Registration.Segments,
                                         Registration.Kind});
                 Request.Index = Request.Index + 1;
                 return registerDomainPaths(MoveTemp(Request));
               })();
}

/**
 * @brief Registers one component schema and indexes it under its owning domain.
 * @signature inline FDomainRegistry registerComponentSchema(const FRegisterComponentSchemaRequest &Request)
 *
 * User Story: As a component author, schema registration is a unary payload
 * transition over a registry value.
 */
inline FDomainRegistry
registerComponentSchema(const FRegisterComponentSchemaRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FComponentSchema Schema = Request.Schema;
  Registry.ComponentSchemas.Add(Schema.Type, Schema);
  FDomainNode *Domain = Registry.Nodes.Find(Schema.Domain);
  Domain != nullptr
      ? (Domain->ComponentSchemas =
             pushUniqueString({Domain->ComponentSchemas, Schema.Type}),
         void())
      : void();
  return Registry;
}

/**
 * @brief Registers one capability spec and indexes it under its owning domain.
 * @signature inline FDomainRegistry registerCapabilitySpec(const FRegisterCapabilitySpecRequest &Request)
 *
 * User Story: As a systems author, capability registration stays a value
 * transition that can be composed below RTK slices.
 */
inline FDomainRegistry
registerCapabilitySpec(const FRegisterCapabilitySpecRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FCapabilitySpec Spec = Request.Spec;
  Registry.Capabilities.Add(Spec.Name, Spec);
  FDomainNode *Domain = Registry.Nodes.Find(Spec.Domain);
  Domain != nullptr
      ? (Domain->Capabilities =
             pushUniqueString({Domain->Capabilities, Spec.Name}),
         void())
      : void();
  return Registry;
}

/**
 * @brief Registers one system spec and indexes it under its owning domain.
 * @signature inline FDomainRegistry registerSystemSpec(const FRegisterSystemSpecRequest &Request)
 *
 * User Story: As an ECS systems author, system registration remains a pure
 * registry transform and never owns store semantics.
 */
inline FDomainRegistry
registerSystemSpec(const FRegisterSystemSpecRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FSystemSpec Spec = Request.Spec;
  Registry.Systems.Add(Spec.Name, Spec);
  FDomainNode *Domain = Registry.Nodes.Find(Spec.Domain);
  Domain != nullptr
      ? (Domain->Systems = pushUniqueString({Domain->Systems, Spec.Name}),
         void())
      : void();
  return Registry;
}

/**
 * @brief Registers one resource spec and indexes it under its owning domain.
 * @signature inline FDomainRegistry registerResourceSpec(const FRegisterResourceSpecRequest &Request)
 *
 * User Story: As a resource author, resource registration stays below feature
 * domains and composes as a unary ECS transform.
 */
inline FDomainRegistry
registerResourceSpec(const FRegisterResourceSpecRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FResourceSpec Spec = Request.Spec;
  Registry.ResourceSpecs.Add(Spec.Name, Spec);
  FDomainNode *Domain = Registry.Nodes.Find(Spec.Domain);
  Domain != nullptr
      ? (Domain->Resources = pushUniqueString({Domain->Resources, Spec.Name}),
         void())
      : void();
  return Registry;
}

/**
 * @brief Alias for resource-spec registration used by feature code.
 * @signature inline FDomainRegistry registerResource(const FRegisterResourceSpecRequest &Request)
 *
 * User Story: As feature code, I can use RTK-style explicit names while ECS
 * resources remain plain registry values.
 */
inline FDomainRegistry
registerResource(const FRegisterResourceSpecRequest &Request) {
  return registerResourceSpec(Request);
}

/**
 * @brief Registers one event spec and indexes it under its owning domain.
 * @signature inline FDomainRegistry registerEventSpec(const FRegisterEventSpecRequest &Request)
 *
 * User Story: As an ECS event author, event registration is declarative domain
 * metadata and does not replace RTK actions.
 */
inline FDomainRegistry
registerEventSpec(const FRegisterEventSpecRequest &Request) {
  FDomainRegistry Registry = Request.Registry;
  const FEventSpec Spec = Request.Spec;
  Registry.EventSpecs.Add(Spec.Type, Spec);
  FDomainNode *Domain = Registry.Nodes.Find(Spec.Domain);
  Domain != nullptr
      ? (Domain->Events = pushUniqueString({Domain->Events, Spec.Type}),
         void())
      : void();
  return Registry;
}

/**
 * @brief Alias for event-spec registration used by feature code.
 * @signature inline FDomainRegistry registerEventType(const FRegisterEventSpecRequest &Request)
 *
 * User Story: As feature code, I can declare ECS event metadata without
 * confusing it with dispatched RTK action events.
 */
inline FDomainRegistry
registerEventType(const FRegisterEventSpecRequest &Request) {
  return registerEventSpec(Request);
}

/**
 * @brief Builds the neutral cross-domain taxonomy shared by ECS cores.
 * @signature inline FDomainRegistry createCrossDomainRegistry()
 *
 * User Story: As a feature author, I need Components / Entities / Systems /
 * Ui domains with stable subdomains so project systems can query and inspect
 * data without importing presentational view layers.
 */
inline FDomainRegistry createCrossDomainRegistry() {
  return registerDomainPaths(
      {createDomainRegistry(),
       {
           {{TEXT("Components")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Attributes")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Bots")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Data")}, EDomainKind::Data},
           {{TEXT("Components"), TEXT("Data"), TEXT("Json")},
            EDomainKind::Data},
           {{TEXT("Components"), TEXT("Data"), TEXT("Json"),
             TEXT("Values")},
            EDomainKind::Data},
           {{TEXT("Components"), TEXT("Frame")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Framebuffer")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Geometry")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Glyphs")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Health")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Input")}, EDomainKind::Input},
           {{TEXT("Components"), TEXT("Interaction")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Level")}, EDomainKind::Data},
           {{TEXT("Components"), TEXT("Level"), TEXT("TerrainGeometry")},
            EDomainKind::Data},
           {{TEXT("Components"), TEXT("Lifecycle")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Narrative")},
            EDomainKind::Narrative},
           {{TEXT("Components"), TEXT("Position")},
            EDomainKind::Component},
           {{TEXT("Components"), TEXT("Random")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Rendering")},
            EDomainKind::Rendering},
           {{TEXT("Components"), TEXT("Spatial")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("Stats")}, EDomainKind::Component},
           {{TEXT("Components"), TEXT("UI")}, EDomainKind::Ui},
           {{TEXT("Entities")}, EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Characters")}, EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
             TEXT("Horses")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
             TEXT("Townspeople")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Characters"), TEXT("Player")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Environments")}, EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Environments"), TEXT("Landmarks")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Environments"), TEXT("Nature")},
            EDomainKind::Entity},
           {{TEXT("Entities"), TEXT("Session")}, EDomainKind::Session},
           {{TEXT("Systems")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("AI")}, EDomainKind::Ai},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Core")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Goals")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Horses")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Orchestrator")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Orchestrator"),
             TEXT("Factories")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Pipeline")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Position")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Stats")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Bots"), TEXT("Townspeople")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Capabilities")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Dialogue")}, EDomainKind::Narrative},
           {{TEXT("Systems"), TEXT("Factories")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Input")}, EDomainKind::Input},
           {{TEXT("Systems"), TEXT("Interaction")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Landmarks")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Enums")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Scales")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Blocks")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Labels")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Sections")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("RuntimeLayout"),
             TEXT("Layout")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Level"), TEXT("TerrainGeometry")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Nature")}, EDomainKind::Procgen},
           {{TEXT("Systems"), TEXT("Rendering")}, EDomainKind::Rendering},
           {{TEXT("Systems"), TEXT("Runtime")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Services")}, EDomainKind::Service},
           {{TEXT("Systems"), TEXT("Spawn")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Speech")}, EDomainKind::Narrative},
           {{TEXT("Systems"), TEXT("Terrain")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Transformations")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("UI")}, EDomainKind::Ui},
           {{TEXT("Systems"), TEXT("Utils")}, EDomainKind::Service},
       },
       0});
}

struct FRelationship {
  func::Maybe<EntityKey> Parent;
  TArray<EntityKey> Children;
};

/**
 * @brief Creates an empty parent/children relationship record.
 */
inline FRelationship createRelationship() {
  FRelationship Relationship;
  Relationship.Parent = func::nothing<EntityKey>();
  return Relationship;
}

inline bool operator==(const FRelationship &Left,
                       const FRelationship &Right) {
  return Left.Parent.hasValue == Right.Parent.hasValue &&
         (!Left.Parent.hasValue || Left.Parent.value == Right.Parent.value) &&
         Left.Children == Right.Children;
}

inline bool operator!=(const FRelationship &Left,
                       const FRelationship &Right) {
  return !(Left == Right);
}

struct FWorld {
  FAllocator Allocator;
  ComponentStore Components;
  TagStore Tags;
  ResourceStore Resources;
  EventQueue Events;
  TMap<EntityKey, TArray<DomainPathKey>> EntityDomains;
  TMap<EntityKey, FRelationship> Relationships;
  FDomainRegistry Domains;
  TArray<EntityKey> DirtyEntities;
  int64 Generation;
};

struct FSpawnedEntity {
  FWorld World;
  FEntityId Id;
  EntityKey Entity;
};

struct FSetComponentRequest {
  FWorld World;
  EntityKey Entity;
  ComponentType Type;
  FComponentValue Value;
};

struct FSetTagRequest {
  FWorld World;
  EntityKey Entity;
  Tag TagValue;
};

struct FSetResourceRequest {
  FWorld World;
  ResourceName Name;
  FComponentValue Value;
};

struct FPushEventRequest {
  FWorld World;
  EventType Type;
  FComponentValue Payload;
};

struct FSetEntityDomainRequest {
  FWorld World;
  EntityKey Entity;
  DomainPathKey Domain;
};

inline FWorld setEntityDomain(FWorld World, const EntityKey &Entity,
                              const DomainPathKey &Domain);

/**
 * @brief Creates a fresh ECS world with allocator and cross-domain registry.
 *
 * Architecture: The world is plain value state. RTK reducers may store and
 * replace it, but ECS never owns the runtime store or dispatch semantics.
 */
inline FWorld createWorld() {
  FWorld World;
  World.Allocator = createAllocator();
  World.Domains = createCrossDomainRegistry();
  World.Generation = 0;
  return World;
}

inline bool componentTableEquals(const ComponentTable &Left,
                                 const ComponentTable &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<EntityKey, FComponentValue> &Entry : Left) {
    const FComponentValue *RightValue = Right.Find(Entry.Key);
    if (!RightValue || *RightValue != Entry.Value) {
      return false;
    }
  }
  return true;
}

inline bool componentStoreEquals(const ComponentStore &Left,
                                 const ComponentStore &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<ComponentType, ComponentTable> &Entry : Left) {
    const ComponentTable *RightValue = Right.Find(Entry.Key);
    if (!RightValue || !componentTableEquals(Entry.Value, *RightValue)) {
      return false;
    }
  }
  return true;
}

template <typename Value>
inline bool arrayMapEquals(const TMap<FString, TArray<Value>> &Left,
                           const TMap<FString, TArray<Value>> &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<FString, TArray<Value>> &Entry : Left) {
    const TArray<Value> *RightValue = Right.Find(Entry.Key);
    if (!RightValue || *RightValue != Entry.Value) {
      return false;
    }
  }
  return true;
}

inline bool resourceStoreEquals(const ResourceStore &Left,
                                const ResourceStore &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<ResourceName, FComponentValue> &Entry : Left) {
    const FComponentValue *RightValue = Right.Find(Entry.Key);
    if (!RightValue || *RightValue != Entry.Value) {
      return false;
    }
  }
  return true;
}

inline bool relationshipStoreEquals(
    const TMap<EntityKey, FRelationship> &Left,
    const TMap<EntityKey, FRelationship> &Right) {
  if (Left.Num() != Right.Num()) {
    return false;
  }
  for (const TPair<EntityKey, FRelationship> &Entry : Left) {
    const FRelationship *RightValue = Right.Find(Entry.Key);
    if (!RightValue || *RightValue != Entry.Value) {
      return false;
    }
  }
  return true;
}

inline bool operator==(const FWorld &Left, const FWorld &Right) {
  return Left.Allocator == Right.Allocator &&
         componentStoreEquals(Left.Components, Right.Components) &&
         arrayMapEquals(Left.Tags, Right.Tags) &&
         resourceStoreEquals(Left.Resources, Right.Resources) &&
         arrayMapEquals(Left.Events, Right.Events) &&
         arrayMapEquals(Left.EntityDomains, Right.EntityDomains) &&
         relationshipStoreEquals(Left.Relationships, Right.Relationships) &&
         Left.Domains == Right.Domains &&
         Left.DirtyEntities == Right.DirtyEntities &&
         Left.Generation == Right.Generation;
}

inline bool operator!=(const FWorld &Left, const FWorld &Right) {
  return !(Left == Right);
}

/**
 * @brief Marks an entity dirty and increments the world generation.
 */
inline FWorld markDirty(FWorld World, const EntityKey &Entity) {
  World.DirtyEntities.AddUnique(Entity);
  World.Generation += 1;
  return World;
}

/**
 * @brief Allocates and marks one entity in the supplied world.
 *
 * @return FSpawnedEntity carrying the next world, id, and entity key.
 */
inline FSpawnedEntity spawnEntity(FWorld World) {
  const FAllocatedEntity Allocated = allocateEntity(World.Allocator);
  World.Allocator = Allocated.Allocator;
  const EntityKey Entity = entityKey(Allocated.Entity);
  FSpawnedEntity Spawned;
  Spawned.World = markDirty(World, Entity);
  Spawned.Id = Allocated.Entity;
  Spawned.Entity = Entity;
  return Spawned;
}

/**
 * @brief Allocates one entity and associates it with an ECS domain.
 */
inline FSpawnedEntity spawnEntityInDomain(FWorld World,
                                          const DomainPathKey &Domain) {
  const FSpawnedEntity Spawned = spawnEntity(World);
  FSpawnedEntity WithDomain = Spawned;
  WithDomain.World = setEntityDomain(Spawned.World, Spawned.Entity, Domain);
  return WithDomain;
}

/**
 * @brief Removes an entity's component, tag, domain, and relationship data.
 */
inline FWorld removeEntity(FWorld World, const EntityKey &Entity) {
  for (TPair<ComponentType, ComponentTable> &Entry : World.Components) {
    Entry.Value.Remove(Entity);
  }
  World.Tags.Remove(Entity);
  World.EntityDomains.Remove(Entity);
  World.Relationships.Remove(Entity);
  for (TPair<EntityKey, FRelationship> &Entry : World.Relationships) {
    Entry.Value.Children.Remove(Entity);
  }
  World.DirtyEntities.Remove(Entity);
  return markDirty(World, Entity);
}

/**
 * @brief Frees an id and removes the matching entity data from the world.
 */
inline FWorld despawnEntity(FWorld World, const FEntityId &Id) {
  World.Allocator = freeEntity(World.Allocator, Id);
  return removeEntity(World, entityKey(Id));
}

/**
 * @brief Sets a parent relationship using immutable world-in/world-out data.
 */
inline FWorld setParent(FWorld World, const EntityKey &Child,
                        const EntityKey &Parent) {
  FRelationship ChildRelationship =
      World.Relationships.Contains(Child) ? World.Relationships.FindRef(Child)
                                          : createRelationship();
  ChildRelationship.Parent = func::just(Parent);
  World.Relationships.Add(Child, ChildRelationship);
  FRelationship ParentRelationship =
      World.Relationships.Contains(Parent)
          ? World.Relationships.FindRef(Parent)
          : createRelationship();
  ParentRelationship.Children.AddUnique(Child);
  World.Relationships.Add(Parent, ParentRelationship);
  return markDirty(markDirty(World, Child), Parent);
}

/**
 * @brief Adds one child to a parent relationship.
 */
inline FWorld addChild(FWorld World, const EntityKey &Parent,
                       const EntityKey &Child) {
  return setParent(World, Child, Parent);
}

/**
 * @brief Removes one child from a parent relationship.
 */
inline FWorld removeChild(FWorld World, const EntityKey &Parent,
                          const EntityKey &Child) {
  FRelationship ParentRelationship =
      World.Relationships.Contains(Parent)
          ? World.Relationships.FindRef(Parent)
          : createRelationship();
  ParentRelationship.Children.Remove(Child);
  World.Relationships.Add(Parent, ParentRelationship);
  FRelationship ChildRelationship =
      World.Relationships.Contains(Child) ? World.Relationships.FindRef(Child)
                                          : createRelationship();
  ChildRelationship.Parent = func::nothing<EntityKey>();
  World.Relationships.Add(Child, ChildRelationship);
  return markDirty(markDirty(World, Parent), Child);
}

inline TArray<EntityKey> appendUniqueEntity(TArray<EntityKey> Entities,
                                            const EntityKey &Entity) {
  Entities.AddUnique(Entity);
  return Entities;
}

/**
 * @brief Returns all known entities across domains, tags, and components.
 */
inline TArray<EntityKey> allEntities(const FWorld &World) {
  TArray<EntityKey> Entities;
  TArray<EntityKey> DomainEntities;
  World.EntityDomains.GenerateKeyArray(DomainEntities);
  Entities.Append(DomainEntities);
  TArray<EntityKey> TaggedEntities;
  World.Tags.GenerateKeyArray(TaggedEntities);
  Entities.Append(TaggedEntities);
  for (const TPair<ComponentType, ComponentTable> &Entry : World.Components) {
    TArray<EntityKey> ComponentEntities;
    Entry.Value.GenerateKeyArray(ComponentEntities);
    Entities.Append(ComponentEntities);
  }
  TArray<EntityKey> UniqueEntities;
  for (const EntityKey &Entity : Entities) {
    UniqueEntities.AddUnique(Entity);
  }
  return UniqueEntities;
}

/**
 * @brief Sets one component value on one entity.
 * @signature inline FWorld setComponent(FWorld World, const EntityKey &Entity, const ComponentType &Type, const FComponentValue &Value)
 *
 * User Story: As a mechanic, I need pure component writes that return the next
 * world value and mark the entity dirty for project-level rendering or syncing.
 */
inline FWorld setComponent(const FSetComponentRequest &Request) {
  FWorld World = Request.World;
  World.Components.FindOrAdd(Request.Type).Add(Request.Entity, Request.Value);
  return markDirty(World, Request.Entity);
}

inline FWorld setComponent(FWorld World, const EntityKey &Entity,
                           const ComponentType &Type,
                           const FComponentValue &Value) {
  return setComponent({World, Entity, Type, Value});
}

/**
 * @brief Reads one component value from an entity.
 */
inline func::Maybe<FComponentValue> getComponent(const FWorld &World,
                                                 const EntityKey &Entity,
                                                 const ComponentType &Type) {
  const ComponentTable *Table = World.Components.Find(Type);
  const FComponentValue *Value = Table ? Table->Find(Entity) : nullptr;
  return Value ? func::just(*Value) : func::nothing<FComponentValue>();
}

/**
 * @brief Checks whether an entity currently has a component type.
 */
inline bool hasComponent(const FWorld &World, const EntityKey &Entity,
                         const ComponentType &Type) {
  return getComponent(World, Entity, Type).hasValue;
}

/**
 * @brief Removes one component value from an entity and marks it dirty.
 */
inline FWorld removeComponent(FWorld World, const EntityKey &Entity,
                              const ComponentType &Type) {
  ComponentTable *Table = World.Components.Find(Type);
  if (Table) {
    Table->Remove(Entity);
  }
  return markDirty(World, Entity);
}

/**
 * @brief Adds one tag to one entity using a request payload.
 */
inline FWorld setTag(const FSetTagRequest &Request) {
  FWorld World = Request.World;
  World.Tags.FindOrAdd(Request.Entity).AddUnique(Request.TagValue);
  return markDirty(World, Request.Entity);
}

inline FWorld setTag(FWorld World, const EntityKey &Entity,
                     const Tag &TagValue) {
  return setTag({World, Entity, TagValue});
}

/**
 * @brief Checks whether an entity currently has a tag.
 */
inline bool hasTag(const FWorld &World, const EntityKey &Entity, const Tag &TagValue) {
  const TArray<Tag> *Tags = World.Tags.Find(Entity);
  return Tags && Tags->Contains(TagValue);
}

/**
 * @brief Sets one world resource using a request payload.
 */
inline FWorld setResource(const FSetResourceRequest &Request) {
  FWorld World = Request.World;
  World.Resources.Add(Request.Name, Request.Value);
  World.Generation += 1;
  return World;
}

inline FWorld setResource(FWorld World, const ResourceName &Name,
                          const FComponentValue &Value) {
  return setResource({World, Name, Value});
}

/**
 * @brief Reads one resource value from the world.
 */
inline func::Maybe<FComponentValue> getResource(const FWorld &World,
                                                const ResourceName &Name) {
  const FComponentValue *Value = World.Resources.Find(Name);
  return Value ? func::just(*Value) : func::nothing<FComponentValue>();
}

/**
 * @brief Appends one ECS event payload to its event queue.
 */
inline FWorld pushEvent(const FPushEventRequest &Request) {
  FWorld World = Request.World;
  World.Events.FindOrAdd(Request.Type).Add(Request.Payload);
  return World;
}

inline FWorld pushEvent(FWorld World, const EventType &Type,
                        const FComponentValue &Payload) {
  return pushEvent({World, Type, Payload});
}

/**
 * @brief Reads queued event payloads for one event type.
 */
inline TArray<FComponentValue> readEvents(const FWorld &World,
                                          const EventType &Type) {
  const TArray<FComponentValue> *Queue = World.Events.Find(Type);
  return Queue ? *Queue : TArray<FComponentValue>();
}

/**
 * @brief Clears all queued ECS events.
 */
inline FWorld clearEvents(FWorld World) {
  World.Events.Empty();
  return World;
}

/**
 * @brief Associates one entity with one ECS domain via request payload.
 */
inline FWorld setEntityDomain(const FSetEntityDomainRequest &Request) {
  FWorld World = Request.World;
  World.EntityDomains.FindOrAdd(Request.Entity).AddUnique(Request.Domain);
  return markDirty(World, Request.Entity);
}

inline FWorld setEntityDomain(FWorld World, const EntityKey &Entity,
                              const DomainPathKey &Domain) {
  return setEntityDomain({World, Entity, Domain});
}

/**
 * @brief Checks whether an entity belongs to a domain path.
 */
inline bool entityInDomain(const FWorld &World, const EntityKey &Entity,
                           const DomainPathKey &Domain) {
  const TArray<DomainPathKey> *Domains = World.EntityDomains.Find(Entity);
  return Domains && Domains->Contains(Domain);
}

/**
 * @brief Filters entities that contain every requested component type.
 */
inline TArray<EntityKey> queryComponents(const FWorld &World,
                                         const TArray<ComponentType> &Types,
                                         const TArray<EntityKey> &Entities) {
  return Entities.FilterByPredicate([&World, &Types](const EntityKey &Entity) {
    return !Types.ContainsByPredicate([&World, &Entity](const ComponentType &Type) {
      return !hasComponent(World, Entity, Type);
    });
  });
}

/**
 * @brief Queries all world entities by required components.
 */
inline TArray<EntityKey>
queryEntitiesByComponents(const FWorld &World,
                          const TArray<ComponentType> &Types) {
  return queryComponents(World, Types, allEntities(World));
}

/**
 * @brief Queries all world entities by tag.
 */
inline TArray<EntityKey> queryEntitiesByTag(const FWorld &World,
                                            const Tag &TagValue) {
  return allEntities(World).FilterByPredicate(
      [&World, &TagValue](const EntityKey &Entity) {
        return hasTag(World, Entity, TagValue);
      });
}

/**
 * @brief Queries all world entities by domain path.
 */
inline TArray<EntityKey> queryEntitiesByDomain(const FWorld &World,
                                               const DomainPathKey &Domain) {
  return allEntities(World).FilterByPredicate(
      [&World, &Domain](const EntityKey &Entity) {
        return entityInDomain(World, Entity, Domain);
      });
}

/**
 * @brief Returns entities dirtied by recent ECS world transforms.
 */
inline TArray<EntityKey> queryDirtyEntities(const FWorld &World) {
  return World.DirtyEntities;
}

/**
 * @brief Returns child entity keys for a parent.
 */
inline TArray<EntityKey> queryChildren(const FWorld &World,
                                       const EntityKey &Parent) {
  const FRelationship *Relationship = World.Relationships.Find(Parent);
  return Relationship ? Relationship->Children : TArray<EntityKey>();
}

/**
 * @brief Gathers available component values for a system input entity.
 */
inline TMap<ComponentType, FComponentValue>
gatherUnwrap(const FWorld &World, const EntityKey &Entity,
             const TArray<ComponentType> &Types) {
  TMap<ComponentType, FComponentValue> Result;
  for (const ComponentType &Type : Types) {
    func::Maybe<FComponentValue> Value = getComponent(World, Entity, Type);
    if (Value.hasValue) {
      Result.Add(Type, Value.value);
    }
  }
  return Result;
}

typedef TFunction<FWorld(FWorld, EntityKey, TMap<ComponentType, FComponentValue>)>
    SystemFn;

struct FSystemDescriptor {
  TArray<ComponentType> RequiredComponents;
  TArray<Tag> RequiredTags;
  func::Maybe<DomainPathKey> Domain;
  SystemFn System;
};

/**
 * @brief Creates a system descriptor for pure ECS system execution.
 */
inline FSystemDescriptor createSystemDescriptor(const TArray<ComponentType> &Components,
                                                const TArray<Tag> &Tags,
                                                func::Maybe<DomainPathKey> Domain,
                                                SystemFn System) {
  FSystemDescriptor Descriptor;
  Descriptor.RequiredComponents = Components;
  Descriptor.RequiredTags = Tags;
  Descriptor.Domain = Domain;
  Descriptor.System = System;
  return Descriptor;
}

/**
 * @brief Checks that an entity has all required tags for a system.
 */
inline bool matchesRequiredTags(const FWorld &World, const EntityKey &Entity,
                                const TArray<Tag> &Tags) {
  return !Tags.ContainsByPredicate([&World, &Entity](const Tag &TagValue) {
    return !hasTag(World, Entity, TagValue);
  });
}

/**
 * @brief Checks the optional domain guard for a system descriptor.
 */
inline bool matchesSystemDomain(const FWorld &World, const EntityKey &Entity,
                                const func::Maybe<DomainPathKey> &Domain) {
  return !Domain.hasValue || entityInDomain(World, Entity, Domain.value);
}

/**
 * @brief Runs one pure ECS system over matching entities.
 */
inline FWorld runSystem(FWorld World, const TArray<ComponentType> &Components,
                        const TArray<EntityKey> &Entities, SystemFn System) {
  const TArray<EntityKey> Matching = queryComponents(World, Components, Entities);
  FWorld Next = World;
  for (const EntityKey &Entity : Matching) {
    Next = System(Next, Entity, gatherUnwrap(Next, Entity, Components));
  }
  return Next;
}

/**
 * @brief Runs a system descriptor over entities matching components, tags, and
 * optional domain.
 */
inline FWorld runSystemDescriptor(FWorld World,
                                  const TArray<EntityKey> &Entities,
                                  const FSystemDescriptor &Descriptor) {
  const TArray<EntityKey> Matching =
      queryComponents(World, Descriptor.RequiredComponents, Entities)
          .FilterByPredicate([&World, &Descriptor](const EntityKey &Entity) {
            return matchesRequiredTags(World, Entity,
                                       Descriptor.RequiredTags) &&
                   matchesSystemDomain(World, Entity, Descriptor.Domain);
          });
  FWorld Next = World;
  for (const EntityKey &Entity : Matching) {
    Next = Descriptor.System(
        Next, Entity, gatherUnwrap(Next, Entity, Descriptor.RequiredComponents));
  }
  return Next;
}

/**
 * @brief Checks whether the registry contains a domain.
 */
inline bool domainExists(const FDomainRegistry &Registry,
                         const DomainPathKey &Domain) {
  return Registry.Nodes.Contains(Domain);
}

/**
 * @brief Checks whether the registry contains a component schema.
 */
inline bool componentSchemaExists(const FDomainRegistry &Registry,
                                  const ComponentType &Type) {
  return Registry.ComponentSchemas.Contains(Type);
}

/**
 * @brief Checks whether the registry contains a resource spec.
 */
inline bool resourceSpecExists(const FDomainRegistry &Registry,
                               const ResourceName &Name) {
  return Registry.ResourceSpecs.Contains(Name);
}

/**
 * @brief Checks whether the registry contains an event spec.
 */
inline bool eventSpecExists(const FDomainRegistry &Registry,
                            const EventType &Type) {
  return Registry.EventSpecs.Contains(Type);
}

/**
 * @brief Validates that a domain exists and returns Either error data.
 */
inline func::Either<FString, bool>
validateDomainExists(const FDomainRegistry &Registry,
                     const DomainPathKey &Domain) {
  return domainExists(Registry, Domain)
             ? func::make_right<FString, bool>(true)
             : func::make_left<FString, bool>(
                   FString::Printf(TEXT("Missing ECS domain: %s"), *Domain));
}

/**
 * @brief Validates that a component schema exists and returns Either error data.
 */
inline func::Either<FString, bool>
validateComponentSchemaExists(const FDomainRegistry &Registry,
                              const ComponentType &Type) {
  return componentSchemaExists(Registry, Type)
             ? func::make_right<FString, bool>(true)
             : func::make_left<FString, bool>(FString::Printf(
                   TEXT("Missing ECS component schema: %s"), *Type));
}

/**
 * @brief Validates entity membership in a domain.
 */
inline func::Either<FString, bool>
validateEntityDomain(const FWorld &World, const EntityKey &Entity,
                     const DomainPathKey &Domain) {
  return entityInDomain(World, Entity, Domain)
             ? func::make_right<FString, bool>(true)
             : func::make_left<FString, bool>(FString::Printf(
                   TEXT("Entity %s is not in ECS domain %s"), *Entity,
                   *Domain));
}

/**
 * @brief Validates a system spec against registered domain/component contracts.
 */
inline func::Either<FString, bool>
validateSystemSpec(const FDomainRegistry &Registry, const FSystemSpec &Spec) {
  func::Either<FString, bool> DomainResult =
      validateDomainExists(Registry, Spec.Domain);
  if (DomainResult.isLeft) {
    return DomainResult;
  }
  for (const ComponentType &Type : Spec.RequiredComponents) {
    func::Either<FString, bool> ComponentResult =
        validateComponentSchemaExists(Registry, Type);
    if (ComponentResult.isLeft) {
      return ComponentResult;
    }
  }
  return func::make_right<FString, bool>(true);
}

/**
 * @brief Formats a component value for ECS inspection output.
 */
inline FString componentValueToString(const FComponentValue &Value) {
  return Value.Kind == EComponentValueKind::Bool
             ? (Value.BoolValue ? TEXT("true") : TEXT("false"))
             : Value.Kind == EComponentValueKind::Int
                   ? FString::Printf(TEXT("%lld"), Value.IntValue)
                   : Value.Kind == EComponentValueKind::Float
                         ? FString::SanitizeFloat(Value.FloatValue)
                         : Value.Kind == EComponentValueKind::Text
                               ? Value.TextValue
                               : Value.Kind == EComponentValueKind::Vec2
                                     ? Value.Vec2Value.ToString()
                                     : Value.Kind == EComponentValueKind::Vec3
                                           ? Value.Vec3Value.ToString()
                                           : Value.Kind ==
                                                     EComponentValueKind::Map
                                                 ? FString::Printf(
                                                       TEXT("Map(len=%d)"),
                                                       Value.MapValue.Num())
                                                 : Value.Kind ==
                                                           EComponentValueKind::
                                                               List
                                                       ? FString::Printf(
                                                             TEXT("List(len=%d)"),
                                                             Value.ListValue.Num())
                                                       : TEXT("None");
}

struct FWorldInspection {
  int32 EntityCount;
  int32 ComponentTypeCount;
  int32 ResourceCount;
  int32 EventTypeCount;
  int32 DirtyEntityCount;
  int32 DomainCount;
  int64 Generation;
};

/**
 * @brief Compares aggregate ECS world inspection counters.
 */
inline bool operator==(const FWorldInspection &Left,
                       const FWorldInspection &Right) {
  return Left.EntityCount == Right.EntityCount &&
         Left.ComponentTypeCount == Right.ComponentTypeCount &&
         Left.ResourceCount == Right.ResourceCount &&
         Left.EventTypeCount == Right.EventTypeCount &&
         Left.DirtyEntityCount == Right.DirtyEntityCount &&
         Left.DomainCount == Right.DomainCount &&
         Left.Generation == Right.Generation;
}

inline bool operator!=(const FWorldInspection &Left,
                       const FWorldInspection &Right) {
  return !(Left == Right);
}

/**
 * @brief Inspects aggregate world counters without exposing mutable state.
 */
inline FWorldInspection inspectWorld(const FWorld &World) {
  FWorldInspection Inspection;
  Inspection.EntityCount = allEntities(World).Num();
  Inspection.ComponentTypeCount = World.Components.Num();
  Inspection.ResourceCount = World.Resources.Num();
  Inspection.EventTypeCount = World.Events.Num();
  Inspection.DirtyEntityCount = World.DirtyEntities.Num();
  Inspection.DomainCount = World.Domains.Nodes.Num();
  Inspection.Generation = World.Generation;
  return Inspection;
}

/**
 * @brief Formats aggregate ECS world inspection data for debugging.
 */
inline FString debugWorldSummary(const FWorld &World) {
  const FWorldInspection Inspection = inspectWorld(World);
  return FString::Printf(
      TEXT("ECS World: entities=%d componentTypes=%d resources=%d events=%d dirty=%d domains=%d generation=%lld"),
      Inspection.EntityCount, Inspection.ComponentTypeCount,
      Inspection.ResourceCount, Inspection.EventTypeCount,
      Inspection.DirtyEntityCount, Inspection.DomainCount,
      Inspection.Generation);
}

struct FEntityInspection {
  EntityKey Entity;
  TArray<Tag> Tags;
  TArray<DomainPathKey> Domains;
  TMap<ComponentType, FComponentValue> Components;
};

inline bool operator==(const FEntityInspection &Left,
                       const FEntityInspection &Right) {
  return Left.Entity == Right.Entity && Left.Tags == Right.Tags &&
         Left.Domains == Right.Domains &&
         Left.Components.OrderIndependentCompareEqual(Right.Components);
}

inline bool operator!=(const FEntityInspection &Left,
                       const FEntityInspection &Right) {
  return !(Left == Right);
}

/**
 * @brief Inspects one entity's tags, domains, and component values.
 */
inline FEntityInspection inspect(const FWorld &World, const EntityKey &Entity) {
  FEntityInspection Inspection;
  Inspection.Entity = Entity;
  const TArray<Tag> *Tags = World.Tags.Find(Entity);
  Inspection.Tags = Tags ? *Tags : TArray<Tag>();
  const TArray<DomainPathKey> *Domains = World.EntityDomains.Find(Entity);
  Inspection.Domains = Domains ? *Domains : TArray<DomainPathKey>();
  for (const TPair<ComponentType, ComponentTable> &Entry : World.Components) {
    const FComponentValue *Value = Entry.Value.Find(Entity);
    if (Value) {
      Inspection.Components.Add(Entry.Key, *Value);
    }
  }
  return Inspection;
}

} // namespace ecs

#endif // DEMO_PROJECT_CORE_ECS_HPP
