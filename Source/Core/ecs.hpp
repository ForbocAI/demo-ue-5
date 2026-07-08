#pragma once
#ifndef FORBOC_AI_GAME_CORE_ECS_HPP
#define FORBOC_AI_GAME_CORE_ECS_HPP

#include "CoreMinimal.h"
#include "Core/ue_fp.hpp"

#include <initializer_list>

/**
 * @file ecs.hpp
 * @brief Game-project ECS primitives that depend on FP only and never on RTK.
 *
 * User Stories:
 * - As a gameplay feature author, I need neutral Components / Entities /
 *   Systems taxonomy so Unreal features can share ECS vocabulary with
 *   Rust and GDScript game projects.
 * - As a reducer author, I need ECS worlds to be plain value structs so project
 *   files can compose ECS transitions with RTK state without ECS importing RTK.
 * - As a maintainer, I need this runtime header outside the SDK core because the
 *   SDK should not own game-project ECS primitives.
 *
 * Functional-core cookbook boundary:
 * - ECS helpers are pure value transforms over request structs.
 * - RTK slices/actions/selectors own store semantics and unidirectional data
 *   flow.
 * - ue_fp.hpp is used for composition, Maybe/Either, and lazy values;
 *   it must not model a replacement action, reducer, selector, or store layer.
 * - Neutral FP helpers for arrays, maps, predicates, traversal, lookup,
 *   validation, and catalogs belong in ue_fp.hpp. Do not re-declare those
 *   generic combinators in ecs.hpp; ECS should import them and only add
 *   world/entity/component/system semantics.
 * - When only nouns change, keep the nouns in catalogs and fold through one
 *   ECS transform. Paired catalogs cover selector/projector and descriptor/run
 *   relationships without feature-local recursive families.
 * - Do not replace every `if` with ternary chains. Domain alternatives should
 *   use `func::match` for Maybe/Either/result values, dispatcher or
 *   `multi_match` case tables for enum/string routing, and FP-core
 *   map/filter/fold/find/traverse helpers for collection decisions. Explicit
 *   guards are reserved for UE/effect boundaries.
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
 * @signature inline EntityKey createEntityKey(const FEntityId &Id)
 *
 * User Story: As component storage, I need a deterministic key for TMap-based
 * component tables.
 */
inline EntityKey createEntityKey(const FEntityId &Id) {
  return LexToString(Id.Index) + FString::Chr(TCHAR(':')) +
         LexToString(Id.Generation);
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
 * @signature inline FAllocator createEntityAllocator()
 *
 * User Story: As a world author, I need isolated allocator state for each world
 * so tests, levels, and runtime sessions do not share entity id counters.
 */
inline FAllocator createEntityAllocator() {
  FAllocator Allocator;
  Allocator.NextIndex = int64{};
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
 * @brief Reads the last reusable entity id from the allocator.
 * @signature inline func::Maybe<FEntityId> findReusableEntityId(const FAllocator &Allocator)
 *
 * User Story: As allocation code, I need the reusable-id branch represented as
 * Maybe so fresh allocation composes without imperative checks.
 */
inline func::Maybe<FEntityId> findReusableEntityId(const FAllocator &Allocator) {
  return !Allocator.Freed.IsEmpty() ? func::just(Allocator.Freed.Last())
                                    : func::nothing<FEntityId>();
}

/**
 * @brief Allocates a previously freed entity id.
 * @signature inline FAllocatedEntity allocateRecycledEntity(FAllocator Allocator, const FEntityId &Entity)
 *
 * User Story: As allocator code, I need recycled allocation to be a small
 * unary-friendly value transform used by the Maybe match branch.
 */
inline FAllocatedEntity allocateRecycledEntity(FAllocator Allocator,
                                               const FEntityId &Entity) {
  FAllocatedEntity Result;
  Allocator.Freed.Pop();
  Result.Entity = Entity;
  Result.Allocator = Allocator;
  return Result;
}

/**
 * @brief Allocates a fresh entity id and records its generation.
 * @signature inline FAllocatedEntity allocateFreshEntity(FAllocator Allocator)
 *
 * User Story: As allocator code, I need the fresh-id branch isolated so entity
 * allocation composes from reusable branch functions.
 */
inline FAllocatedEntity allocateFreshEntity(FAllocator Allocator) {
  FAllocatedEntity Result;
  Result.Entity = createEntityId(Allocator.NextIndex, int32{});
  Allocator.Generations.Add(Allocator.NextIndex, int32{});
  ++Allocator.NextIndex;
  Result.Allocator = Allocator;
  return Result;
}

/**
 * @brief Allocates a fresh or recycled entity id.
 * @signature inline FAllocatedEntity allocateEntity(FAllocator Allocator)
 *
 * User Story: As a spawn mechanic, I need entity allocation to return new
 * allocator state plus the allocated id so state transitions stay value based.
 */
inline FAllocatedEntity allocateEntity(FAllocator Allocator) {
  return func::match(
      findReusableEntityId(Allocator),
      [&Allocator](const FEntityId &Entity) {
        return allocateRecycledEntity(Allocator, Entity);
      },
      [&Allocator]() { return allocateFreshEntity(Allocator); });
}

/**
 * @brief Checks whether an entity id matches its allocator generation.
 * @signature inline bool entityGenerationMatches(const FAllocator &Allocator, const FEntityId &Id)
 *
 * User Story: As allocator code, I need stale-handle checks to share one Maybe
 * lookup predicate across free and alive queries.
 */
inline bool entityGenerationMatches(const FAllocator &Allocator,
                                    const FEntityId &Id) {
  return func::match(
      func::find_map_value<int64, int32>(Allocator.Generations, Id.Index),
      [&Id](const int32 &Current) { return Current == Id.Generation; },
      []() { return false; });
}

/**
 * @brief Frees an entity id after its generation is known to match.
 * @signature inline FAllocator freeMatchedEntity(FAllocator Allocator, const FEntityId &Id)
 *
 * User Story: As despawn code, I need generation incrementing isolated from the
 * Maybe predicate that decides whether an id can be released.
 */
inline FAllocator freeMatchedEntity(FAllocator Allocator, const FEntityId &Id) {
  int32 NextGeneration = Id.Generation;
  ++NextGeneration;
  Allocator.Generations.Add(Id.Index, NextGeneration);
  Allocator.Freed.Add(createEntityId(Id.Index, NextGeneration));
  return Allocator;
}

/**
 * @brief Frees an entity id and increments its generation for safe reuse.
 * @signature inline FAllocator freeEntityId(FAllocator Allocator, const FEntityId &Id)
 *
 * User Story: As a despawn mechanic, I need old handles to stop resolving after
 * their slot has been released.
 */
inline FAllocator freeEntityId(FAllocator Allocator, const FEntityId &Id) {
  return entityGenerationMatches(Allocator, Id)
             ? freeMatchedEntity(Allocator, Id)
             : Allocator;
}

/**
 * @brief Checks whether an entity id is still alive in the allocator.
 * @signature inline bool isEntityAlive(const FAllocator &Allocator, const FEntityId &Id)
 *
 * User Story: As a query system, I need to reject stale handles before reading
 * component data.
 */
inline bool isEntityAlive(const FAllocator &Allocator, const FEntityId &Id) {
  return entityGenerationMatches(Allocator, Id);
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

inline FComponentValue createNoneComponentValue() {
  FComponentValue Value;
  Value.Kind = EComponentValueKind::None;
  Value.BoolValue = false;
  Value.IntValue = int64{};
  Value.FloatValue = float{};
  return Value;
}

inline FComponentValue createBoolComponentValue(bool RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Bool;
  Value.BoolValue = RawValue;
  return Value;
}

inline FComponentValue createIntComponentValue(int64 RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Int;
  Value.IntValue = RawValue;
  return Value;
}

inline FComponentValue createFloatComponentValue(float RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Float;
  Value.FloatValue = RawValue;
  return Value;
}

inline FComponentValue createTextComponentValue(const FString &RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Text;
  Value.TextValue = RawValue;
  return Value;
}

inline FComponentValue createVec3ComponentValue(const FVector &RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Vec3;
  Value.Vec3Value = RawValue;
  return Value;
}

inline FComponentValue createVec2ComponentValue(const FVector2D &RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Vec2;
  Value.Vec2Value = RawValue;
  return Value;
}

inline FComponentValue
createMapComponentValue(const TMap<FString, FComponentValue> &RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::Map;
  const TArray<FString> Keys = func::map_keys<FString, FComponentValue>(RawValue);
  Value.MapValue =
      func::fold_array<FString, TMap<FString, TSharedPtr<FComponentValue>>>(
      Keys, TMap<FString, TSharedPtr<FComponentValue>>(),
      [&RawValue](const TMap<FString, TSharedPtr<FComponentValue>> &Acc,
                  const FString &Key) {
        return func::match(
            func::find_map_value_ptr<FString, FComponentValue>(RawValue, Key),
            [&Acc, &Key](const FComponentValue *Found) {
              TMap<FString, TSharedPtr<FComponentValue>> Next = Acc;
              Next.Add(Key, MakeShared<FComponentValue>(*Found));
              return Next;
            },
            [&Acc]() { return Acc; });
      });
  return Value;
}

inline FComponentValue createListComponentValue(const TArray<FComponentValue> &RawValue) {
  FComponentValue Value = createNoneComponentValue();
  Value.Kind = EComponentValueKind::List;
  Value.ListValue =
      func::fold_array<FComponentValue, TArray<TSharedPtr<FComponentValue>>>(
      RawValue, TArray<TSharedPtr<FComponentValue>>(),
      [](const TArray<TSharedPtr<FComponentValue>> &Acc,
         const FComponentValue &Entry) {
        TArray<TSharedPtr<FComponentValue>> Next = Acc;
        Next.Add(MakeShared<FComponentValue>(Entry));
        return Next;
      });
  return Value;
}

/**
 * @brief Maps source rows into ECS component values through the neutral array mapper.
 * @signature template <typename Source> inline TArray<FComponentValue> mapComponentValues(const TArray<Source> &Items, std::function<FComponentValue(const Source &)> MapValue)
 *
 * User Story: As projection adapters, component-list values should share one
 * ECS mapper instead of building request/factory families per source noun.
 */
template <typename Source>
inline TArray<FComponentValue>
mapComponentValues(const TArray<Source> &Items,
                   std::function<FComponentValue(const Source &)> MapValue) {
  return func::map_array<Source, FComponentValue>(Items, MapValue);
}

inline bool componentValuePtrEquals(const TSharedPtr<FComponentValue> &Left,
                                    const TSharedPtr<FComponentValue> &Right);

inline bool componentMapEquals(
    const TMap<FString, TSharedPtr<FComponentValue>> &Left,
    const TMap<FString, TSharedPtr<FComponentValue>> &Right) {
  return func::map_values_equal<FString, TSharedPtr<FComponentValue>>(
      Left, Right,
      [](const TSharedPtr<FComponentValue> &LeftValue,
         const TSharedPtr<FComponentValue> &RightValue) {
        return componentValuePtrEquals(LeftValue, RightValue);
      });
}

inline bool componentListEquals(
    const TArray<TSharedPtr<FComponentValue>> &Left,
    const TArray<TSharedPtr<FComponentValue>> &Right) {
  const TArray<int32> Indices = func::index_range(Left.Num());
  return Left.Num() == Right.Num() &&
         func::all_array<int32>(
             Indices,
             [&Left, &Right](const int32 &Index) {
               return componentValuePtrEquals(Left[Index], Right[Index]);
             });
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

struct FCreateFieldSchemaRequest {
  FString Name;
  FString Kind;
  bool bRequired = false;
  FString Description;
};

struct FCreateComponentSchemaRequest {
  ComponentType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  TArray<Tag> Tags;
  FString Description;
};

struct FCreateCapabilitySpecRequest {
  FString Name;
  DomainPathKey Domain;
  TArray<ComponentType> Reads;
  TArray<ComponentType> Writes;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FCreateSystemSpecRequest {
  SystemName Name;
  DomainPathKey Domain;
  TArray<ComponentType> Components;
  TArray<Tag> Tags;
  TArray<ResourceName> Resources;
  TArray<EventType> Emits;
};

struct FCreateResourceSpecRequest {
  ResourceName Name;
  DomainPathKey Domain;
  FString Kind;
  FString Description;
};

struct FCreateEventSpecRequest {
  EventType Type;
  DomainPathKey Domain;
  TArray<FFieldSchema> Fields;
  FString Description;
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

  FDomainPathRegistration() {}

  FDomainPathRegistration(std::initializer_list<const char *> InSegments,
                          EDomainKind InKind)
      : Kind(InKind) {
    const TArray<const char *> SegmentAtoms(InSegments);
    Segments = func::map_array<const char *, FString>(
        SegmentAtoms, [](const char *Segment) {
          return FString(UTF8_TO_TCHAR(Segment));
        });
  }
};

struct FRegisterDomainPathsRequest {
  FDomainRegistry Registry;
  TArray<FDomainPathRegistration> Registrations;
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
 * @signature inline DomainPathKey createDomainPathKey(const FDomainPath &Path)
 */
inline DomainPathKey createDomainPathKey(const FDomainPath &Path) {
  const FString Separator = FString::Chr(TCHAR('/'));
  return FString::Join(Path.Segments, *Separator);
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
 * @signature inline FFieldSchema createFieldSchema(const FCreateFieldSchemaRequest &Request)
 *
 * User Story: As a registry author, I need schema fields to be built from one
 * explicit payload so neutral domain metadata composes predictably.
 */
inline FFieldSchema createFieldSchema(const FCreateFieldSchemaRequest &Request) {
  FFieldSchema Schema;
  Schema.Name = Request.Name;
  Schema.Kind = Request.Kind;
  Schema.bRequired = Request.bRequired;
  Schema.Description = Request.Description;
  return Schema;
}

/**
 * @brief Creates an ECS component schema owned by one domain path.
 * @signature inline FComponentSchema createComponentSchema(const FCreateComponentSchemaRequest &Request)
 *
 * User Story: As a component-domain owner, I need component schema
 * construction to stay unary and data-shaped for registry folds.
 */
inline FComponentSchema
createComponentSchema(const FCreateComponentSchemaRequest &Request) {
  FComponentSchema Schema;
  Schema.Type = Request.Type;
  Schema.Domain = Request.Domain;
  Schema.Fields = Request.Fields;
  Schema.Tags = Request.Tags;
  Schema.Description = Request.Description;
  return Schema;
}

/**
 * @brief Creates a capability contract describing component/resource access.
 * @signature inline FCapabilitySpec createCapabilitySpec(const FCreateCapabilitySpecRequest &Request)
 *
 * User Story: As an ECS systems author, I need capability metadata to be a
 * plain request-to-value transform below RTK slices.
 */
inline FCapabilitySpec
createCapabilitySpec(const FCreateCapabilitySpecRequest &Request) {
  FCapabilitySpec Spec;
  Spec.Name = Request.Name;
  Spec.Domain = Request.Domain;
  Spec.Reads = Request.Reads;
  Spec.Writes = Request.Writes;
  Spec.Resources = Request.Resources;
  Spec.Emits = Request.Emits;
  return Spec;
}

/**
 * @brief Creates a system contract for registry inspection and validation.
 * @signature inline FSystemSpec createSystemSpec(const FCreateSystemSpecRequest &Request)
 *
 * User Story: As an ECS system author, I need system metadata to remain a
 * value object that reducers may reference without owning system execution.
 */
inline FSystemSpec createSystemSpec(const FCreateSystemSpecRequest &Request) {
  FSystemSpec Spec;
  Spec.Name = Request.Name;
  Spec.Domain = Request.Domain;
  Spec.RequiredComponents = Request.Components;
  Spec.RequiredTags = Request.Tags;
  Spec.Resources = Request.Resources;
  Spec.Emits = Request.Emits;
  return Spec;
}

/**
 * @brief Creates a named ECS resource contract owned by one domain.
 * @signature inline FResourceSpec createResourceSpec(const FCreateResourceSpecRequest &Request)
 *
 * User Story: As a resource-domain maintainer, I need resource specs created
 * from one payload so registration remains foldable and neutral.
 */
inline FResourceSpec
createResourceSpec(const FCreateResourceSpecRequest &Request) {
  FResourceSpec Spec;
  Spec.Name = Request.Name;
  Spec.Domain = Request.Domain;
  Spec.Kind = Request.Kind;
  Spec.Description = Request.Description;
  return Spec;
}

/**
 * @brief Creates a typed event contract owned by one domain.
 * @signature inline FEventSpec createEventSpec(const FCreateEventSpecRequest &Request)
 *
 * User Story: As an event-domain maintainer, I need event specs to remain ECS
 * metadata rather than replacing RTK action semantics.
 */
inline FEventSpec createEventSpec(const FCreateEventSpecRequest &Request) {
  FEventSpec Spec;
  Spec.Type = Request.Type;
  Spec.Domain = Request.Domain;
  Spec.Fields = Request.Fields;
  Spec.Description = Request.Description;
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

typedef std::function<FDomainNode(const FDomainNode &)> FDomainNodeTransform;

inline func::Maybe<DomainPathKey> findParentDomainKey(const FDomainPath &Path) {
  return func::fmap(
      func::maybe_filter(
	          func::just<FDomainPath>(Path),
	          [](const FDomainPath &Candidate) {
	            return func::match(
	                func::maybe_filter(
	                    func::just<TArray<FString>>(Candidate.Segments),
	                    [](const TArray<FString> &Segments) {
	                      return !Segments.IsEmpty();
	                    }),
	                [](const TArray<FString> &Segments) {
	                  TArray<FString> ParentCandidate = Segments;
	                  ParentCandidate.Pop(EAllowShrinking::No);
	                  return !ParentCandidate.IsEmpty();
	                },
	                []() { return false; });
	          }),
	      [](const FDomainPath &Candidate) {
	        FDomainPath ParentPath = Candidate;
	        ParentPath.Segments.Pop(EAllowShrinking::No);
	        return createDomainPathKey(ParentPath);
	      });
}

inline FDomainRegistry updateDomainNode(FDomainRegistry Registry,
                                        const DomainPathKey &Key,
                                        FDomainNodeTransform Transform) {
  Registry.Nodes =
      func::update_map_value_when_present<DomainPathKey, FDomainNode>(
          Registry.Nodes, Key, Transform);
  return Registry;
}

inline FDomainNode appendDomainChild(const DomainPathKey &Child,
                                     const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.Children =
      func::append_unique_value<DomainPathKey>(Next.Children, Child);
  return Next;
}

inline FDomainNode appendDomainComponentSchema(const ComponentType &Type,
                                               const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.ComponentSchemas =
      func::append_unique_value<ComponentType>(Next.ComponentSchemas, Type);
  return Next;
}

inline FDomainNode appendDomainCapability(const FString &Name,
                                          const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.Capabilities =
      func::append_unique_value<FString>(Next.Capabilities, Name);
  return Next;
}

inline FDomainNode appendDomainSystem(const SystemName &Name,
                                      const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.Systems = func::append_unique_value<SystemName>(Next.Systems, Name);
  return Next;
}

inline FDomainNode appendDomainResource(const ResourceName &Name,
                                        const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.Resources =
      func::append_unique_value<ResourceName>(Next.Resources, Name);
  return Next;
}

inline FDomainNode appendDomainEvent(const EventType &Type,
                                     const FDomainNode &Node) {
  FDomainNode Next = Node;
  Next.Events = func::append_unique_value<EventType>(Next.Events, Type);
  return Next;
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
  const DomainPathKey Key = createDomainPathKey(Node.Path);
  Registry.Nodes.Add(Key, Node);
  return func::match(
      findParentDomainKey(Node.Path),
      [Registry, Key](const DomainPathKey &ParentKey) mutable {
        return updateDomainNode(
            Registry, ParentKey,
            [Key](const FDomainNode &Parent) {
              return appendDomainChild(Key, Parent);
            });
      },
      [Registry]() { return Registry; });
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
 * @brief Registers domain path declarations through the shared array fold.
 * @signature inline FDomainRegistry registerDomainPaths(FRegisterDomainPathsRequest Request)
 *
 * User Story: As a maintainer, I can extend the ECS taxonomy by appending data
 * declarations while one fold owns the traversal shape.
 */
inline FDomainRegistry
registerDomainPaths(FRegisterDomainPathsRequest Request) {
  return func::fold_array<FDomainPathRegistration, FDomainRegistry>(
      Request.Registrations, Request.Registry,
      [](const FDomainRegistry &Registry,
         const FDomainPathRegistration &Registration) {
        return registerDomainPath(
            {Registry, Registration.Segments, Registration.Kind});
      });
}

/**
 * @brief Registers ECS domain path declarations through one boundary function.
 * @signature inline FDomainRegistry registerDomainPathDeclarations(FDomainRegistry Registry, std::initializer_list<FDomainPathRegistration> Declarations)
 *
 * User Story: As taxonomy code, path declarations should be plain atoms while
 * this ECS helper owns platform string conversion, request construction, and
 * fold execution.
 */
inline FDomainRegistry registerDomainPathDeclarations(
    FDomainRegistry Registry,
    std::initializer_list<FDomainPathRegistration> Declarations) {
  const TArray<FDomainPathRegistration> Registrations(Declarations);
  return registerDomainPaths({Registry, Registrations});
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
  return updateDomainNode(
      Registry, Schema.Domain, [Schema](const FDomainNode &Domain) {
        return appendDomainComponentSchema(Schema.Type, Domain);
      });
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
  return updateDomainNode(
      Registry, Spec.Domain, [Spec](const FDomainNode &Domain) {
        return appendDomainCapability(Spec.Name, Domain);
      });
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
  return updateDomainNode(
      Registry, Spec.Domain, [Spec](const FDomainNode &Domain) {
        return appendDomainSystem(Spec.Name, Domain);
      });
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
  return updateDomainNode(
      Registry, Spec.Domain, [Spec](const FDomainNode &Domain) {
        return appendDomainResource(Spec.Name, Domain);
      });
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
  return updateDomainNode(
      Registry, Spec.Domain, [Spec](const FDomainNode &Domain) {
        return appendDomainEvent(Spec.Type, Domain);
      });
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
 * @brief Builds a domain registry from authored domain path declarations.
 * @signature inline FDomainRegistry createDomainRegistry(const TArray<FDomainPathRegistration> &Registrations)
 *
 * User Story: As a runtime data author, ECS taxonomy should be loaded from
 * authored settings while the core still owns the registry fold machinery.
 */
inline FDomainRegistry
createDomainRegistry(const TArray<FDomainPathRegistration> &Registrations) {
  return registerDomainPaths({createDomainRegistry(), Registrations});
}

struct FRelationship {
  func::Maybe<EntityKey> Parent;
  TArray<EntityKey> Children;
};

struct FGetRelationshipRequest {
  const TMap<EntityKey, FRelationship> &Relationships;
  EntityKey Entity;
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

/**
 * @brief Builds a relationship lookup request payload.
 * @signature inline FGetRelationshipRequest createGetRelationshipRequest(const TMap<EntityKey, FRelationship> &Relationships, const EntityKey &Entity)
 *
 * User Story: As hierarchy ECS code, I need relationship reads to share one
 * lookup payload instead of repeating map branches.
 */
inline FGetRelationshipRequest
createGetRelationshipRequest(const TMap<EntityKey, FRelationship> &Relationships,
                             const EntityKey &Entity) {
  return {Relationships, Entity};
}

/**
 * @brief Reads a relationship or returns an empty default relationship.
 * @signature inline FRelationship getRelationshipOrDefault(const FGetRelationshipRequest &Request)
 *
 * User Story: As relationship reducers, parent and child updates should share
 * one Maybe-backed read helper.
 */
inline FRelationship
getRelationshipOrDefault(const FGetRelationshipRequest &Request) {
  return func::or_else(
      func::find_map_value<EntityKey, FRelationship>(Request.Relationships,
                                             Request.Entity),
      createRelationship());
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

typedef std::function<FWorld(const FWorld &)> FWorldTransform;
typedef std::function<FWorldTransform(const ComponentType &)>
    FComponentTypeWorldTransformFactory;

template <typename Item>
using TWorldRowProjector =
    std::function<FWorld(const FWorld &, const Item &)>;

/**
 * @brief Projects selected rows into a world through one row projector.
 * @signature template <typename Item> inline FWorld projectRowsIntoWorld(const FWorld &World, const TArray<Item> &Items, TWorldRowProjector<Item> Project)
 *
 * User Story: As RTK selector projection code, selected entity-adapter rows
 * should fold into ECS through one reusable world transform shape.
 */
template <typename Item>
inline FWorld projectRowsIntoWorld(const FWorld &World, const TArray<Item> &Items,
                          TWorldRowProjector<Item> Project) {
  return func::fold_array<Item, FWorld>(
      Items, World, [Project](const FWorld &Acc, const Item &ItemValue) {
        return Project(Acc, ItemValue);
      });
}

/**
 * @brief Projects paired catalogs into a world through one projection step.
 * @signature template <typename LeftCatalog, typename RightCatalog, typename Step> inline FWorld projectWorldCatalogPairs(const FWorld &World, const LeftCatalog &Left, const RightCatalog &Right, Step ProjectStep)
 *
 * User Story: As RTK selector projection code, changing domain nouns should be
 * registered as selector/projector catalogs and executed by one ECS fold.
 */
template <typename LeftCatalog, typename RightCatalog, typename Step>
inline FWorld projectWorldCatalogPairs(const FWorld &World,
                                       const LeftCatalog &Left,
                                       const RightCatalog &Right,
                                       Step ProjectStep) {
  return func::zip_catalog_fold(Left, Right, World, ProjectStep);
}

/**
 * @brief Applies a catalog of world transforms into one next world.
 * @signature template <typename TransformCatalog> inline FWorld applyWorldTransformCatalog(const FWorld &World, const TransformCatalog &Transforms)
 *
 * User Story: As lifecycle ECS code, named world transforms should be
 * registered as a catalog when only the cleanup noun changes.
 */
template <typename TransformCatalog>
inline FWorld applyWorldTransformCatalog(const FWorld &World,
                                         const TransformCatalog &Transforms) {
  return func::fold_catalog(
      Transforms, World,
      [](const FWorld &Acc, const FWorldTransform &Transform) {
        return Transform(Acc);
      });
}

/**
 * @brief Applies a list of world transforms to one next world.
 * @signature inline FWorld applyWorldTransforms(const FWorld &World, const TArray<FWorldTransform> &Transforms)
 *
 * User Story: As reducer-owned ECS projection, system execution should read as
 * a fold over named world transforms instead of hand-composed nested calls.
 */
inline FWorld applyWorldTransforms(const FWorld &World,
                                   const TArray<FWorldTransform> &Transforms) {
  return func::fold_array<FWorldTransform, FWorld>(
      Transforms, World,
      [](const FWorld &Acc, const FWorldTransform &Transform) {
        return Transform(Acc);
      });
}

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

struct FRecordEntityDirtyRequest {
  FWorld World;
  EntityKey Entity;
};

struct FRecordEntitiesDirtyRequest {
  FWorld World;
  TArray<EntityKey> Entities;
};

struct FSpawnEntityInDomainRequest {
  FWorld World;
  DomainPathKey Domain;
};

struct FRemoveEntityRequest {
  FWorld World;
  EntityKey Entity;
};

struct FDespawnEntityRequest {
  FWorld World;
  FEntityId Id;
};

struct FSetRelationshipParentRequest {
  FWorld World;
  EntityKey Child;
  EntityKey Parent;
};

struct FAddRelationshipChildRequest {
  FWorld World;
  EntityKey Parent;
  EntityKey Child;
};

struct FRemoveRelationshipChildRequest {
  FWorld World;
  EntityKey Parent;
  EntityKey Child;
};

struct FGetComponentRequest {
  const FWorld &World;
  EntityKey Entity;
  ComponentType Type;
};

struct FHasComponentRequest {
  const FWorld &World;
  EntityKey Entity;
  ComponentType Type;
};

struct FRemoveComponentRequest {
  FWorld World;
  EntityKey Entity;
  ComponentType Type;
};

struct FHasTagRequest {
  const FWorld &World;
  EntityKey Entity;
  Tag TagValue;
};

struct FGetResourceRequest {
  const FWorld &World;
  ResourceName Name;
};

struct FReadEventsRequest {
  const FWorld &World;
  EventType Type;
};

struct FEntityInDomainRequest {
  const FWorld &World;
  EntityKey Entity;
  DomainPathKey Domain;
};

struct FQueryComponentsRequest {
  const FWorld &World;
  TArray<ComponentType> Types;
  TArray<EntityKey> Entities;
};

struct FQueryEntitiesByComponentsRequest {
  const FWorld &World;
  TArray<ComponentType> Types;
};

struct FQueryEntitiesByTagRequest {
  const FWorld &World;
  Tag TagValue;
};

struct FQueryEntitiesByDomainRequest {
  const FWorld &World;
  DomainPathKey Domain;
};

struct FQueryChildrenRequest {
  const FWorld &World;
  EntityKey Parent;
};

struct FGatherComponentsRequest {
  const FWorld &World;
  EntityKey Entity;
  TArray<ComponentType> Types;
};

/**
 * @brief Builds the dirty entity recording request payload.
 * @signature inline FRecordEntityDirtyRequest createRecordEntityDirtyRequest(FWorld World, const EntityKey &Entity)
 *
 * User Story: As ECS reducer code, I need one reusable factory for dirty
 * marking payloads instead of scattered aggregate construction.
 */
inline FRecordEntityDirtyRequest createRecordEntityDirtyRequest(FWorld World,
                                                const EntityKey &Entity) {
  return {World, Entity};
}

/**
 * @brief Builds the dirty entity batch recording request payload.
 * @signature inline FRecordEntitiesDirtyRequest createRecordEntitiesDirtyRequest(FWorld World, const TArray<EntityKey> &Entities)
 *
 * User Story: As relationship reducers, I need repeated dirty marking to use a
 * foldable payload rather than nested calls.
 */
inline FRecordEntitiesDirtyRequest
createRecordEntitiesDirtyRequest(FWorld World,
                                 const TArray<EntityKey> &Entities) {
  return {World, Entities};
}

/**
 * @brief Builds the spawn-in-domain request payload.
 * @signature inline FSpawnEntityInDomainRequest createSpawnEntityInDomainRequest(FWorld World, const DomainPathKey &Domain)
 *
 * User Story: As spawn code, I need a reusable payload factory for domain-bound
 * entity allocation.
 */
inline FSpawnEntityInDomainRequest
createSpawnEntityInDomainRequest(FWorld World, const DomainPathKey &Domain) {
  return {World, Domain};
}

/**
 * @brief Builds the entity-removal request payload.
 * @signature inline FRemoveEntityRequest createRemoveEntityRequest(FWorld World, const EntityKey &Entity)
 *
 * User Story: As lifecycle code, I need entity cleanup payloads constructed
 * consistently from one factory.
 */
inline FRemoveEntityRequest createRemoveEntityRequest(FWorld World,
                                                      const EntityKey &Entity) {
  return {World, Entity};
}

/**
 * @brief Builds the despawn request payload.
 * @signature inline FDespawnEntityRequest createDespawnEntityRequest(FWorld World, const FEntityId &Id)
 *
 * User Story: As lifecycle code, I need allocator cleanup payloads constructed
 * consistently from one factory.
 */
inline FDespawnEntityRequest createDespawnEntityRequest(FWorld World,
                                                        const FEntityId &Id) {
  return {World, Id};
}

/**
 * @brief Builds the parent-setting request payload.
 * @signature inline FSetRelationshipParentRequest createSetRelationshipParentRequest(FWorld World, const EntityKey &Child, const EntityKey &Parent)
 *
 * User Story: As relationship code, I need parent writes to use a reusable
 * payload factory.
 */
inline FSetRelationshipParentRequest createSetRelationshipParentRequest(FWorld World,
                                                const EntityKey &Child,
                                                const EntityKey &Parent) {
  return {World, Child, Parent};
}

/**
 * @brief Builds the child-adding request payload.
 * @signature inline FAddRelationshipChildRequest createAddRelationshipChildRequest(FWorld World, const EntityKey &Parent, const EntityKey &Child)
 *
 * User Story: As hierarchy code, I need child-add payloads constructed
 * consistently before delegating to relationship reducers.
 */
inline FAddRelationshipChildRequest createAddRelationshipChildRequest(FWorld World,
                                              const EntityKey &Parent,
                                              const EntityKey &Child) {
  return {World, Parent, Child};
}

/**
 * @brief Builds the child-removal request payload.
 * @signature inline FRemoveRelationshipChildRequest createRemoveRelationshipChildRequest(FWorld World, const EntityKey &Parent, const EntityKey &Child)
 *
 * User Story: As hierarchy code, I need child-removal payloads constructed
 * consistently before applying relationship reducers.
 */
inline FRemoveRelationshipChildRequest createRemoveRelationshipChildRequest(FWorld World,
                                                    const EntityKey &Parent,
                                                    const EntityKey &Child) {
  return {World, Parent, Child};
}

/**
 * @brief Builds the component-write request payload.
 * @signature inline FSetComponentRequest createSetComponentRequest(FWorld World, const EntityKey &Entity, const ComponentType &Type, const FComponentValue &Value)
 *
 * User Story: As component reducers, I need one factory for component writes so
 * callers do not hand-assemble payload structs.
 */
inline FSetComponentRequest
createSetComponentRequest(FWorld World, const EntityKey &Entity,
                          const ComponentType &Type,
                          const FComponentValue &Value) {
  return {World, Entity, Type, Value};
}

/**
 * @brief Builds the component-read request payload.
 * @signature inline FGetComponentRequest createGetComponentRequest(const FWorld &World, const EntityKey &Entity, const ComponentType &Type)
 *
 * User Story: As ECS readers, I need component lookup payloads created through
 * one factory before Maybe composition.
 */
inline FGetComponentRequest
createGetComponentRequest(const FWorld &World, const EntityKey &Entity,
                          const ComponentType &Type) {
  return {World, Entity, Type};
}

/**
 * @brief Builds the component-membership request payload.
 * @signature inline FHasComponentRequest createHasComponentRequest(const FWorld &World, const EntityKey &Entity, const ComponentType &Type)
 *
 * User Story: As ECS query code, I need component membership payloads reused
 * across system and selector queries.
 */
inline FHasComponentRequest
createHasComponentRequest(const FWorld &World, const EntityKey &Entity,
                          const ComponentType &Type) {
  return {World, Entity, Type};
}

/**
 * @brief Builds the component-removal request payload.
 * @signature inline FRemoveComponentRequest createRemoveComponentRequest(FWorld World, const EntityKey &Entity, const ComponentType &Type)
 *
 * User Story: As ECS reducers, I need component removal payloads created from
 * a single factory.
 */
inline FRemoveComponentRequest
createRemoveComponentRequest(FWorld World, const EntityKey &Entity,
                             const ComponentType &Type) {
  return {World, Entity, Type};
}

/**
 * @brief Builds the tag-write request payload.
 * @signature inline FSetTagRequest createSetTagRequest(FWorld World, const EntityKey &Entity, const Tag &TagValue)
 *
 * User Story: As tag reducers, I need tag write payloads created through one
 * factory before world transitions.
 */
inline FSetTagRequest createSetTagRequest(FWorld World,
                                          const EntityKey &Entity,
                                          const Tag &TagValue) {
  return {World, Entity, TagValue};
}

/**
 * @brief Builds the tag-membership request payload.
 * @signature inline FHasTagRequest createHasTagRequest(const FWorld &World, const EntityKey &Entity, const Tag &TagValue)
 *
 * User Story: As ECS query code, I need tag predicates to share one request
 * factory.
 */
inline FHasTagRequest createHasTagRequest(const FWorld &World,
                                          const EntityKey &Entity,
                                          const Tag &TagValue) {
  return {World, Entity, TagValue};
}

/**
 * @brief Builds the resource-write request payload.
 * @signature inline FSetResourceRequest createSetResourceRequest(FWorld World, const ResourceName &Name, const FComponentValue &Value)
 *
 * User Story: As resource reducers, I need resource write payloads created
 * through one reusable factory.
 */
inline FSetResourceRequest
createSetResourceRequest(FWorld World, const ResourceName &Name,
                         const FComponentValue &Value) {
  return {World, Name, Value};
}

/**
 * @brief Builds the resource-read request payload.
 * @signature inline FGetResourceRequest createGetResourceRequest(const FWorld &World, const ResourceName &Name)
 *
 * User Story: As ECS readers, I need resource lookup payloads created through
 * one factory before Maybe composition.
 */
inline FGetResourceRequest createGetResourceRequest(const FWorld &World,
                                                    const ResourceName &Name) {
  return {World, Name};
}

/**
 * @brief Builds the event-push request payload.
 * @signature inline FPushEventRequest createPushEventRequest(FWorld World, const EventType &Type, const FComponentValue &Payload)
 *
 * User Story: As ECS event code, I need event queue writes created through one
 * reusable payload factory.
 */
inline FPushEventRequest createPushEventRequest(FWorld World,
                                                const EventType &Type,
                                                const FComponentValue &Payload) {
  return {World, Type, Payload};
}

/**
 * @brief Builds the event-read request payload.
 * @signature inline FReadEventsRequest createReadEventsRequest(const FWorld &World, const EventType &Type)
 *
 * User Story: As ECS systems, I need event reads created through one factory
 * before matching optional queues.
 */
inline FReadEventsRequest createReadEventsRequest(const FWorld &World,
                                                  const EventType &Type) {
  return {World, Type};
}

/**
 * @brief Builds the entity-domain write request payload.
 * @signature inline FSetEntityDomainRequest createSetEntityDomainRequest(FWorld World, const EntityKey &Entity, const DomainPathKey &Domain)
 *
 * User Story: As domain projection code, I need entity-domain payloads created
 * through one factory before ECS reducers apply them.
 */
inline FSetEntityDomainRequest
createSetEntityDomainRequest(FWorld World, const EntityKey &Entity,
                             const DomainPathKey &Domain) {
  return {World, Entity, Domain};
}

/**
 * @brief Builds the entity-domain membership request payload.
 * @signature inline FEntityInDomainRequest createEntityInDomainRequest(const FWorld &World, const EntityKey &Entity, const DomainPathKey &Domain)
 *
 * User Story: As ECS domain queries, I need membership predicates to reuse one
 * payload factory.
 */
inline FEntityInDomainRequest
createEntityInDomainRequest(const FWorld &World, const EntityKey &Entity,
                            const DomainPathKey &Domain) {
  return {World, Entity, Domain};
}

/**
 * @brief Builds the component-query request payload.
 * @signature inline FQueryComponentsRequest createQueryComponentsRequest(const FWorld &World, const TArray<ComponentType> &Types, const TArray<EntityKey> &Entities)
 *
 * User Story: As ECS system execution, I need component query payloads created
 * through one reusable factory.
 */
inline FQueryComponentsRequest
createQueryComponentsRequest(const FWorld &World,
                             const TArray<ComponentType> &Types,
                             const TArray<EntityKey> &Entities) {
  return {World, Types, Entities};
}

/**
 * @brief Builds the all-entities component-query request payload.
 * @signature inline FQueryEntitiesByComponentsRequest createQueryEntitiesByComponentsRequest(const FWorld &World, const TArray<ComponentType> &Types)
 *
 * User Story: As selectors, I need all-entity component queries to share one
 * request factory.
 */
inline FQueryEntitiesByComponentsRequest
createQueryEntitiesByComponentsRequest(const FWorld &World,
                                       const TArray<ComponentType> &Types) {
  return {World, Types};
}

/**
 * @brief Builds the tag-query request payload.
 * @signature inline FQueryEntitiesByTagRequest createQueryEntitiesByTagRequest(const FWorld &World, const Tag &TagValue)
 *
 * User Story: As selectors, I need tag queries to share one request factory.
 */
inline FQueryEntitiesByTagRequest
createQueryEntitiesByTagRequest(const FWorld &World, const Tag &TagValue) {
  return {World, TagValue};
}

/**
 * @brief Builds the domain-query request payload.
 * @signature inline FQueryEntitiesByDomainRequest createQueryEntitiesByDomainRequest(const FWorld &World, const DomainPathKey &Domain)
 *
 * User Story: As selectors, I need domain queries to share one request factory.
 */
inline FQueryEntitiesByDomainRequest
createQueryEntitiesByDomainRequest(const FWorld &World,
                                   const DomainPathKey &Domain) {
  return {World, Domain};
}

/**
 * @brief Builds the child-query request payload.
 * @signature inline FQueryChildrenRequest createQueryChildrenRequest(const FWorld &World, const EntityKey &Parent)
 *
 * User Story: As hierarchy selectors, I need child queries to share one
 * payload factory.
 */
inline FQueryChildrenRequest createQueryChildrenRequest(const FWorld &World,
                                                        const EntityKey &Parent) {
  return {World, Parent};
}

/**
 * @brief Builds the component-gathering request payload.
 * @signature inline FGatherComponentsRequest createGatherComponentsRequest(const FWorld &World, const EntityKey &Entity, const TArray<ComponentType> &Types)
 *
 * User Story: As ECS system execution, I need component gathering to share one
 * request factory before fold-based lookup.
 */
inline FGatherComponentsRequest
createGatherComponentsRequest(const FWorld &World, const EntityKey &Entity,
                          const TArray<ComponentType> &Types) {
  return {World, Entity, Types};
}

inline FWorld setEntityDomain(const FSetEntityDomainRequest &Request);

/**
 * @brief Creates a fresh ECS world with allocator and cross-domain registry.
 *
 * Architecture: The world is plain value state. RTK reducers may store and
 * replace it, but ECS never owns the runtime store or dispatch semantics.
 */
inline FWorld createWorld(const FDomainRegistry &Domains) {
  FWorld World;
  World.Allocator = createEntityAllocator();
  World.Domains = Domains;
  World.Generation = int64{};
  return World;
}

inline FWorld createWorld() { return createWorld(createDomainRegistry()); }

inline bool componentTableEquals(const ComponentTable &Left,
                                 const ComponentTable &Right) {
  return func::map_values_equal<EntityKey, FComponentValue>(
      Left, Right,
      [](const FComponentValue &LeftValue,
         const FComponentValue &RightValue) { return LeftValue == RightValue; });
}

inline bool componentStoreEquals(const ComponentStore &Left,
                                 const ComponentStore &Right) {
  return func::map_values_equal<ComponentType, ComponentTable>(
      Left, Right,
      [](const ComponentTable &LeftValue, const ComponentTable &RightValue) {
        return componentTableEquals(LeftValue, RightValue);
      });
}

inline bool tagStoreEquals(const TagStore &Left, const TagStore &Right) {
  return func::map_values_equal<EntityKey, TArray<Tag>>(
      Left, Right,
      [](const TArray<Tag> &LeftValue, const TArray<Tag> &RightValue) {
        return LeftValue == RightValue;
      });
}

inline bool resourceStoreEquals(const ResourceStore &Left,
                                const ResourceStore &Right) {
  return func::map_values_equal<ResourceName, FComponentValue>(
      Left, Right,
      [](const FComponentValue &LeftValue,
         const FComponentValue &RightValue) { return LeftValue == RightValue; });
}

inline bool relationshipStoreEquals(
    const TMap<EntityKey, FRelationship> &Left,
    const TMap<EntityKey, FRelationship> &Right) {
  return func::map_values_equal<EntityKey, FRelationship>(
      Left, Right,
      [](const FRelationship &LeftValue, const FRelationship &RightValue) {
        return LeftValue == RightValue;
      });
}

inline bool eventQueueEquals(const EventQueue &Left, const EventQueue &Right) {
  return func::map_values_equal<EventType, TArray<FComponentValue>>(
      Left, Right,
      [](const TArray<FComponentValue> &LeftValue,
         const TArray<FComponentValue> &RightValue) {
        return LeftValue == RightValue;
      });
}

inline bool entityDomainStoreEquals(
    const TMap<EntityKey, TArray<DomainPathKey>> &Left,
    const TMap<EntityKey, TArray<DomainPathKey>> &Right) {
  return func::map_values_equal<EntityKey, TArray<DomainPathKey>>(
      Left, Right,
      [](const TArray<DomainPathKey> &LeftValue,
         const TArray<DomainPathKey> &RightValue) {
        return LeftValue == RightValue;
      });
}

inline bool operator==(const FWorld &Left, const FWorld &Right) {
  return Left.Allocator == Right.Allocator &&
         componentStoreEquals(Left.Components, Right.Components) &&
         tagStoreEquals(Left.Tags, Right.Tags) &&
         resourceStoreEquals(Left.Resources, Right.Resources) &&
         eventQueueEquals(Left.Events, Right.Events) &&
         entityDomainStoreEquals(Left.EntityDomains, Right.EntityDomains) &&
         relationshipStoreEquals(Left.Relationships, Right.Relationships) &&
         Left.Domains == Right.Domains &&
         Left.DirtyEntities == Right.DirtyEntities &&
         Left.Generation == Right.Generation;
}

inline bool operator!=(const FWorld &Left, const FWorld &Right) {
  return !(Left == Right);
}

/**
 * @brief Records one dirty entity and increments the world generation.
 * @signature inline FWorld recordEntityDirty(const FRecordEntityDirtyRequest &Request)
 *
 * User Story: As a reducer-owned ECS transition, dirty tracking should be a
 * unary world transform that can be composed without hidden side effects.
 */
inline FWorld recordEntityDirty(const FRecordEntityDirtyRequest &Request) {
  FWorld World = Request.World;
  World.DirtyEntities.AddUnique(Request.Entity);
  ++World.Generation;
  return World;
}

/**
 * @brief Records multiple dirty entities through a world update fold.
 * @signature inline FWorld recordEntitiesDirty(const FRecordEntitiesDirtyRequest &Request)
 *
 * User Story: As relationship reducers, I need multi-entity dirty updates to
 * compose from the single-entity reducer rather than repeating nested calls.
 */
inline FWorld recordEntitiesDirty(const FRecordEntitiesDirtyRequest &Request) {
  return func::fold_array<EntityKey, FWorld>(
      Request.Entities, Request.World,
      [](const FWorld &World, const EntityKey &Entity) {
        return recordEntityDirty(createRecordEntityDirtyRequest(World, Entity));
      });
}

typedef std::function<FRelationship(const FRelationship &)>
    FRelationshipTransform;
typedef std::function<FRelationship(const EntityKey &, const FRelationship &)>
    FRelatedRelationshipTransform;

struct FRelationshipWrite {
  EntityKey Entity;
  FRelationshipTransform Transform;
};

enum class ERelationshipWriteKind {
  AssignParent,
  ClearParent,
  AppendChild,
  RemoveChild
};

struct FRelationshipWriteDeclaration {
  EntityKey Entity;
  ERelationshipWriteKind Kind;
  func::Maybe<EntityKey> RelatedEntity;
};

/**
 * @brief Assigns a parent on one relationship value.
 */
inline FRelationship assignRelationshipParent(const EntityKey &Parent,
                                              const FRelationship &Value) {
  FRelationship Next = Value;
  Next.Parent = func::just(Parent);
  return Next;
}

/**
 * @brief Clears the parent on one relationship value.
 */
inline FRelationship clearRelationshipParent(const FRelationship &Value) {
  FRelationship Next = Value;
  Next.Parent = func::nothing<EntityKey>();
  return Next;
}

/**
 * @brief Adds a child to one relationship value.
 */
inline FRelationship appendRelationshipChild(const EntityKey &Child,
                                             const FRelationship &Value) {
  FRelationship Next = Value;
  Next.Children.AddUnique(Child);
  return Next;
}

/**
 * @brief Removes a child from one relationship value.
 */
inline FRelationship removeRelationshipChildKey(const EntityKey &Child,
                                            const FRelationship &Value) {
  FRelationship Next = Value;
  Next.Children.Remove(Child);
  return Next;
}

inline EntityKey
relationshipWriteRelatedEntity(const FRelationshipWriteDeclaration &Declaration) {
  check(Declaration.RelatedEntity.hasValue);
  return Declaration.RelatedEntity.value;
}

inline FRelationshipTransform createRelatedRelationshipTransform(
    const FRelationshipWriteDeclaration &Declaration,
    FRelatedRelationshipTransform Transform) {
  const EntityKey RelatedEntity = relationshipWriteRelatedEntity(Declaration);
  return FRelationshipTransform(
      [RelatedEntity, Transform](const FRelationship &Relationship) {
        return Transform(RelatedEntity, Relationship);
      });
}

/**
 * @brief Expands one relationship write declaration into an ECS transform.
 */
inline FRelationshipTransform createRelationshipWriteTransform(
    const FRelationshipWriteDeclaration &Declaration) {
  const func::Maybe<FRelationshipTransform> Transform =
      func::multi_match<ERelationshipWriteKind, FRelationshipTransform>(
          Declaration.Kind,
          {func::when<ERelationshipWriteKind, FRelationshipTransform>(
               func::equals(ERelationshipWriteKind::AssignParent),
               [&Declaration](const ERelationshipWriteKind &) {
                 return createRelatedRelationshipTransform(
                     Declaration, assignRelationshipParent);
               }),
           func::when<ERelationshipWriteKind, FRelationshipTransform>(
               func::equals(ERelationshipWriteKind::ClearParent),
               [](const ERelationshipWriteKind &) {
                 return FRelationshipTransform(
                     [](const FRelationship &Relationship) {
                       return clearRelationshipParent(Relationship);
                     });
               }),
           func::when<ERelationshipWriteKind, FRelationshipTransform>(
               func::equals(ERelationshipWriteKind::AppendChild),
               [&Declaration](const ERelationshipWriteKind &) {
                 return createRelatedRelationshipTransform(
                     Declaration, appendRelationshipChild);
               }),
           func::when<ERelationshipWriteKind, FRelationshipTransform>(
               func::equals(ERelationshipWriteKind::RemoveChild),
               [&Declaration](const ERelationshipWriteKind &) {
                 return createRelatedRelationshipTransform(
                     Declaration, removeRelationshipChildKey);
               })});
  check(Transform.hasValue);
  return Transform.value;
}

/**
 * @brief Expands one relationship write declaration into the fold payload.
 */
inline FRelationshipWrite createRelationshipWrite(
    const FRelationshipWriteDeclaration &Declaration) {
  return {Declaration.Entity, createRelationshipWriteTransform(Declaration)};
}

/**
 * @brief Expands relationship write declarations through one composer.
 */
inline TArray<FRelationshipWrite> createRelationshipWrites(
    const TArray<FRelationshipWriteDeclaration> &Declarations) {
  return func::map_array<FRelationshipWriteDeclaration, FRelationshipWrite>(
      Declarations, createRelationshipWrite);
}

inline TArray<FRelationshipWrite> createRelationshipWrites(
    std::initializer_list<FRelationshipWriteDeclaration> Declarations) {
  return createRelationshipWrites(
      TArray<FRelationshipWriteDeclaration>(Declarations));
}

/**
 * @brief Writes one relationship row by upserting a default-or-present value.
 */
inline TMap<EntityKey, FRelationship>
writeRelationship(TMap<EntityKey, FRelationship> Relationships,
                  const FRelationshipWrite &Write) {
  return func::upsert_map_value<EntityKey, FRelationship>(
      Relationships, Write.Entity, createRelationship(), Write.Transform);
}

/**
 * @brief Removes one relationship row from the relationship table.
 */
inline TMap<EntityKey, FRelationship>
removeRelationship(TMap<EntityKey, FRelationship> Relationships,
                   const EntityKey &Entity) {
  Relationships.Remove(Entity);
  return Relationships;
}

/**
 * @brief Applies one relationship write by reading default-or-present first.
 */
inline FWorld applyRelationshipWrite(const FWorld &World,
                                     const FRelationshipWrite &Write) {
  FWorld Next = World;
  Next.Relationships = writeRelationship(Next.Relationships, Write);
  return Next;
}

/**
 * @brief Applies relationship writes through one world update fold.
 */
inline FWorld applyRelationshipWrites(const FWorld &World,
                                      const TArray<FRelationshipWrite> &Writes) {
  return func::fold_array<FRelationshipWrite, FWorld>(
      Writes, World, [](const FWorld &Acc, const FRelationshipWrite &Write) {
        return applyRelationshipWrite(Acc, Write);
      });
}

inline EntityKey relationshipWriteEntity(const FRelationshipWrite &Write) {
  return Write.Entity;
}

inline bool relationshipContainsChild(const EntityKey &Child,
                                      const FRelationship &Relationship) {
  return func::contains_value<EntityKey>(Relationship.Children, Child);
}

inline TArray<EntityKey> findRelationshipParentsForChild(
    const FWorld &World, const EntityKey &Child) {
  return func::filter_array<EntityKey>(
      func::map_keys<EntityKey, FRelationship>(World.Relationships),
      [&World, &Child](const EntityKey &Parent) {
        return relationshipContainsChild(
            Child,
            getRelationshipOrDefault(createGetRelationshipRequest(
                World.Relationships, Parent)));
      });
}

inline FRelationshipWriteDeclaration createRemoveRelationshipChildDeclaration(
    const EntityKey &Parent, const EntityKey &Child) {
  return {Parent, ERelationshipWriteKind::RemoveChild, func::just(Child)};
}

inline TArray<FRelationshipWriteDeclaration>
createRelationshipChildCleanupDeclarations(const FWorld &World,
                                           const EntityKey &Child) {
  return func::map_array<EntityKey, FRelationshipWriteDeclaration>(
      findRelationshipParentsForChild(World, Child),
      [&Child](const EntityKey &Parent) {
        return createRemoveRelationshipChildDeclaration(Parent, Child);
      });
}

/**
 * @brief Records every dirty entity touched by relationship writes.
 */
inline FWorld
recordRelationshipWriteDirtyEntities(const FWorld &World,
                             const TArray<FRelationshipWrite> &Writes) {
  return recordEntitiesDirty(createRecordEntitiesDirtyRequest(
      World,
      func::map_array<FRelationshipWrite, EntityKey>(Writes, relationshipWriteEntity)));
}

/**
 * @brief Applies relationship writes and records touched relationship entities.
 */
inline FWorld applyRelationshipWritesAndRecordDirty(
    const FWorld &World, const TArray<FRelationshipWrite> &Writes) {
  return recordRelationshipWriteDirtyEntities(
      applyRelationshipWrites(World, Writes), Writes);
}

/**
 * @brief Applies declared relationship writes and marks touched entities dirty.
 */
inline FWorld applyRelationshipWriteDeclarations(
    const FWorld &World,
    const TArray<FRelationshipWriteDeclaration> &Declarations) {
  return applyRelationshipWritesAndRecordDirty(
      World, createRelationshipWrites(Declarations));
}

inline FWorld applyRelationshipWriteDeclarations(
    const FWorld &World,
    std::initializer_list<FRelationshipWriteDeclaration> Declarations) {
  return applyRelationshipWriteDeclarations(
      World, TArray<FRelationshipWriteDeclaration>(Declarations));
}

/**
 * @brief Allocates and marks one entity in the supplied world.
 *
 * @return FSpawnedEntity carrying the next world, id, and entity key.
 */
inline FSpawnedEntity spawnEntity(FWorld World) {
  const FAllocatedEntity Allocated = allocateEntity(World.Allocator);
  World.Allocator = Allocated.Allocator;
  const EntityKey Entity = createEntityKey(Allocated.Entity);
  FSpawnedEntity Spawned;
  Spawned.World =
      recordEntityDirty(createRecordEntityDirtyRequest(World, Entity));
  Spawned.Id = Allocated.Entity;
  Spawned.Entity = Entity;
  return Spawned;
}

/**
 * @brief Allocates one entity and associates it with an ECS domain.
 * @signature inline FSpawnedEntity spawnEntityInDomain(const FSpawnEntityInDomainRequest &Request)
 *
 * User Story: As a spawn reducer, I need entity creation and domain wiring to
 * return one next world value from a request payload.
 */
inline FSpawnedEntity
spawnEntityInDomain(const FSpawnEntityInDomainRequest &Request) {
  const FSpawnedEntity Spawned = spawnEntity(Request.World);
  FSpawnedEntity WithDomain = Spawned;
  WithDomain.World = setEntityDomain(createSetEntityDomainRequest(
      Spawned.World, Spawned.Entity, Request.Domain));
  return WithDomain;
}

/**
 * @brief Builds a transform factory that removes one entity from component tables.
 * @signature inline FComponentTypeWorldTransformFactory removeEntityFromComponentType(const EntityKey &Entity)
 *
 * User Story: As lifecycle reducers, entity component cleanup should be a
 * curried factory that folds through component types without local loops.
 */
inline FComponentTypeWorldTransformFactory
removeEntityFromComponentType(const EntityKey &Entity) {
  return [Entity](const ComponentType &Type) {
    return [Entity, Type](const FWorld &World) {
      FWorld Next = World;
      ComponentTable Table = Next.Components.FindChecked(Type);
      Table.Remove(Entity);
      Next.Components.Add(Type, Table);
      return Next;
    };
  };
}

/**
 * @brief Builds a world transform that removes an entity from every component table.
 * @signature inline FWorldTransform removeEntityComponentIndexes(const EntityKey &Entity)
 *
 * User Story: As lifecycle reducers, component index cleanup should compose as
 * one unary world transform inside the despawn pipeline.
 */
inline FWorldTransform removeEntityComponentIndexes(const EntityKey &Entity) {
  return [Entity](const FWorld &World) {
    const FComponentTypeWorldTransformFactory RemoveFromType =
        removeEntityFromComponentType(Entity);
    return func::fold_array<ComponentType, FWorld>(
        func::map_keys<ComponentType, ComponentTable>(World.Components), World,
        [RemoveFromType](const FWorld &Acc, const ComponentType &Type) {
          return RemoveFromType(Type)(Acc);
        });
  };
}

/**
 * @brief Builds a world transform that removes direct entity-owned indexes.
 * @signature inline FWorldTransform removeEntityDirectIndexes(const EntityKey &Entity)
 *
 * User Story: As lifecycle reducers, direct tag/domain/relationship/dirty
 * cleanup should be one named unary transform.
 */
inline FWorldTransform removeEntityDirectIndexes(const EntityKey &Entity) {
  return [Entity](const FWorld &World) {
    FWorld Next = World;
    Next.Tags.Remove(Entity);
    Next.EntityDomains.Remove(Entity);
    Next.Relationships = removeRelationship(Next.Relationships, Entity);
    Next.DirtyEntities.Remove(Entity);
    return Next;
  };
}

/**
 * @brief Builds a world transform that removes an entity from parent child lists.
 * @signature inline FWorldTransform removeEntityRelationshipChildIndexes(const EntityKey &Entity)
 *
 * User Story: As hierarchy reducers, child-list cleanup should reuse
 * relationship writes so row mutation and dirty propagation stay centralized.
 */
inline FWorldTransform
removeEntityRelationshipChildIndexes(const EntityKey &Entity) {
  return [Entity](const FWorld &World) {
    return applyRelationshipWriteDeclarations(
        World, createRelationshipChildCleanupDeclarations(World, Entity));
  };
}

/**
 * @brief Removes an entity's component, tag, domain, and relationship data.
 * @signature inline FWorld removeEntity(const FRemoveEntityRequest &Request)
 *
 * User Story: As a despawn reducer, I need every entity-owned ECS index cleaned
 * through one world-in/world-out request.
 */
inline FWorld removeEntity(const FRemoveEntityRequest &Request) {
  const FWorld World = applyWorldTransformCatalog(
      Request.World,
      func::catalog(removeEntityComponentIndexes(Request.Entity),
                    removeEntityDirectIndexes(Request.Entity),
                    removeEntityRelationshipChildIndexes(Request.Entity)));
  return recordEntityDirty(
      createRecordEntityDirtyRequest(World, Request.Entity));
}

/**
 * @brief Frees an id and removes the matching entity data from the world.
 * @signature inline FWorld despawnEntity(const FDespawnEntityRequest &Request)
 *
 * User Story: As a lifecycle reducer, I need allocator and entity-index
 * cleanup to move through one typed request.
 */
inline FWorld despawnEntity(const FDespawnEntityRequest &Request) {
  FWorld World = Request.World;
  World.Allocator = freeEntityId(World.Allocator, Request.Id);
  return removeEntity(createRemoveEntityRequest(World, createEntityKey(Request.Id)));
}

/**
 * @brief Sets a parent relationship using immutable world-in/world-out data.
 * @signature inline FWorld setRelationshipParent(const FSetRelationshipParentRequest &Request)
 *
 * User Story: As relationship ECS code, I need parent/child writes to stay a
 * pure request transform and mark both touched entities.
 */
inline FWorld setRelationshipParent(const FSetRelationshipParentRequest &Request) {
  return applyRelationshipWriteDeclarations(
      Request.World,
      {{Request.Child, ERelationshipWriteKind::AssignParent,
        func::just(Request.Parent)},
       {Request.Parent, ERelationshipWriteKind::AppendChild,
        func::just(Request.Child)}});
}

/**
 * @brief Adds one child to a parent relationship.
 * @signature inline FWorld addRelationshipChild(const FAddRelationshipChildRequest &Request)
 *
 * User Story: As ECS relationship code, adding children should compose through
 * the same parent-setting primitive instead of duplicating write logic.
 */
inline FWorld addRelationshipChild(const FAddRelationshipChildRequest &Request) {
  return setRelationshipParent(createSetRelationshipParentRequest(Request.World, Request.Child,
                                          Request.Parent));
}

/**
 * @brief Removes one child from a parent relationship.
 * @signature inline FWorld removeRelationshipChild(const FRemoveRelationshipChildRequest &Request)
 *
 * User Story: As relationship ECS code, removing children should be a typed
 * unary transition over the world value.
 */
inline FWorld removeRelationshipChild(const FRemoveRelationshipChildRequest &Request) {
  return applyRelationshipWriteDeclarations(
      Request.World,
      {{Request.Parent, ERelationshipWriteKind::RemoveChild,
        func::just(Request.Child)},
       {Request.Child, ERelationshipWriteKind::ClearParent,
        func::nothing<EntityKey>()}});
}

/**
 * @brief Returns all known entities across domains, tags, and components.
 */
inline TArray<EntityKey> collectEntityKeys(const FWorld &World) {
  const TArray<EntityKey> DomainEntities =
      func::map_keys<EntityKey, TArray<DomainPathKey>>(World.EntityDomains);
  const TArray<EntityKey> TaggedEntities =
      func::map_keys<EntityKey, TArray<Tag>>(World.Tags);
  const TArray<ComponentType> ComponentTypes =
      func::map_keys<ComponentType, ComponentTable>(World.Components);
  const TArray<EntityKey> ComponentEntities =
      func::fold_array<ComponentType, TArray<EntityKey>>(
      ComponentTypes, TArray<EntityKey>(),
      [&World](const TArray<EntityKey> &Acc, const ComponentType &Type) {
        return func::match(
            func::find_map_value_ptr<ComponentType, ComponentTable>(World.Components,
                                                           Type),
            [&Acc](const ComponentTable *Table) {
              return func::append_map_keys<EntityKey, FComponentValue>(Acc, *Table);
            },
            [&Acc]() { return Acc; });
      });
  return func::unique_array<EntityKey>(func::concat_arrays<EntityKey>(
      {DomainEntities, TaggedEntities, ComponentEntities}));
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
  return recordEntityDirty(
      createRecordEntityDirtyRequest(World, Request.Entity));
}

/**
 * @brief Reads one component value from an entity.
 * @signature inline func::Maybe<FComponentValue> getComponent(const FGetComponentRequest &Request)
 *
 * User Story: As ECS system code, nested component-table lookups should
 * short-circuit through Maybe rather than branching around nullable pointers.
 */
inline func::Maybe<FComponentValue>
getComponent(const FGetComponentRequest &Request) {
  return func::mbind(
      func::find_map_value_ptr<ComponentType, ComponentTable>(
          Request.World.Components, Request.Type),
      [&Request](const ComponentTable *Table) {
        return func::find_map_value<EntityKey, FComponentValue>(*Table,
                                                        Request.Entity);
      });
}

/**
 * @brief Checks whether an entity currently has a component type.
 * @signature inline bool hasComponent(const FHasComponentRequest &Request)
 *
 * User Story: As ECS query code, component membership should reuse the Maybe
 * reader so read semantics stay centralized.
 */
inline bool hasComponent(const FHasComponentRequest &Request) {
  return func::is_just(getComponent(createGetComponentRequest(
      Request.World, Request.Entity, Request.Type)));
}

/**
 * @brief Builds a table-present branch for removing one component value.
 * @signature inline std::function<FWorld(const ComponentTable &)> removeComponentFromTable(const FRemoveComponentRequest &Request)
 *
 * User Story: As ECS reducers, component removal should isolate the present
 * table branch so Maybe matching replaces imperative nullable checks.
 */
inline std::function<FWorld(const ComponentTable &)>
removeComponentFromTable(const FRemoveComponentRequest &Request) {
  return [&Request](const ComponentTable &Table) {
    FWorld Next = Request.World;
    ComponentTable UpdatedTable = Table;
    UpdatedTable.Remove(Request.Entity);
    Next.Components.Add(Request.Type, UpdatedTable);
    return Next;
  };
}

/**
 * @brief Removes one component value from an entity and marks it dirty.
 * @signature inline FWorld removeComponent(const FRemoveComponentRequest &Request)
 *
 * User Story: As ECS reducers, component removal should remain a unary
 * world-in/world-out transform.
 */
inline FWorld removeComponent(const FRemoveComponentRequest &Request) {
  const FWorld World = func::match(
      func::find_map_value<ComponentType, ComponentTable>(Request.World.Components,
                                                 Request.Type),
      removeComponentFromTable(Request),
      [&Request]() { return Request.World; });
  return recordEntityDirty(
      createRecordEntityDirtyRequest(World, Request.Entity));
}

/**
 * @brief Adds one tag to one entity using a request payload.
 */
inline FWorld setTag(const FSetTagRequest &Request) {
  FWorld World = Request.World;
  World.Tags.FindOrAdd(Request.Entity).AddUnique(Request.TagValue);
  return recordEntityDirty(
      createRecordEntityDirtyRequest(World, Request.Entity));
}

/**
 * @brief Checks whether an entity currently has a tag.
 * @signature inline bool hasTag(const FHasTagRequest &Request)
 *
 * User Story: As ECS query code, tag membership should use Maybe lookup plus
 * reusable array predicates.
 */
inline bool hasTag(const FHasTagRequest &Request) {
  return func::map_array_contains<EntityKey, Tag>(Request.World.Tags, Request.Entity,
                                          Request.TagValue);
}

/**
 * @brief Sets one world resource using a request payload.
 */
inline FWorld setResource(const FSetResourceRequest &Request) {
  FWorld World = Request.World;
  World.Resources.Add(Request.Name, Request.Value);
  ++World.Generation;
  return World;
}

/**
 * @brief Reads one resource value from the world.
 * @signature inline func::Maybe<FComponentValue> getResource(const FGetResourceRequest &Request)
 *
 * User Story: As ECS side-effect orchestration code, resource reads should be
 * optional values instead of nullable pointer branches.
 */
inline func::Maybe<FComponentValue>
getResource(const FGetResourceRequest &Request) {
  return func::find_map_value<ResourceName, FComponentValue>(Request.World.Resources,
                                                    Request.Name);
}

/**
 * @brief Appends one ECS event payload to its event queue.
 */
inline FWorld pushEvent(const FPushEventRequest &Request) {
  FWorld World = Request.World;
  World.Events.FindOrAdd(Request.Type).Add(Request.Payload);
  return World;
}

/**
 * @brief Reads queued event payloads for one event type.
 * @signature inline TArray<FComponentValue> readEvents(const FReadEventsRequest &Request)
 *
 * User Story: As ECS systems code, event queues should materialize through a
 * Maybe match at the boundary.
 */
inline TArray<FComponentValue> readEvents(const FReadEventsRequest &Request) {
  return func::or_else(
      func::find_map_value<EventType, TArray<FComponentValue>>(Request.World.Events,
                                                       Request.Type),
      TArray<FComponentValue>());
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
  return recordEntityDirty(
      createRecordEntityDirtyRequest(World, Request.Entity));
}

/**
 * @brief Checks whether an entity belongs to a domain path.
 * @signature inline bool isEntityInDomain(const FEntityInDomainRequest &Request)
 *
 * User Story: As ECS domain queries, membership should be a Maybe lookup plus
 * an array predicate instead of borrowing sibling-domain logic.
 */
inline bool isEntityInDomain(const FEntityInDomainRequest &Request) {
  return func::map_array_contains<EntityKey, DomainPathKey>(
      Request.World.EntityDomains, Request.Entity, Request.Domain);
}

/**
 * @brief Filters entities that contain every requested component type.
 * @signature inline TArray<EntityKey> queryComponents(const FQueryComponentsRequest &Request)
 *
 * User Story: As ECS system execution, component filtering should compose from
 * reusable predicates and request-shaped readers.
 */
inline TArray<EntityKey> queryComponents(const FQueryComponentsRequest &Request) {
  return func::filter_array<EntityKey>(
      Request.Entities,
      [&Request](const EntityKey &Entity) {
        return func::all_array<ComponentType>(
            Request.Types,
            [&Request, &Entity](const ComponentType &Type) {
              return hasComponent(createHasComponentRequest(Request.World,
                                                            Entity, Type));
            });
      });
}

/**
 * @brief Queries all world entities by required components.
 * @signature inline TArray<EntityKey> queryEntitiesByComponents(const FQueryEntitiesByComponentsRequest &Request)
 *
 * User Story: As selectors or reducers query ECS state, component queries
 * should pass through one payload and remain view-neutral.
 */
inline TArray<EntityKey>
queryEntitiesByComponents(const FQueryEntitiesByComponentsRequest &Request) {
  return queryComponents(createQueryComponentsRequest(
      Request.World, Request.Types, collectEntityKeys(Request.World)));
}

/**
 * @brief Queries all world entities by tag.
 * @signature inline TArray<EntityKey> queryEntitiesByTag(const FQueryEntitiesByTagRequest &Request)
 *
 * User Story: As selectors derive entity lists, tag filtering should reuse the
 * same functional query primitives as systems.
 */
inline TArray<EntityKey>
queryEntitiesByTag(const FQueryEntitiesByTagRequest &Request) {
  const TArray<EntityKey> Entities = collectEntityKeys(Request.World);
  return func::filter_array<EntityKey>(
      Entities,
      [&Request](const EntityKey &Entity) {
        return hasTag(createHasTagRequest(Request.World, Entity,
                                          Request.TagValue));
      });
}

/**
 * @brief Queries all world entities by domain path.
 * @signature inline TArray<EntityKey> queryEntitiesByDomain(const FQueryEntitiesByDomainRequest &Request)
 *
 * User Story: As feature domains query ECS ownership, domain filtering should
 * import downward into neutral ECS predicates.
 */
inline TArray<EntityKey>
queryEntitiesByDomain(const FQueryEntitiesByDomainRequest &Request) {
  const TArray<EntityKey> Entities = collectEntityKeys(Request.World);
  return func::filter_array<EntityKey>(
      Entities,
      [&Request](const EntityKey &Entity) {
        return isEntityInDomain(createEntityInDomainRequest(
            Request.World, Entity, Request.Domain));
      });
}

/**
 * @brief Returns entities dirtied by recent world transforms.
 */
inline TArray<EntityKey> queryDirtyWorldEntities(const FWorld &World) {
  return World.DirtyEntities;
}

/**
 * @brief Returns child entity keys for a parent.
 * @signature inline TArray<EntityKey> queryChildren(const FQueryChildrenRequest &Request)
 *
 * User Story: As hierarchy selectors, child reads should materialize through
 * Maybe instead of nullable pointer branches.
 */
inline TArray<EntityKey> queryChildren(const FQueryChildrenRequest &Request) {
  return getRelationshipOrDefault(createGetRelationshipRequest(
             Request.World.Relationships, Request.Parent))
      .Children;
}

/**
 * @brief Gathers available component values for a system input entity.
 * @signature inline TMap<ComponentType, FComponentValue> gatherComponents(const FGatherComponentsRequest &Request)
 *
 * User Story: As ECS system execution, component gathering should fold over
 * requested types and let Maybe decide whether each component is present.
 */
inline TMap<ComponentType, FComponentValue>
gatherComponents(const FGatherComponentsRequest &Request) {
  return func::fold_array<ComponentType,
                          TMap<ComponentType, FComponentValue>>(
      Request.Types, TMap<ComponentType, FComponentValue>(),
      [&Request](const TMap<ComponentType, FComponentValue> &Acc,
                 const ComponentType &Type) {
        return func::match(
            getComponent(createGetComponentRequest(Request.World,
                                                   Request.Entity, Type)),
            [&Acc, &Type](const FComponentValue &Value) {
              TMap<ComponentType, FComponentValue> Next = Acc;
              Next.Add(Type, Value);
              return Next;
            },
            [&Acc]() { return Acc; });
      });
}

struct FSystemExecutionPayload {
  FWorld World;
  EntityKey Entity;
  TMap<ComponentType, FComponentValue> Components;
};

typedef TFunction<FWorld(const FSystemExecutionPayload &)> SystemFn;

struct FSystemDescriptor {
  TArray<ComponentType> RequiredComponents;
  TArray<Tag> RequiredTags;
  func::Maybe<DomainPathKey> Domain;
  SystemFn System;
};

struct FRunSystemRequest {
  FWorld World;
  TArray<ComponentType> Components;
  TArray<EntityKey> Entities;
  SystemFn System;
};

struct FRunSystemDescriptorRequest {
  FWorld World;
  TArray<EntityKey> Entities;
  FSystemDescriptor Descriptor;
};

/**
 * @brief Builds a system execution payload.
 * @signature inline FSystemExecutionPayload createSystemExecutionPayload(FWorld World, const EntityKey &Entity, const TMap<ComponentType, FComponentValue> &Components)
 *
 * User Story: As ECS systems, I need every system invocation to receive one
 * explicit payload rather than scalar parameters.
 */
inline FSystemExecutionPayload createSystemExecutionPayload(
    FWorld World, const EntityKey &Entity,
    const TMap<ComponentType, FComponentValue> &Components) {
  return {World, Entity, Components};
}

/**
 * @brief Builds a run-system request payload.
 * @signature inline FRunSystemRequest createRunSystemRequest(FWorld World, const TArray<ComponentType> &Components, const TArray<EntityKey> &Entities, SystemFn System)
 *
 * User Story: As ECS execution code, I need system runs to receive one payload
 * rather than scalar parameters.
 */
inline FRunSystemRequest
createRunSystemRequest(FWorld World, const TArray<ComponentType> &Components,
                       const TArray<EntityKey> &Entities, SystemFn System) {
  return {World, Components, Entities, System};
}

/**
 * @brief Builds a run-system-descriptor request payload.
 * @signature inline FRunSystemDescriptorRequest createRunSystemDescriptorRequest(FWorld World, const TArray<EntityKey> &Entities, const FSystemDescriptor &Descriptor)
 *
 * User Story: As ECS execution code, descriptor-based system runs should
 * receive one payload and compose matching internally.
 */
inline FRunSystemDescriptorRequest createRunSystemDescriptorRequest(
    FWorld World, const TArray<EntityKey> &Entities,
    const FSystemDescriptor &Descriptor) {
  return {World, Entities, Descriptor};
}

/**
 * @brief Creates a reusable predicate that checks all required tags.
 * @signature inline std::function<bool(const EntityKey &)> requireEntityTags(const FWorld &World, const TArray<Tag> &Tags)
 *
 * User Story: As ECS system matching, tag requirements should be expressed as
 * a function factory returning a unary predicate over entities.
 */
inline std::function<bool(const EntityKey &)>
requireEntityTags(const FWorld &World, const TArray<Tag> &Tags) {
  return [&World, Tags](const EntityKey &Entity) {
    return func::all_array<Tag>(
        Tags,
        [&World, &Entity](const Tag &TagValue) {
          return hasTag(createHasTagRequest(World, Entity, TagValue));
        });
  };
}

/**
 * @brief Creates a reusable predicate that checks an optional domain guard.
 * @signature inline std::function<bool(const EntityKey &)> requireEntityOptionalDomain(const FWorld &World, const func::Maybe<DomainPathKey> &Domain)
 *
 * User Story: As ECS system matching, optional domain guards should be
 * first-class unary predicates that compose with tag predicates.
 */
inline std::function<bool(const EntityKey &)>
requireEntityOptionalDomain(const FWorld &World,
                      const func::Maybe<DomainPathKey> &Domain) {
  return [&World, Domain](const EntityKey &Entity) {
    return func::match(
        Domain,
        [&World, &Entity](const DomainPathKey &DomainValue) {
          return isEntityInDomain(
              createEntityInDomainRequest(World, Entity, DomainValue));
        },
        []() { return true; });
  };
}

/**
 * @brief Creates a reusable predicate for descriptor tag/domain requirements.
 * @signature inline std::function<bool(const EntityKey &)> requireSystemDescriptor(const FWorld &World, const FSystemDescriptor &Descriptor)
 *
 * User Story: As ECS system execution, descriptor matching should be a
 * functional composition of smaller entity predicates.
 */
inline std::function<bool(const EntityKey &)>
requireSystemDescriptor(const FWorld &World,
                        const FSystemDescriptor &Descriptor) {
  return func::all_pass<EntityKey>(
      {requireEntityTags(World, Descriptor.RequiredTags),
       requireEntityOptionalDomain(World, Descriptor.Domain)});
}

/**
 * @brief Runs one pure ECS system over matching entities.
 * @signature inline FWorld runSystem(const FRunSystemRequest &Request)
 *
 * User Story: As ECS execution code, running a system should fold over matching
 * entities and invoke systems with a single execution payload.
 */
inline FWorld runSystem(const FRunSystemRequest &Request) {
  const TArray<EntityKey> Matching = queryComponents(
      createQueryComponentsRequest(Request.World, Request.Components,
                                   Request.Entities));
  return func::fold_array<EntityKey, FWorld>(
      Matching, Request.World,
      [&Request](const FWorld &World, const EntityKey &Entity) {
        return Request.System(createSystemExecutionPayload(
            World, Entity,
            gatherComponents(createGatherComponentsRequest(World, Entity,
                                                   Request.Components))));
      });
}

/**
 * @brief Runs a system descriptor over entities matching components, tags, and
 * optional domain.
 * @signature inline FWorld runSystemDescriptor(const FRunSystemDescriptorRequest &Request)
 *
 * User Story: As ECS execution code, descriptor execution should compose
 * component queries, descriptor guards, and system invocation through shared
 * request helpers.
 */
inline FWorld
runSystemDescriptor(const FRunSystemDescriptorRequest &Request) {
  const TArray<EntityKey> ComponentMatches = queryComponents(
      createQueryComponentsRequest(Request.World,
                                   Request.Descriptor.RequiredComponents,
                                   Request.Entities));
  const std::function<bool(const EntityKey &)> MatchesDescriptor =
      requireSystemDescriptor(Request.World, Request.Descriptor);
  const TArray<EntityKey> Matching =
      func::filter_array<EntityKey>(ComponentMatches, MatchesDescriptor);
  return runSystem(createRunSystemRequest(
      Request.World, Request.Descriptor.RequiredComponents, Matching,
      Request.Descriptor.System));
}

struct FValidateEntityDomainRequest {
  const FWorld &World;
  EntityKey Entity;
  DomainPathKey Domain;
};

struct FValidateSystemSpecRequest {
  const FDomainRegistry &Registry;
  FSystemSpec Spec;
};

/**
 * @brief Creates a reusable domain-exists validator for a registry.
 * @signature inline std::function<func::Either<FString, bool>(const DomainPathKey &)> requireDomain(const FDomainRegistry &Registry)
 *
 * User Story: As registry validation, domain validation should be a reusable
 * function factory over a registry.
 */
inline std::function<func::Either<FString, bool>(const DomainPathKey &)>
requireDomain(const FDomainRegistry &Registry) {
  return func::require_map_key<DomainPathKey, FDomainNode>(
      Registry.Nodes, [](const DomainPathKey &Domain) {
        return FString::Printf(TEXT("Missing ECS domain: %s"), *Domain);
      });
}

/**
 * @brief Creates a reusable component-schema validator for a registry.
 * @signature inline std::function<func::Either<FString, bool>(const ComponentType &)> requireComponentSchema(const FDomainRegistry &Registry)
 *
 * User Story: As registry validation, component schema validation should be a
 * reusable function factory over a registry.
 */
inline std::function<func::Either<FString, bool>(const ComponentType &)>
requireComponentSchema(const FDomainRegistry &Registry) {
  return func::require_map_key<ComponentType, FComponentSchema>(
      Registry.ComponentSchemas, [](const ComponentType &Type) {
        return FString::Printf(TEXT("Missing ECS component schema: %s"), *Type);
      });
}

/**
 * @brief Builds an entity-domain validation request payload.
 * @signature inline FValidateEntityDomainRequest createValidateEntityDomainRequest(const FWorld &World, const EntityKey &Entity, const DomainPathKey &Domain)
 *
 * User Story: As ECS validation, entity-domain validation should still use one
 * payload at the multi-input domain boundary.
 */
inline FValidateEntityDomainRequest
createValidateEntityDomainRequest(const FWorld &World, const EntityKey &Entity,
                                  const DomainPathKey &Domain) {
  return {World, Entity, Domain};
}

/**
 * @brief Builds a system-spec validation request payload.
 * @signature inline FValidateSystemSpecRequest createValidateSystemSpecRequest(const FDomainRegistry &Registry, const FSystemSpec &Spec)
 *
 * User Story: As registry validation, system spec validation remains one
 * payload at the public multi-input boundary.
 */
inline FValidateSystemSpecRequest
createValidateSystemSpecRequest(const FDomainRegistry &Registry,
                                const FSystemSpec &Spec) {
  return {Registry, Spec};
}

/**
 * @brief Validates entity membership in a domain.
 * @signature inline func::Either<FString, bool> validateEntityDomain(const FValidateEntityDomainRequest &Request)
 *
 * User Story: As ECS validation, domain membership failures should return
 * Either errors and reuse entity-domain predicates.
 */
inline func::Either<FString, bool>
validateEntityDomain(const FValidateEntityDomainRequest &Request) {
  return isEntityInDomain(createEntityInDomainRequest(
             Request.World, Request.Entity, Request.Domain))
             ? func::make_right<FString, bool>(true)
             : func::make_left<FString, bool>(FString::Printf(
                   TEXT("Entity %s is not in ECS domain %s"),
                   *Request.Entity, *Request.Domain));
}

/**
 * @brief Validates a system spec against registered domain/component contracts.
 * @signature inline func::Either<FString, bool> validateSystemSpec(const FValidateSystemSpecRequest &Request)
 *
 * User Story: As registry validation, system spec checks should compose domain
 * and component validation through ue_fp Either semantics.
 */
inline func::Either<FString, bool>
validateSystemSpec(const FValidateSystemSpecRequest &Request) {
  return func::ebind(
      requireDomain(Request.Registry)(Request.Spec.Domain),
      [&Request](const bool &) {
        return func::fold_either_array<FString, ComponentType, bool>(
            Request.Spec.RequiredComponents, true,
            [&Request](const bool &, const ComponentType &Type) {
              return requireComponentSchema(Request.Registry)(Type);
            });
      });
}

typedef func::ArgDispatcher<int32, FComponentValue, FString>
    FComponentValueFormatter;

struct FComponentValueFormatCase {
  EComponentValueKind Kind;
  std::function<FString(const FComponentValue &)> Format;
};

/**
 * @brief Converts a component value kind into a dispatcher key.
 * @signature inline int32 componentValueKindKey(EComponentValueKind Kind)
 *
 * User Story: As ECS inspection code, enum formatting should use a stable
 * dispatcher key that composes with ue_fp dispatch helpers.
 */
inline int32 componentValueKindKey(EComponentValueKind Kind) {
  return static_cast<int32>(Kind);
}

/**
 * @brief Creates a reusable formatter registration function.
 * @signature inline std::function<FComponentValueFormatter(FComponentValueFormatter)> registerComponentValueFormatter(EComponentValueKind Kind, std::function<FString(const FComponentValue &)> Format)
 *
 * User Story: As ECS inspection code, component formatter registration should
 * be a reusable function factory for pipeline composition.
 */
inline std::function<FComponentValueFormatter(FComponentValueFormatter)>
registerComponentValueFormatter(
    EComponentValueKind Kind,
    std::function<FString(const FComponentValue &)> Format) {
  return [Kind, Format](FComponentValueFormatter Dispatcher) {
    return func::arg_dispatcher_register<int32, FComponentValue, FString>(
        Dispatcher, componentValueKindKey(Kind), Format);
  };
}

/**
 * @brief Builds the formatter dispatcher from registered case declarations.
 */
inline FComponentValueFormatter createComponentValueFormatter(
    std::initializer_list<FComponentValueFormatCase> Cases) {
  const TArray<FComponentValueFormatCase> Declarations(Cases);
  return func::fold_array<FComponentValueFormatCase, FComponentValueFormatter>(
      Declarations,
      func::create_arg_dispatcher<int32, FComponentValue, FString>(),
      [](const FComponentValueFormatter &Dispatcher,
         const FComponentValueFormatCase &Case) {
        return registerComponentValueFormatter(Case.Kind,
                                               Case.Format)(Dispatcher);
      });
}

/**
 * @brief Builds the component value formatter through functional composition.
 * @signature inline FComponentValueFormatter createComponentValueFormatter()
 *
 * User Story: As ECS inspection code, component formatting should be a
 * dispatcher assembled from reusable registrations instead of a branch ladder.
 */
inline FComponentValueFormatter createComponentValueFormatter() {
  return createComponentValueFormatter(
      {{EComponentValueKind::None,
        [](const FComponentValue &) { return FString(TEXT("None")); }},
       {EComponentValueKind::Bool,
        [](const FComponentValue &Value) {
          return Value.BoolValue ? FString(TEXT("true"))
                                 : FString(TEXT("false"));
        }},
       {EComponentValueKind::Int,
        [](const FComponentValue &Value) {
          return FString::Printf(TEXT("%lld"), Value.IntValue);
        }},
       {EComponentValueKind::Float,
        [](const FComponentValue &Value) {
          return FString::SanitizeFloat(Value.FloatValue);
        }},
       {EComponentValueKind::Text,
        [](const FComponentValue &Value) { return Value.TextValue; }},
       {EComponentValueKind::Vec2,
        [](const FComponentValue &Value) {
          return Value.Vec2Value.ToString();
        }},
       {EComponentValueKind::Vec3,
        [](const FComponentValue &Value) {
          return Value.Vec3Value.ToString();
        }},
       {EComponentValueKind::Map,
        [](const FComponentValue &Value) {
          return FString::Printf(TEXT("Map(len=%d)"), Value.MapValue.Num());
        }},
       {EComponentValueKind::List,
        [](const FComponentValue &Value) {
          return FString::Printf(TEXT("List(len=%d)"),
                                 Value.ListValue.Num());
        }}});
}

/**
 * @brief Returns the lazily initialized component value formatter.
 * @signature inline const FComponentValueFormatter &componentValueFormatter()
 *
 * User Story: As ECS inspection code, formatter registration should be built
 * once and reused by every inspection call.
 */
inline const FComponentValueFormatter &componentValueFormatter() {
  static const func::Lazy<FComponentValueFormatter> Formatter =
      func::lazy([]() { return createComponentValueFormatter(); });
  return func::eval(Formatter);
}

/**
 * @brief Formats a component value for ECS inspection output.
 * @signature inline FString componentValueToString(const FComponentValue &Value)
 *
 * User Story: As ECS inspection code, component formatting should dispatch
 * through a reusable functional table.
 */
inline FString componentValueToString(const FComponentValue &Value) {
  const FComponentValueFormatter &Formatter = componentValueFormatter();
  const int32 Key = componentValueKindKey(Value.Kind);
  const func::Maybe<FString> Formatted =
      func::arg_dispatcher_dispatch_maybe<int32, FComponentValue, FString>(
      {&Formatter, &Key, &Value});
  checkf(Formatted.hasValue, TEXT("Unhandled ECS component value kind: %d"),
         Key);
  return Formatted.value;
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
  Inspection.EntityCount = collectEntityKeys(World).Num();
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

typedef std::function<FEntityInspection(const FEntityInspection &)>
    FEntityInspectionTransform;
typedef std::function<FEntityInspectionTransform(const ComponentType &)>
    FComponentInspectionTransformFactory;
typedef std::function<FComponentInspectionTransformFactory(const EntityKey &)>
    FEntityComponentInspectionFactory;
typedef std::function<TArray<Tag>(const EntityKey &)> FTagInspectionSelector;
typedef std::function<TArray<DomainPathKey>(const EntityKey &)>
    FDomainInspectionSelector;

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
 * @brief Creates an empty inspection record for one entity.
 * @signature inline FEntityInspection createEntityInspection(const EntityKey &Entity)
 *
 * User Story: As inspection code, I need the display record initialized in one
 * reusable pure constructor before selectors add derived values.
 */
inline FEntityInspection createEntityInspection(const EntityKey &Entity) {
  FEntityInspection Inspection;
  Inspection.Entity = Entity;
  return Inspection;
}

/**
 * @brief Builds a selector for entity tags used by inspection.
 * @signature inline FTagInspectionSelector selectEntityTagsForInspection(const FWorld &World)
 *
 * User Story: As display inspection code, tag lookup should be a unary selector
 * over entity keys instead of nullable pointer logic.
 */
inline FTagInspectionSelector selectEntityTagsForInspection(const FWorld &World) {
  return [&World](const EntityKey &Entity) {
    return func::match(
        func::find_map_value<EntityKey, TArray<Tag>>(World.Tags, Entity),
        [](const TArray<Tag> &Tags) { return Tags; },
        []() { return TArray<Tag>(); });
  };
}

/**
 * @brief Builds a selector for entity domains used by inspection.
 * @signature inline FDomainInspectionSelector selectEntityDomainsForInspection(const FWorld &World)
 *
 * User Story: As display inspection code, domain lookup should be a unary
 * selector over entity keys instead of nullable pointer logic.
 */
inline FDomainInspectionSelector selectEntityDomainsForInspection(const FWorld &World) {
  return [&World](const EntityKey &Entity) {
    return func::match(
        func::find_map_value<EntityKey, TArray<DomainPathKey>>(World.EntityDomains,
                                                      Entity),
        [](const TArray<DomainPathKey> &Domains) { return Domains; },
        []() { return TArray<DomainPathKey>(); });
  };
}

/**
 * @brief Builds curried component inspection transforms for one world.
 * @signature inline FEntityComponentInspectionFactory selectEntityComponentsForInspection(const FWorld &World)
 *
 * User Story: As ECS inspection code, component display data should fold
 * through curried transforms instead of component table loops.
 */
inline FEntityComponentInspectionFactory
selectEntityComponentsForInspection(const FWorld &World) {
  return [&World](const EntityKey &Entity) {
    return [&World, Entity](const ComponentType &Type) {
      return [&World, Entity, Type](const FEntityInspection &Inspection) {
        return func::match(
            getComponent(createGetComponentRequest(World, Entity, Type)),
            [&Type, &Inspection](const FComponentValue &Value) {
              FEntityInspection Next = Inspection;
              Next.Components.Add(Type, Value);
              return Next;
            },
            [&Inspection]() { return Inspection; });
      };
    };
  };
}

/**
 * @brief Inspects one entity's tags, domains, and component values.
 * @signature inline FEntityInspection inspectEntity(const FWorld &World, const EntityKey &Entity)
 *
 * User Story: As display code, entity inspection should derive read-only
 * projection data from selectors and ECS lookups without owning logic.
 */
inline FEntityInspection inspectEntity(const FWorld &World, const EntityKey &Entity) {
  FEntityInspection Inspection = createEntityInspection(Entity);
  Inspection.Tags = selectEntityTagsForInspection(World)(Entity);
  Inspection.Domains = selectEntityDomainsForInspection(World)(Entity);
  const FComponentInspectionTransformFactory AddComponent =
      selectEntityComponentsForInspection(World)(Entity);
  return func::fold_array<ComponentType, FEntityInspection>(
      func::map_keys<ComponentType, ComponentTable>(World.Components),
      Inspection,
      [AddComponent](const FEntityInspection &Acc,
                     const ComponentType &Type) {
        return AddComponent(Type)(Acc);
      });
}

} // namespace ecs

#endif // FORBOC_AI_GAME_CORE_ECS_HPP
