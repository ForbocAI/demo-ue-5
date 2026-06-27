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
 *   Mechanics / Ui taxonomy so Unreal features can share ECS vocabulary with
 *   Rust and GDScript game projects.
 * - As a reducer author, I need ECS worlds to be plain value structs so project
 *   files can compose ECS transitions with RTK state without ECS importing RTK.
 * - As a maintainer, I need this demo header outside the SDK core because the
 *   SDK should not own game-project ECS primitives.
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
  TMap<FString, FComponentValue> MapValue;
  TArray<FComponentValue> ListValue;
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

struct FDomainNode {
  FDomainPath Path;
  EDomainKind Kind;
  TArray<DomainPathKey> Children;
  TArray<ComponentType> ComponentSchemas;
  TArray<FString> Capabilities;
  TArray<SystemName> Systems;
};

struct FDomainRegistry {
  TMap<DomainPathKey, FDomainNode> Nodes;
  TMap<ComponentType, FComponentSchema> ComponentSchemas;
  TMap<FString, FCapabilitySpec> Capabilities;
  TMap<SystemName, FSystemSpec> Systems;
};

inline FDomainPath createDomainPath(const TArray<FString> &Segments) {
  FDomainPath Path;
  Path.Segments = Segments;
  return Path;
}

inline DomainPathKey domainPathKey(const FDomainPath &Path) {
  return FString::Join(Path.Segments, TEXT("/"));
}

inline FDomainNode createDomainNode(const FDomainPath &Path, EDomainKind Kind) {
  FDomainNode Node;
  Node.Path = Path;
  Node.Kind = Kind;
  return Node;
}

inline FDomainRegistry createDomainRegistry() { return FDomainRegistry(); }

inline void pushUnique(TArray<FString> &Values, const FString &Value) {
  Values.AddUnique(Value);
}

inline FDomainRegistry registerDomain(FDomainRegistry Registry,
                                      const FDomainNode &Node) {
  const DomainPathKey Key = domainPathKey(Node.Path);
  Registry.Nodes.Add(Key, Node);
  if (Node.Path.Segments.Num() > 1) {
    FDomainPath ParentPath = Node.Path;
    ParentPath.Segments.RemoveAt(ParentPath.Segments.Num() - 1);
    const DomainPathKey ParentKey = domainPathKey(ParentPath);
    FDomainNode *Parent = Registry.Nodes.Find(ParentKey);
    if (Parent) {
      pushUnique(Parent->Children, Key);
    }
  }
  return Registry;
}

inline FDomainRegistry registerDomainPath(FDomainRegistry Registry,
                                          const TArray<FString> &Segments,
                                          EDomainKind Kind) {
  return registerDomain(Registry, createDomainNode(createDomainPath(Segments), Kind));
}

/**
 * @brief Builds the neutral cross-domain taxonomy shared by ECS cores.
 * @signature inline FDomainRegistry createCrossDomainRegistry()
 *
 * User Story: As a feature author, I need Components / Entities / Mechanics /
 * Ui domains with stable subdomains so project systems can query and inspect
 * data without importing presentational view layers.
 */
inline FDomainRegistry createCrossDomainRegistry() {
  FDomainRegistry Registry = createDomainRegistry();
  Registry = registerDomainPath(Registry, {TEXT("Components")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Attributes")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Data")}, EDomainKind::Data);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Frame")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Framebuffer")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Geometry")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Glyphs")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Health")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Level")}, EDomainKind::Data);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Position")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Random")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Rendering")}, EDomainKind::Rendering);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Spatial")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Components"), TEXT("Stats")}, EDomainKind::Component);
  Registry = registerDomainPath(Registry, {TEXT("Entities")}, EDomainKind::Entity);
  Registry = registerDomainPath(Registry, {TEXT("Entities"), TEXT("Characters")}, EDomainKind::Entity);
  Registry = registerDomainPath(Registry, {TEXT("Entities"), TEXT("Components")}, EDomainKind::Entity);
  Registry = registerDomainPath(Registry, {TEXT("Entities"), TEXT("Environments")}, EDomainKind::Entity);
  Registry = registerDomainPath(Registry, {TEXT("Entities"), TEXT("Session")}, EDomainKind::Session);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Capabilities")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Factories")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Orchestrators")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Services")}, EDomainKind::Service);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Systems")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Transformations")}, EDomainKind::System);
  Registry = registerDomainPath(Registry, {TEXT("Mechanics"), TEXT("Utils")}, EDomainKind::Service);
  Registry = registerDomainPath(Registry, {TEXT("Ui")}, EDomainKind::Ui);
  Registry = registerDomainPath(Registry, {TEXT("Ui"), TEXT("Device")}, EDomainKind::Ui);
  Registry = registerDomainPath(Registry, {TEXT("Ui"), TEXT("Hud")}, EDomainKind::Ui);
  Registry = registerDomainPath(Registry, {TEXT("Ui"), TEXT("Notifications")}, EDomainKind::Ui);
  Registry = registerDomainPath(Registry, {TEXT("Ui"), TEXT("Screens")}, EDomainKind::Ui);
  Registry = registerDomainPath(Registry, {TEXT("Ui"), TEXT("Tokens")}, EDomainKind::Ui);
  return Registry;
}

struct FRelationship {
  func::Maybe<EntityKey> Parent;
  TArray<EntityKey> Children;
};

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

inline FWorld createWorld() {
  FWorld World;
  World.Allocator = createAllocator();
  World.Domains = createCrossDomainRegistry();
  World.Generation = 0;
  return World;
}

inline FWorld markDirty(FWorld World, const EntityKey &Entity) {
  World.DirtyEntities.AddUnique(Entity);
  World.Generation += 1;
  return World;
}

/**
 * @brief Sets one component value on one entity.
 * @signature inline FWorld setComponent(FWorld World, const EntityKey &Entity, const ComponentType &Type, const FComponentValue &Value)
 *
 * User Story: As a mechanic, I need pure component writes that return the next
 * world value and mark the entity dirty for project-level rendering or syncing.
 */
inline FWorld setComponent(FWorld World, const EntityKey &Entity,
                           const ComponentType &Type,
                           const FComponentValue &Value) {
  World.Components.FindOrAdd(Type).Add(Entity, Value);
  return markDirty(World, Entity);
}

inline func::Maybe<FComponentValue> getComponent(const FWorld &World,
                                                 const EntityKey &Entity,
                                                 const ComponentType &Type) {
  const ComponentTable *Table = World.Components.Find(Type);
  const FComponentValue *Value = Table ? Table->Find(Entity) : nullptr;
  return Value ? func::just(*Value) : func::nothing<FComponentValue>();
}

inline bool hasComponent(const FWorld &World, const EntityKey &Entity,
                         const ComponentType &Type) {
  return getComponent(World, Entity, Type).hasValue;
}

inline FWorld removeComponent(FWorld World, const EntityKey &Entity,
                              const ComponentType &Type) {
  ComponentTable *Table = World.Components.Find(Type);
  if (Table) {
    Table->Remove(Entity);
  }
  return markDirty(World, Entity);
}

inline FWorld setTag(FWorld World, const EntityKey &Entity, const Tag &TagValue) {
  World.Tags.FindOrAdd(Entity).AddUnique(TagValue);
  return markDirty(World, Entity);
}

inline bool hasTag(const FWorld &World, const EntityKey &Entity, const Tag &TagValue) {
  const TArray<Tag> *Tags = World.Tags.Find(Entity);
  return Tags && Tags->Contains(TagValue);
}

inline FWorld setResource(FWorld World, const ResourceName &Name,
                          const FComponentValue &Value) {
  World.Resources.Add(Name, Value);
  World.Generation += 1;
  return World;
}

inline func::Maybe<FComponentValue> getResource(const FWorld &World,
                                                const ResourceName &Name) {
  const FComponentValue *Value = World.Resources.Find(Name);
  return Value ? func::just(*Value) : func::nothing<FComponentValue>();
}

inline FWorld pushEvent(FWorld World, const EventType &Type,
                        const FComponentValue &Payload) {
  World.Events.FindOrAdd(Type).Add(Payload);
  return World;
}

inline TArray<FComponentValue> readEvents(const FWorld &World,
                                          const EventType &Type) {
  const TArray<FComponentValue> *Queue = World.Events.Find(Type);
  return Queue ? *Queue : TArray<FComponentValue>();
}

inline FWorld clearEvents(FWorld World) {
  World.Events.Empty();
  return World;
}

inline FWorld setEntityDomain(FWorld World, const EntityKey &Entity,
                              const DomainPathKey &Domain) {
  World.EntityDomains.FindOrAdd(Entity).AddUnique(Domain);
  return markDirty(World, Entity);
}

inline bool entityInDomain(const FWorld &World, const EntityKey &Entity,
                           const DomainPathKey &Domain) {
  const TArray<DomainPathKey> *Domains = World.EntityDomains.Find(Entity);
  return Domains && Domains->Contains(Domain);
}

inline TArray<EntityKey> queryComponents(const FWorld &World,
                                         const TArray<ComponentType> &Types,
                                         const TArray<EntityKey> &Entities) {
  return Entities.FilterByPredicate([&World, &Types](const EntityKey &Entity) {
    return !Types.ContainsByPredicate([&World, &Entity](const ComponentType &Type) {
      return !hasComponent(World, Entity, Type);
    });
  });
}

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

inline FWorld runSystem(FWorld World, const TArray<ComponentType> &Components,
                        const TArray<EntityKey> &Entities, SystemFn System) {
  const TArray<EntityKey> Matching = queryComponents(World, Components, Entities);
  FWorld Next = World;
  for (const EntityKey &Entity : Matching) {
    Next = System(Next, Entity, gatherUnwrap(Next, Entity, Components));
  }
  return Next;
}

struct FEntityInspection {
  EntityKey Entity;
  TArray<Tag> Tags;
  TArray<DomainPathKey> Domains;
  TMap<ComponentType, FComponentValue> Components;
};

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
