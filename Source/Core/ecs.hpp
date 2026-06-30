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
 * - When only nouns change, keep the nouns in catalogs and fold through one
 *   ECS transform. Paired catalogs cover selector/projector and descriptor/run
 *   relationships without feature-local recursive families.
 * - Do not replace every `if` with ternary chains. Domain alternatives should
 *   use `func::match` for Maybe/Either/result values, dispatcher or
 *   `multi_match` case tables for enum/string routing, and ECS
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

/**
 * @brief Folds a TArray with a unary request payload and recursive functional step.
 * @signature template <typename Item, typename Accumulator> inline Accumulator foldArray(Accumulator Acc, const TArray<Item> &Items, std::function<Accumulator(const Accumulator &, const Item &)> Step, int32 Index = 0)
 *
 * User Story: As ECS core code, I need reusable value folding so world,
 * registry, and inspection transforms compound through functional_core
 * composition instead of bespoke loops.
 */
template <typename Item, typename Accumulator>
inline Accumulator foldArray(
    Accumulator Acc, const TArray<Item> &Items,
    std::function<Accumulator(const Accumulator &, const Item &)> Step,
    int32 Index = 0) {
  return Index >= Items.Num()
             ? Acc
             : foldArray<Item, Accumulator>(
                   Step(Acc, Items[Index]), Items, Step, Index + 1);
}

/**
 * @brief Runs one effect for each array item through the shared fold shape.
 * @signature template <typename Item> inline void forEachArray(const TArray<Item> &Items, std::function<void(const Item &)> Effect)
 *
 * User Story: As adapter code at IO boundaries, side-effect iteration should
 * still use one neutral traversal primitive instead of local loops.
 */
template <typename Item>
inline void forEachArray(const TArray<Item> &Items,
                         std::function<void(const Item &)> Effect) {
  foldArray<Item, int32>(
      0, Items,
      [Effect](const int32 &Count, const Item &ItemValue) {
        Effect(ItemValue);
        return Count + 1;
      });
}

/**
 * @brief Returns true when every item satisfies a predicate.
 * @signature template <typename Item> inline bool allArray(const TArray<Item> &Items, std::function<bool(const Item &)> Predicate, int32 Index = 0)
 *
 * User Story: As ECS validation code, I need reusable universal checks that
 * compose with Maybe/Either flows instead of hand-written loops.
 */
template <typename Item>
inline bool allArray(const TArray<Item> &Items,
                     std::function<bool(const Item &)> Predicate,
                     int32 Index = 0) {
  return Index >= Items.Num()
             ? true
             : Predicate(Items[Index]) &&
                   allArray<Item>(Items, Predicate, Index + 1);
}

/**
 * @brief Returns true when any item satisfies a predicate.
 * @signature template <typename Item> inline bool anyArray(const TArray<Item> &Items, std::function<bool(const Item &)> Predicate, int32 Index = 0)
 *
 * User Story: As ECS query code, I need reusable existential checks for tags,
 * domains, and component membership without repeated imperative branches.
 */
template <typename Item>
inline bool anyArray(const TArray<Item> &Items,
                     std::function<bool(const Item &)> Predicate,
                     int32 Index = 0) {
  return Index >= Items.Num()
             ? false
             : Predicate(Items[Index]) ||
                   anyArray<Item>(Items, Predicate, Index + 1);
}

/**
 * @brief Returns true when an array contains one value.
 * @signature template <typename Item> inline bool containsValue(const TArray<Item> &Values, const Item &Value)
 *
 * User Story: As ECS predicates, tag/domain/component checks should share one
 * membership primitive instead of repeating equivalent lambdas.
 */
template <typename Item>
inline bool containsValue(const TArray<Item> &Values, const Item &Value) {
  return anyArray<Item>(
      Values, [&Value](const Item &Candidate) { return Candidate == Value; });
}

/**
 * @brief Finds the first item that satisfies a predicate.
 * @signature template <typename Item> inline func::Maybe<Item> findArray(const TArray<Item> &Items, std::function<bool(const Item &)> Predicate, int32 Index = 0)
 *
 * User Story: As reducers and selectors, optional array lookups should share
 * one Maybe-producing primitive instead of local recursive scan families.
 */
template <typename Item>
inline func::Maybe<Item> findArray(const TArray<Item> &Items,
                                   std::function<bool(const Item &)> Predicate,
                                   int32 Index = 0) {
  return Index >= Items.Num()
             ? func::nothing<Item>()
             : Predicate(Items[Index])
                   ? func::just<Item>(Items[Index])
                   : findArray<Item>(Items, Predicate, Index + 1);
}

/**
 * @brief Filters a TArray through a recursive value-returning request.
 * @signature template <typename Item> inline TArray<Item> filterArray(const TArray<Item> &Items, std::function<bool(const Item &)> Predicate, TArray<Item> Acc = TArray<Item>(), int32 Index = 0)
 *
 * User Story: As ECS query code, entity lists should narrow through reusable
 * functional composition rather than ad hoc view- or system-local loops.
 */
template <typename Item>
inline TArray<Item> filterArray(const TArray<Item> &Items,
                                std::function<bool(const Item &)> Predicate,
                                TArray<Item> Acc = TArray<Item>(),
                                int32 Index = 0) {
  return Index >= Items.Num()
             ? Acc
             : filterArray<Item>(
                   Items, Predicate,
                   Predicate(Items[Index]) ? appendValue<Item>(Acc, Items[Index])
                                           : Acc,
                   Index + 1);
}

/**
 * @brief Appends one item to a value array.
 * @signature template <typename Item> inline TArray<Item> appendValue(TArray<Item> Values, const Item &Value)
 *
 * User Story: As recursive ECS helpers, I need a small unary append primitive
 * so folds and filters can compound without mutable caller-owned arrays.
 */
template <typename Item>
inline TArray<Item> appendValue(TArray<Item> Values, const Item &Value) {
  Values.Add(Value);
  return Values;
}

/**
 * @brief Appends one item only if it is not already present.
 * @signature template <typename Item> inline TArray<Item> appendUniqueValue(TArray<Item> Values, const Item &Value)
 *
 * User Story: As ECS indexing code, I need set-like array updates that remain
 * value-returning and fold-friendly.
 */
template <typename Item>
inline TArray<Item> appendUniqueValue(TArray<Item> Values, const Item &Value) {
  Values.AddUnique(Value);
  return Values;
}

/**
 * @brief Maps a TArray through one reusable unary transform.
 * @signature template <typename Source, typename Output> inline TArray<Output> mapArray(const TArray<Source> &Items, std::function<Output(const Source &)> MapValue)
 *
 * User Story: As ECS projection code, repeated source-to-view transforms should
 * read as one neutral fold instead of request/factory boilerplate per domain.
 */
template <typename Source, typename Output>
inline TArray<Output>
mapArray(const TArray<Source> &Items,
         std::function<Output(const Source &)> MapValue) {
  return foldArray<Source, TArray<Output>>(
      TArray<Output>(), Items,
      [MapValue](const TArray<Output> &Acc, const Source &Item) {
        return appendValue<Output>(Acc, MapValue(Item));
      });
}

/**
 * @brief Filters and maps a TArray through reusable unary predicates.
 * @signature template <typename Source, typename Output> inline TArray<Output> filterMapArray(const TArray<Source> &Items, std::function<bool(const Source &)> Keep, std::function<Output(const Source &)> MapValue)
 *
 * User Story: As ECS query code, selection and projection should compose
 * through one neutral fold instead of repeated sibling-domain helper code.
 */
template <typename Source, typename Output>
inline TArray<Output>
filterMapArray(const TArray<Source> &Items,
               std::function<bool(const Source &)> Keep,
               std::function<Output(const Source &)> MapValue) {
  return foldArray<Source, TArray<Output>>(
      TArray<Output>(), Items,
      [Keep, MapValue](const TArray<Output> &Acc, const Source &Item) {
        return Keep(Item) ? appendValue<Output>(Acc, MapValue(Item)) : Acc;
      });
}

/**
 * @brief Maps a TArray through a Maybe-producing transform and fails as one unit.
 * @signature template <typename Source, typename Output> inline func::Maybe<TArray<Output>> traverseMaybeArray(const TArray<Source> &Items, std::function<func::Maybe<Output>(const Source &)> MapValue)
 *
 * User Story: As data adapters, validated arrays should use one all-or-nothing
 * traversal instead of feature-local recursive parsing families.
 */
template <typename Source, typename Output>
inline func::Maybe<TArray<Output>>
traverseMaybeArray(const TArray<Source> &Items,
                   std::function<func::Maybe<Output>(const Source &)>
                       MapValue) {
  return foldArray<Source, func::Maybe<TArray<Output>>>(
      func::just<TArray<Output>>(TArray<Output>()), Items,
      [MapValue](const func::Maybe<TArray<Output>> &Acc,
                 const Source &Item) {
        return func::match(
            Acc,
            [&Item, MapValue](const TArray<Output> &Values) {
              return func::match(
                  MapValue(Item),
                  [&Values](const Output &Value) {
                    return func::just<TArray<Output>>(
                        appendValue<Output>(Values, Value));
                  },
                  []() { return func::nothing<TArray<Output>>(); });
            },
            []() { return func::nothing<TArray<Output>>(); });
      });
}

/**
 * @brief Appends every item from one array to another.
 * @signature template <typename Item> inline TArray<Item> appendValues(TArray<Item> Values, const TArray<Item> &AdditionalValues)
 *
 * User Story: As ECS query code, I need reusable array concatenation so entity
 * discovery can be expressed as folds.
 */
template <typename Item>
inline TArray<Item> appendValues(TArray<Item> Values,
                                 const TArray<Item> &AdditionalValues) {
  Values.Append(AdditionalValues);
  return Values;
}

/**
 * @brief Concatenates arrays through the shared fold primitive.
 * @signature template <typename Item> inline TArray<Item> concatArrays(const TArray<TArray<Item>> &Arrays)
 *
 * User Story: As ECS entity discovery, repeated entity sources should combine
 * through one reusable fold instead of repeated append calls.
 */
template <typename Item>
inline TArray<Item> concatArrays(const TArray<TArray<Item>> &Arrays) {
  return foldArray<TArray<Item>, TArray<Item>>(
      TArray<Item>(), Arrays,
      [](const TArray<Item> &Acc, const TArray<Item> &Values) {
        return appendValues<Item>(Acc, Values);
      });
}

/**
 * @brief Returns values with duplicates removed while preserving first-seen order.
 * @signature template <typename Item> inline TArray<Item> uniqueArray(const TArray<Item> &Values)
 *
 * User Story: As ECS indexing code, uniqueness passes should share one
 * reusable fold over AddUnique semantics.
 */
template <typename Item>
inline TArray<Item> uniqueArray(const TArray<Item> &Values) {
  return foldArray<Item, TArray<Item>>(
      TArray<Item>(), Values,
      [](const TArray<Item> &Acc, const Item &Value) {
        return appendUniqueValue<Item>(Acc, Value);
      });
}

/**
 * @brief Builds an index array from zero to Count - 1 through recursion.
 * @signature inline TArray<int32> indexRange(int32 Count, int32 Index = 0, TArray<int32> Acc = TArray<int32>())
 *
 * User Story: As ECS list code, I need index-aware checks without imperative
 * counter loops.
 */
inline TArray<int32> indexRange(int32 Count, int32 Index = 0,
                                TArray<int32> Acc = TArray<int32>()) {
  return Index >= Count
             ? Acc
             : indexRange(Count, Index + 1, appendValue<int32>(Acc, Index));
}

/**
 * @brief Folds integer indexes from zero to Count - 1.
 * @signature template <typename Accumulator> inline Accumulator foldIndexRange(int32 Count, Accumulator Acc, std::function<Accumulator(const Accumulator &, int32)> Step, int32 Index = 0)
 *
 * User Story: As grid and generated-data code, index traversal should be one
 * ECS primitive instead of local row/column recursion or loops.
 */
template <typename Accumulator>
inline Accumulator foldIndexRange(
    int32 Count, Accumulator Acc,
    std::function<Accumulator(const Accumulator &, int32)> Step,
    int32 Index = 0) {
  return Index >= Count
             ? Acc
             : foldIndexRange<Accumulator>(
                   Count, Step(Acc, Index), Step, Index + 1);
}

/**
 * @brief Maps integer indexes from zero to Count - 1.
 * @signature template <typename Output> inline TArray<Output> mapIndexRange(int32 Count, std::function<Output(int32)> MapValue)
 *
 * User Story: As generated feature data, index-based outputs should use the
 * same map shape as authored arrays.
 */
template <typename Output>
inline TArray<Output> mapIndexRange(int32 Count,
                                    std::function<Output(int32)> MapValue) {
  return foldIndexRange<TArray<Output>>(
      Count, TArray<Output>(),
      [MapValue](const TArray<Output> &Acc, int32 Index) {
        return appendValue<Output>(Acc, MapValue(Index));
      });
}

/**
 * @brief Maps a TArray through an indexed Maybe-producing transform.
 * @signature template <typename Source, typename Output> inline func::Maybe<TArray<Output>> traverseMaybeArrayWithIndex(const TArray<Source> &Items, std::function<func::Maybe<Output>(const Source &, int32)> MapValue)
 *
 * User Story: As data adapters, indexed validation should share one traversal
 * shape while still reporting the authored array position that failed.
 */
template <typename Source, typename Output>
inline func::Maybe<TArray<Output>>
traverseMaybeArrayWithIndex(
    const TArray<Source> &Items,
    std::function<func::Maybe<Output>(const Source &, int32)> MapValue) {
  return foldIndexRange<func::Maybe<TArray<Output>>>(
      Items.Num(), func::just<TArray<Output>>(TArray<Output>()),
      [&Items, MapValue](const func::Maybe<TArray<Output>> &Acc,
                         int32 Index) {
        return func::match(
            Acc,
            [&Items, MapValue, Index](const TArray<Output> &Values) {
              return func::match(
                  MapValue(Items[Index], Index),
                  [&Values](const Output &Value) {
                    return func::just<TArray<Output>>(
                        appendValue<Output>(Values, Value));
                  },
                  []() { return func::nothing<TArray<Output>>(); });
            },
            []() { return func::nothing<TArray<Output>>(); });
      });
}

struct FGridIndex {
  int32 Row;
  int32 Column;
};

/**
 * @brief Folds a two-dimensional row/column range.
 * @signature template <typename Accumulator> inline Accumulator foldGridRange(int32 Rows, int32 Columns, Accumulator Acc, std::function<Accumulator(const Accumulator &, const FGridIndex &)> Step)
 *
 * User Story: As terrain and texture code, grid traversal should be one ECS
 * primitive instead of separate row and column families per feature.
 */
template <typename Accumulator>
inline Accumulator foldGridRange(
    int32 Rows, int32 Columns, Accumulator Acc,
    std::function<Accumulator(const Accumulator &, const FGridIndex &)> Step) {
  return foldIndexRange<Accumulator>(
      Rows, Acc,
      [Columns, Step](const Accumulator &RowAcc, int32 Row) {
        return foldIndexRange<Accumulator>(
            Columns, RowAcc,
            [Row, Step](const Accumulator &ColumnAcc, int32 Column) {
              return Step(ColumnAcc, FGridIndex{Row, Column});
            });
      });
}

/**
 * @brief Maps a two-dimensional row/column range.
 * @signature template <typename Output> inline TArray<Output> mapGridRange(int32 Rows, int32 Columns, std::function<Output(const FGridIndex &)> MapValue)
 *
 * User Story: As generated grid features, per-cell values should use one
 * neutral map shape across terrain, rendering, and authored data.
 */
template <typename Output>
inline TArray<Output>
mapGridRange(int32 Rows, int32 Columns,
             std::function<Output(const FGridIndex &)> MapValue) {
  return foldGridRange<TArray<Output>>(
      Rows, Columns, TArray<Output>(),
      [MapValue](const TArray<Output> &Acc, const FGridIndex &Index) {
        return appendValue<Output>(Acc, MapValue(Index));
      });
}

/**
 * @brief Extracts all keys from a TMap as a value array.
 * @signature template <typename Key, typename Value> inline TArray<Key> mapKeys(const TMap<Key, Value> &Values)
 *
 * User Story: As ECS inspection and equality code, I need map traversal to
 * start from value arrays so recursive array folds can do the work.
 */
template <typename Key, typename Value>
inline TArray<Key> mapKeys(const TMap<Key, Value> &Values) {
  TArray<Key> Keys;
  Values.GetKeys(Keys);
  return Keys;
}

/**
 * @brief Appends all keys from a map to an existing array.
 * @signature template <typename Key, typename Value> inline TArray<Key> appendMapKeys(TArray<Key> Values, const TMap<Key, Value> &Map)
 *
 * User Story: As ECS entity discovery, component-table keys should append
 * through one composable primitive shared by all entity indexes.
 */
template <typename Key, typename Value>
inline TArray<Key> appendMapKeys(TArray<Key> Values,
                                 const TMap<Key, Value> &Map) {
  return appendValues<Key>(Values, mapKeys<Key, Value>(Map));
}

/**
 * @brief Lifts a TMap lookup into Maybe without copying the found value.
 * @signature template <typename Key, typename Value> inline func::Maybe<const Value *> findMapValuePtr(const TMap<Key, Value> &Values, const Key &KeyValue)
 *
 * User Story: As ECS lookup code, I need pointer-returning Maybe values so
 * nested lookups compose efficiently through mbind/fmap without duplicating
 * large component tables.
 */
template <typename Key, typename Value>
inline func::Maybe<const Value *>
findMapValuePtr(const TMap<Key, Value> &Values, const Key &KeyValue) {
  const Value *ValuePtr = Values.Find(KeyValue);
  return ValuePtr ? func::just<const Value *>(ValuePtr)
                  : func::nothing<const Value *>();
}

/**
 * @brief Lifts a TMap lookup into Maybe by copying the found value.
 * @signature template <typename Key, typename Value> inline func::Maybe<Value> findMapValue(const TMap<Key, Value> &Values, const Key &KeyValue)
 *
 * User Story: As ECS read boundaries, small value reads should share one
 * Maybe-producing copied lookup instead of repeating pointer matches.
 */
template <typename Key, typename Value>
inline func::Maybe<Value>
findMapValue(const TMap<Key, Value> &Values, const Key &KeyValue) {
  return func::match(
      findMapValuePtr<Key, Value>(Values, KeyValue),
      [](const Value *ValuePtr) { return func::just(*ValuePtr); },
      []() { return func::nothing<Value>(); });
}

/**
 * @brief Checks whether a keyed array value contains an item.
 * @signature template <typename Key, typename Item> inline bool mapArrayContains(const TMap<Key, TArray<Item>> &Values, const Key &KeyValue, const Item &Value)
 *
 * User Story: As ECS tag/domain predicates, map-of-array membership should be
 * one reusable Maybe + contains composition.
 */
template <typename Key, typename Item>
inline bool
mapArrayContains(const TMap<Key, TArray<Item>> &Values, const Key &KeyValue,
                 const Item &Value) {
  return func::match(
      findMapValuePtr<Key, TArray<Item>>(Values, KeyValue),
      [&Value](const TArray<Item> *FoundValues) {
        return containsValue<Item>(*FoundValues, Value);
      },
      []() { return false; });
}

/**
 * @brief Compares two TMaps by key set and value comparator through reusable folds.
 * @signature template <typename Key, typename Value> inline bool mapValuesEqual(const TMap<Key, Value> &Left, const TMap<Key, Value> &Right, std::function<bool(const Value &, const Value &)> Equals)
 *
 * User Story: As ECS equality code, component stores, resources, events, and
 * relationships should share one composable map comparison primitive.
 */
template <typename Key, typename Value>
inline bool mapValuesEqual(const TMap<Key, Value> &Left,
                           const TMap<Key, Value> &Right,
                           std::function<bool(const Value &, const Value &)>
                               Equals) {
  const TArray<Key> Keys = mapKeys<Key, Value>(Left);
  return Left.Num() == Right.Num() &&
         allArray<Key>(
             Keys,
             [&Left, &Right, Equals](const Key &KeyValue) {
               return func::match(
                   findMapValuePtr<Key, Value>(Right, KeyValue),
                   [&Left, &KeyValue, Equals](const Value *RightValue) {
                     const Value *LeftValue = Left.Find(KeyValue);
                     return LeftValue && Equals(*LeftValue, *RightValue);
                   },
                   []() { return false; });
             });
}

/**
 * @brief Folds a functional-core catalog through one ECS-visible primitive.
 * @signature template <typename Catalog, typename Accumulator, typename Step> inline Accumulator foldCatalog(const Catalog &Values, Accumulator Seed, Step FoldStep)
 *
 * User Story: As ECS feature code, noun-changing lists should recurse through
 * one neutral catalog fold instead of per-domain wrapper families.
 */
template <typename Catalog, typename Accumulator, typename Step>
inline Accumulator foldCatalog(const Catalog &Values, Accumulator Seed,
                               Step FoldStep) {
  return func::fold_catalog(Values, Seed, FoldStep);
}

/**
 * @brief Folds paired functional-core catalogs in lockstep.
 * @signature template <typename LeftCatalog, typename RightCatalog, typename Accumulator, typename Step> inline Accumulator foldCatalogPairs(const LeftCatalog &Left, const RightCatalog &Right, Accumulator Seed, Step FoldStep)
 *
 * User Story: As ECS projection and system code, selector/projector or
 * descriptor/runner pairs should be data registration plus one fold.
 */
template <typename LeftCatalog, typename RightCatalog, typename Accumulator,
          typename Step>
inline Accumulator foldCatalogPairs(const LeftCatalog &Left,
                                    const RightCatalog &Right,
                                    Accumulator Seed, Step FoldStep) {
  return func::zip_catalog_fold(Left, Right, Seed, FoldStep);
}

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
 * @brief Reads the last reusable entity id from the allocator.
 * @signature inline func::Maybe<FEntityId> lastFreedEntity(const FAllocator &Allocator)
 *
 * User Story: As allocation code, I need the reusable-id branch represented as
 * Maybe so fresh allocation composes without imperative checks.
 */
inline func::Maybe<FEntityId> lastFreedEntity(const FAllocator &Allocator) {
  return Allocator.Freed.Num() > 0 ? func::just(Allocator.Freed.Last())
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
  Result.Entity = createEntityId(Allocator.NextIndex, 0);
  Allocator.Generations.Add(Allocator.NextIndex, 0);
  Allocator.NextIndex += 1;
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
      lastFreedEntity(Allocator),
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
      findMapValue<int64, int32>(Allocator.Generations, Id.Index),
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
  const int32 NextGeneration = Id.Generation + 1;
  Allocator.Generations.Add(Id.Index, NextGeneration);
  Allocator.Freed.Add(createEntityId(Id.Index, NextGeneration));
  return Allocator;
}

/**
 * @brief Frees an entity id and increments its generation for safe reuse.
 * @signature inline FAllocator freeEntity(FAllocator Allocator, const FEntityId &Id)
 *
 * User Story: As a despawn mechanic, I need old handles to stop resolving after
 * their slot has been released.
 */
inline FAllocator freeEntity(FAllocator Allocator, const FEntityId &Id) {
  return entityGenerationMatches(Allocator, Id)
             ? freeMatchedEntity(Allocator, Id)
             : Allocator;
}

/**
 * @brief Checks whether an entity id is still alive in the allocator.
 * @signature inline bool isAlive(const FAllocator &Allocator, const FEntityId &Id)
 *
 * User Story: As a query system, I need to reject stale handles before reading
 * component data.
 */
inline bool isAlive(const FAllocator &Allocator, const FEntityId &Id) {
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
  const TArray<FString> Keys = mapKeys<FString, FComponentValue>(RawValue);
  Value.MapValue = foldArray<FString,
                             TMap<FString, TSharedPtr<FComponentValue>>>(
      TMap<FString, TSharedPtr<FComponentValue>>(), Keys,
      [&RawValue](const TMap<FString, TSharedPtr<FComponentValue>> &Acc,
                  const FString &Key) {
        return func::match(
            findMapValuePtr<FString, FComponentValue>(RawValue, Key),
            [&Acc, &Key](const FComponentValue *Found) {
              TMap<FString, TSharedPtr<FComponentValue>> Next = Acc;
              Next.Add(Key, MakeShared<FComponentValue>(*Found));
              return Next;
            },
            [&Acc]() { return Acc; });
      });
  return Value;
}

inline FComponentValue listValue(const TArray<FComponentValue> &RawValue) {
  FComponentValue Value = noneValue();
  Value.Kind = EComponentValueKind::List;
  Value.ListValue = foldArray<FComponentValue,
                              TArray<TSharedPtr<FComponentValue>>>(
      TArray<TSharedPtr<FComponentValue>>(), RawValue,
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
  return mapArray<Source, FComponentValue>(Items, MapValue);
}

inline bool componentValuePtrEquals(const TSharedPtr<FComponentValue> &Left,
                                    const TSharedPtr<FComponentValue> &Right);

inline bool componentMapEquals(
    const TMap<FString, TSharedPtr<FComponentValue>> &Left,
    const TMap<FString, TSharedPtr<FComponentValue>> &Right) {
  return mapValuesEqual<FString, TSharedPtr<FComponentValue>>(
      Left, Right,
      [](const TSharedPtr<FComponentValue> &LeftValue,
         const TSharedPtr<FComponentValue> &RightValue) {
        return componentValuePtrEquals(LeftValue, RightValue);
      });
}

inline bool componentListEquals(
    const TArray<TSharedPtr<FComponentValue>> &Left,
    const TArray<TSharedPtr<FComponentValue>> &Right) {
  const TArray<int32> Indices = indexRange(Left.Num());
  return Left.Num() == Right.Num() &&
         allArray<int32>(
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
 * @brief Registers domain path declarations through the shared array fold.
 * @signature inline FDomainRegistry registerDomainPaths(FRegisterDomainPathsRequest Request)
 *
 * User Story: As a maintainer, I can extend the ECS taxonomy by appending data
 * declarations while one fold owns the traversal shape.
 */
inline FDomainRegistry
registerDomainPaths(FRegisterDomainPathsRequest Request) {
  return foldArray<FDomainPathRegistration, FDomainRegistry>(
      Request.Registry, Request.Registrations,
      [](const FDomainRegistry &Registry,
         const FDomainPathRegistration &Registration) {
        return registerDomainPath(
            {Registry, Registration.Segments, Registration.Kind});
      },
      Request.Index);
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
 * @signature inline FDomainRegistry buildCrossDomainRegistry()
 *
 * User Story: As a feature author, I need Components / Entities / Systems /
 * Ui domains with stable subdomains so project systems can query and inspect
 * data without importing presentational view layers.
 */
inline FDomainRegistry buildCrossDomainRegistry() {
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
           {{TEXT("Systems"), TEXT("Projection")}, EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Bots")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
             TEXT("AI")},
            EDomainKind::Ai},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
             TEXT("Goals")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
             TEXT("Position")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Bots"),
             TEXT("Stats")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Interaction")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Runtime")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Spawn")},
            EDomainKind::System},
           {{TEXT("Systems"), TEXT("Projection"), TEXT("Terrain")},
            EDomainKind::System},
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

/**
 * @brief Returns the lazily memoized neutral ECS domain registry.
 * @signature inline const FDomainRegistry &crossDomainRegistryCache()
 *
 * User Story: As every new world initializes, I need the static taxonomy to be
 * computed once through functional_core lazy evaluation instead of rebuilt for
 * each world value.
 */
inline const FDomainRegistry &crossDomainRegistryCache() {
  static const func::Lazy<FDomainRegistry> Registry =
      func::lazy([]() { return buildCrossDomainRegistry(); });
  return func::eval(Registry);
}

/**
 * @brief Copies the cached neutral ECS domain registry into a world.
 * @signature inline FDomainRegistry createCrossDomainRegistry()
 *
 * User Story: As a reducer storing ECS world state, I need each world to own a
 * plain registry value while the expensive construction path remains lazy.
 */
inline FDomainRegistry createCrossDomainRegistry() {
  return crossDomainRegistryCache();
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
      findMapValue<EntityKey, FRelationship>(Request.Relationships,
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
typedef std::function<FWorldTransform(const EntityKey &)>
    FEntityKeyWorldTransformFactory;

template <typename Item>
using TWorldRowProjector =
    std::function<FWorld(const FWorld &, const Item &)>;

/**
 * @brief Projects selected rows into a world through one row projector.
 * @signature template <typename Item> inline FWorld projectRows(const FWorld &World, const TArray<Item> &Items, TWorldRowProjector<Item> Project)
 *
 * User Story: As RTK selector projection code, selected entity-adapter rows
 * should fold into ECS through one reusable world transform shape.
 */
template <typename Item>
inline FWorld projectRows(const FWorld &World, const TArray<Item> &Items,
                          TWorldRowProjector<Item> Project) {
  return foldArray<Item, FWorld>(
      World, Items, [Project](const FWorld &Acc, const Item &ItemValue) {
        return Project(Acc, ItemValue);
      });
}

/**
 * @brief Folds paired catalogs into a world through one projection step.
 * @signature template <typename LeftCatalog, typename RightCatalog, typename Step> inline FWorld foldWorldCatalogPairs(const FWorld &World, const LeftCatalog &Left, const RightCatalog &Right, Step FoldStep)
 *
 * User Story: As RTK selector projection code, changing domain nouns should be
 * registered as selector/projector catalogs and executed by one ECS fold.
 */
template <typename LeftCatalog, typename RightCatalog, typename Step>
inline FWorld foldWorldCatalogPairs(const FWorld &World,
                                    const LeftCatalog &Left,
                                    const RightCatalog &Right,
                                    Step FoldStep) {
  return foldCatalogPairs(Left, Right, World, FoldStep);
}

/**
 * @brief Folds a catalog of world transforms into one next world.
 * @signature template <typename TransformCatalog> inline FWorld foldWorldTransformCatalog(const FWorld &World, const TransformCatalog &Transforms)
 *
 * User Story: As lifecycle ECS code, named world transforms should be
 * registered as a catalog when only the cleanup noun changes.
 */
template <typename TransformCatalog>
inline FWorld foldWorldTransformCatalog(const FWorld &World,
                                        const TransformCatalog &Transforms) {
  return foldCatalog(
      Transforms, World,
      [](const FWorld &Acc, const FWorldTransform &Transform) {
        return Transform(Acc);
      });
}

/**
 * @brief Runs a list of world transforms through the shared fold shape.
 * @signature inline FWorld foldWorldTransforms(const FWorld &World, const TArray<FWorldTransform> &Transforms)
 *
 * User Story: As reducer-owned ECS projection, system execution should read as
 * a fold over named world transforms instead of hand-composed nested calls.
 */
inline FWorld foldWorldTransforms(const FWorld &World,
                                  const TArray<FWorldTransform> &Transforms) {
  return foldArray<FWorldTransform, FWorld>(
      World, Transforms,
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

struct FMarkDirtyRequest {
  FWorld World;
  EntityKey Entity;
};

struct FMarkManyDirtyRequest {
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

struct FSetParentRequest {
  FWorld World;
  EntityKey Child;
  EntityKey Parent;
};

struct FAddChildRequest {
  FWorld World;
  EntityKey Parent;
  EntityKey Child;
};

struct FRemoveChildRequest {
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

struct FGatherUnwrapRequest {
  const FWorld &World;
  EntityKey Entity;
  TArray<ComponentType> Types;
};

/**
 * @brief Builds the dirty-marking request payload.
 * @signature inline FMarkDirtyRequest createMarkDirtyRequest(FWorld World, const EntityKey &Entity)
 *
 * User Story: As ECS reducer code, I need one reusable factory for dirty
 * marking payloads instead of scattered aggregate construction.
 */
inline FMarkDirtyRequest createMarkDirtyRequest(FWorld World,
                                                const EntityKey &Entity) {
  return {World, Entity};
}

/**
 * @brief Builds the multi-entity dirty-marking request payload.
 * @signature inline FMarkManyDirtyRequest createMarkManyDirtyRequest(FWorld World, const TArray<EntityKey> &Entities)
 *
 * User Story: As relationship reducers, I need repeated dirty marking to use a
 * foldable payload rather than nested calls.
 */
inline FMarkManyDirtyRequest
createMarkManyDirtyRequest(FWorld World, const TArray<EntityKey> &Entities) {
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
 * @signature inline FSetParentRequest createSetParentRequest(FWorld World, const EntityKey &Child, const EntityKey &Parent)
 *
 * User Story: As relationship code, I need parent writes to use a reusable
 * payload factory.
 */
inline FSetParentRequest createSetParentRequest(FWorld World,
                                                const EntityKey &Child,
                                                const EntityKey &Parent) {
  return {World, Child, Parent};
}

/**
 * @brief Builds the child-adding request payload.
 * @signature inline FAddChildRequest createAddChildRequest(FWorld World, const EntityKey &Parent, const EntityKey &Child)
 *
 * User Story: As hierarchy code, I need child-add payloads constructed
 * consistently before delegating to relationship reducers.
 */
inline FAddChildRequest createAddChildRequest(FWorld World,
                                              const EntityKey &Parent,
                                              const EntityKey &Child) {
  return {World, Parent, Child};
}

/**
 * @brief Builds the child-removal request payload.
 * @signature inline FRemoveChildRequest createRemoveChildRequest(FWorld World, const EntityKey &Parent, const EntityKey &Child)
 *
 * User Story: As hierarchy code, I need child-removal payloads constructed
 * consistently before applying relationship reducers.
 */
inline FRemoveChildRequest createRemoveChildRequest(FWorld World,
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
 * @signature inline FGatherUnwrapRequest createGatherUnwrapRequest(const FWorld &World, const EntityKey &Entity, const TArray<ComponentType> &Types)
 *
 * User Story: As ECS system execution, I need component gathering to share one
 * request factory before fold-based lookup.
 */
inline FGatherUnwrapRequest
createGatherUnwrapRequest(const FWorld &World, const EntityKey &Entity,
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
inline FWorld createWorld() {
  FWorld World;
  World.Allocator = createAllocator();
  World.Domains = createCrossDomainRegistry();
  World.Generation = 0;
  return World;
}

inline bool componentTableEquals(const ComponentTable &Left,
                                 const ComponentTable &Right) {
  return mapValuesEqual<EntityKey, FComponentValue>(
      Left, Right,
      [](const FComponentValue &LeftValue,
         const FComponentValue &RightValue) { return LeftValue == RightValue; });
}

inline bool componentStoreEquals(const ComponentStore &Left,
                                 const ComponentStore &Right) {
  return mapValuesEqual<ComponentType, ComponentTable>(
      Left, Right,
      [](const ComponentTable &LeftValue, const ComponentTable &RightValue) {
        return componentTableEquals(LeftValue, RightValue);
      });
}

template <typename Value>
inline bool arrayMapEquals(const TMap<FString, TArray<Value>> &Left,
                           const TMap<FString, TArray<Value>> &Right) {
  return mapValuesEqual<FString, TArray<Value>>(
      Left, Right,
      [](const TArray<Value> &LeftValue, const TArray<Value> &RightValue) {
        return LeftValue == RightValue;
      });
}

inline bool resourceStoreEquals(const ResourceStore &Left,
                                const ResourceStore &Right) {
  return mapValuesEqual<ResourceName, FComponentValue>(
      Left, Right,
      [](const FComponentValue &LeftValue,
         const FComponentValue &RightValue) { return LeftValue == RightValue; });
}

inline bool relationshipStoreEquals(
    const TMap<EntityKey, FRelationship> &Left,
    const TMap<EntityKey, FRelationship> &Right) {
  return mapValuesEqual<EntityKey, FRelationship>(
      Left, Right,
      [](const FRelationship &LeftValue, const FRelationship &RightValue) {
        return LeftValue == RightValue;
      });
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
 * @signature inline FWorld markDirty(const FMarkDirtyRequest &Request)
 *
 * User Story: As a reducer-owned ECS transition, dirty tracking should be a
 * unary world transform that can be composed without hidden side effects.
 */
inline FWorld markDirty(const FMarkDirtyRequest &Request) {
  FWorld World = Request.World;
  World.DirtyEntities.AddUnique(Request.Entity);
  World.Generation += 1;
  return World;
}

/**
 * @brief Marks multiple entities dirty through a fold.
 * @signature inline FWorld markManyDirty(const FMarkManyDirtyRequest &Request)
 *
 * User Story: As relationship reducers, I need multi-entity dirty updates to
 * compose from the single-entity reducer rather than repeating nested calls.
 */
inline FWorld markManyDirty(const FMarkManyDirtyRequest &Request) {
  return foldArray<EntityKey, FWorld>(
      Request.World, Request.Entities,
      [](const FWorld &World, const EntityKey &Entity) {
        return markDirty(createMarkDirtyRequest(World, Entity));
      });
}

typedef std::function<FRelationship(const FRelationship &)>
    FRelationshipTransform;

struct FRelationshipWrite {
  EntityKey Entity;
  FRelationshipTransform Transform;
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
inline FRelationship eraseRelationshipChild(const EntityKey &Child,
                                            const FRelationship &Value) {
  FRelationship Next = Value;
  Next.Children.Remove(Child);
  return Next;
}

/**
 * @brief Applies one relationship write by reading default-or-present first.
 */
inline FWorld applyRelationshipWrite(const FWorld &World,
                                     const FRelationshipWrite &Write) {
  FWorld Next = World;
  Next.Relationships.Add(
      Write.Entity,
      Write.Transform(getRelationshipOrDefault(createGetRelationshipRequest(
          Next.Relationships, Write.Entity))));
  return Next;
}

/**
 * @brief Applies relationship writes through one fold.
 */
inline FWorld foldRelationshipWrites(const FWorld &World,
                                     const TArray<FRelationshipWrite> &Writes) {
  return foldArray<FRelationshipWrite, FWorld>(
      World, Writes, [](const FWorld &Acc, const FRelationshipWrite &Write) {
        return applyRelationshipWrite(Acc, Write);
      });
}

inline EntityKey relationshipWriteEntity(const FRelationshipWrite &Write) {
  return Write.Entity;
}

/**
 * @brief Marks every entity touched by relationship writes through dirty fold.
 */
inline FWorld
markRelationshipWritesDirty(const FWorld &World,
                             const TArray<FRelationshipWrite> &Writes) {
  return markManyDirty(createMarkManyDirtyRequest(
      World,
      mapArray<FRelationshipWrite, EntityKey>(Writes, relationshipWriteEntity)));
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
  Spawned.World = markDirty(createMarkDirtyRequest(World, Entity));
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
    return foldArray<ComponentType, FWorld>(
        World, mapKeys<ComponentType, ComponentTable>(World.Components),
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
    Next.Relationships.Remove(Entity);
    Next.DirtyEntities.Remove(Entity);
    return Next;
  };
}

/**
 * @brief Builds a transform factory that removes one child from one parent relationship.
 * @signature inline FEntityKeyWorldTransformFactory removeEntityFromRelationshipParent(const EntityKey &Entity)
 *
 * User Story: As hierarchy reducers, parent child-list cleanup should be a
 * curried transform instead of a relationship-table loop.
 */
inline FEntityKeyWorldTransformFactory
removeEntityFromRelationshipParent(const EntityKey &Entity) {
  return [Entity](const EntityKey &Parent) {
    return [Entity, Parent](const FWorld &World) {
      FWorld Next = World;
      FRelationship Relationship = Next.Relationships.FindChecked(Parent);
      Relationship.Children.Remove(Entity);
      Next.Relationships.Add(Parent, Relationship);
      return Next;
    };
  };
}

/**
 * @brief Builds a world transform that removes an entity from parent child lists.
 * @signature inline FWorldTransform removeEntityRelationshipChildIndexes(const EntityKey &Entity)
 *
 * User Story: As hierarchy reducers, child-list cleanup should fold through
 * relationship keys as a reusable world transform.
 */
inline FWorldTransform
removeEntityRelationshipChildIndexes(const EntityKey &Entity) {
  return [Entity](const FWorld &World) {
    const FEntityKeyWorldTransformFactory RemoveFromParent =
        removeEntityFromRelationshipParent(Entity);
    return foldArray<EntityKey, FWorld>(
        World, mapKeys<EntityKey, FRelationship>(World.Relationships),
        [RemoveFromParent](const FWorld &Acc, const EntityKey &Parent) {
          return RemoveFromParent(Parent)(Acc);
        });
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
  const FWorld World = foldWorldTransformCatalog(
      Request.World,
      func::catalog(removeEntityComponentIndexes(Request.Entity),
                    removeEntityDirectIndexes(Request.Entity),
                    removeEntityRelationshipChildIndexes(Request.Entity)));
  return markDirty(createMarkDirtyRequest(World, Request.Entity));
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
  World.Allocator = freeEntity(World.Allocator, Request.Id);
  return removeEntity(createRemoveEntityRequest(World, entityKey(Request.Id)));
}

/**
 * @brief Sets a parent relationship using immutable world-in/world-out data.
 * @signature inline FWorld setParent(const FSetParentRequest &Request)
 *
 * User Story: As relationship ECS code, I need parent/child writes to stay a
 * pure request transform and mark both touched entities.
 */
inline FWorld setParent(const FSetParentRequest &Request) {
  TArray<FRelationshipWrite> Writes;
  Writes.Add(
      {Request.Child,
       [Parent = Request.Parent](const FRelationship &Relationship) {
         return assignRelationshipParent(Parent, Relationship);
       }});
  Writes.Add(
      {Request.Parent,
       [Child = Request.Child](const FRelationship &Relationship) {
         return appendRelationshipChild(Child, Relationship);
       }});
  return markRelationshipWritesDirty(
      foldRelationshipWrites(Request.World, Writes), Writes);
}

/**
 * @brief Adds one child to a parent relationship.
 * @signature inline FWorld addChild(const FAddChildRequest &Request)
 *
 * User Story: As ECS relationship code, adding children should compose through
 * the same parent-setting primitive instead of duplicating write logic.
 */
inline FWorld addChild(const FAddChildRequest &Request) {
  return setParent(createSetParentRequest(Request.World, Request.Child,
                                          Request.Parent));
}

/**
 * @brief Removes one child from a parent relationship.
 * @signature inline FWorld removeChild(const FRemoveChildRequest &Request)
 *
 * User Story: As relationship ECS code, removing children should be a typed
 * unary transition over the world value.
 */
inline FWorld removeChild(const FRemoveChildRequest &Request) {
  TArray<FRelationshipWrite> Writes;
  Writes.Add(
      {Request.Parent,
       [Child = Request.Child](const FRelationship &Relationship) {
         return eraseRelationshipChild(Child, Relationship);
       }});
  Writes.Add({Request.Child, [](const FRelationship &Relationship) {
               return clearRelationshipParent(Relationship);
             }});
  return markRelationshipWritesDirty(
      foldRelationshipWrites(Request.World, Writes), Writes);
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
  const TArray<EntityKey> DomainEntities =
      mapKeys<EntityKey, TArray<DomainPathKey>>(World.EntityDomains);
  const TArray<EntityKey> TaggedEntities =
      mapKeys<EntityKey, TArray<Tag>>(World.Tags);
  const TArray<ComponentType> ComponentTypes =
      mapKeys<ComponentType, ComponentTable>(World.Components);
  const TArray<EntityKey> ComponentEntities =
      foldArray<ComponentType, TArray<EntityKey>>(
      TArray<EntityKey>(), ComponentTypes,
      [&World](const TArray<EntityKey> &Acc, const ComponentType &Type) {
        return func::match(
            findMapValuePtr<ComponentType, ComponentTable>(World.Components,
                                                           Type),
            [&Acc](const ComponentTable *Table) {
              return appendMapKeys<EntityKey, FComponentValue>(Acc, *Table);
            },
            [&Acc]() { return Acc; });
      });
  return uniqueArray<EntityKey>(concatArrays<EntityKey>(
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
  return markDirty(createMarkDirtyRequest(World, Request.Entity));
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
      findMapValuePtr<ComponentType, ComponentTable>(
          Request.World.Components, Request.Type),
      [&Request](const ComponentTable *Table) {
        return findMapValue<EntityKey, FComponentValue>(*Table,
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
 * @signature inline std::function<FWorld(const ComponentTable &)> removeComponentFromPresentTable(const FRemoveComponentRequest &Request)
 *
 * User Story: As ECS reducers, component removal should isolate the present
 * table branch so Maybe matching replaces imperative nullable checks.
 */
inline std::function<FWorld(const ComponentTable &)>
removeComponentFromPresentTable(const FRemoveComponentRequest &Request) {
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
      findMapValue<ComponentType, ComponentTable>(Request.World.Components,
                                                 Request.Type),
      removeComponentFromPresentTable(Request),
      [&Request]() { return Request.World; });
  return markDirty(createMarkDirtyRequest(World, Request.Entity));
}

/**
 * @brief Adds one tag to one entity using a request payload.
 */
inline FWorld setTag(const FSetTagRequest &Request) {
  FWorld World = Request.World;
  World.Tags.FindOrAdd(Request.Entity).AddUnique(Request.TagValue);
  return markDirty(createMarkDirtyRequest(World, Request.Entity));
}

/**
 * @brief Checks whether an entity currently has a tag.
 * @signature inline bool hasTag(const FHasTagRequest &Request)
 *
 * User Story: As ECS query code, tag membership should use Maybe lookup plus
 * reusable array predicates.
 */
inline bool hasTag(const FHasTagRequest &Request) {
  return mapArrayContains<EntityKey, Tag>(Request.World.Tags, Request.Entity,
                                          Request.TagValue);
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

/**
 * @brief Reads one resource value from the world.
 * @signature inline func::Maybe<FComponentValue> getResource(const FGetResourceRequest &Request)
 *
 * User Story: As ECS side-effect orchestration code, resource reads should be
 * optional values instead of nullable pointer branches.
 */
inline func::Maybe<FComponentValue>
getResource(const FGetResourceRequest &Request) {
  return findMapValue<ResourceName, FComponentValue>(Request.World.Resources,
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
      findMapValue<EventType, TArray<FComponentValue>>(Request.World.Events,
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
  return markDirty(createMarkDirtyRequest(World, Request.Entity));
}

/**
 * @brief Checks whether an entity belongs to a domain path.
 * @signature inline bool entityInDomain(const FEntityInDomainRequest &Request)
 *
 * User Story: As ECS domain queries, membership should be a Maybe lookup plus
 * an array predicate instead of borrowing sibling-domain logic.
 */
inline bool entityInDomain(const FEntityInDomainRequest &Request) {
  return mapArrayContains<EntityKey, DomainPathKey>(
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
  return filterArray<EntityKey>(
      Request.Entities,
      [&Request](const EntityKey &Entity) {
        return allArray<ComponentType>(
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
      Request.World, Request.Types, allEntities(Request.World)));
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
  const TArray<EntityKey> Entities = allEntities(Request.World);
  return filterArray<EntityKey>(
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
  const TArray<EntityKey> Entities = allEntities(Request.World);
  return filterArray<EntityKey>(
      Entities,
      [&Request](const EntityKey &Entity) {
        return entityInDomain(createEntityInDomainRequest(
            Request.World, Entity, Request.Domain));
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
 * @signature inline TMap<ComponentType, FComponentValue> gatherUnwrap(const FGatherUnwrapRequest &Request)
 *
 * User Story: As ECS system execution, component gathering should fold over
 * requested types and let Maybe decide whether each component is present.
 */
inline TMap<ComponentType, FComponentValue>
gatherUnwrap(const FGatherUnwrapRequest &Request) {
  return foldArray<ComponentType, TMap<ComponentType, FComponentValue>>(
      TMap<ComponentType, FComponentValue>(), Request.Types,
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
 * @signature inline std::function<bool(const EntityKey &)> requireTags(const FWorld &World, const TArray<Tag> &Tags)
 *
 * User Story: As ECS system matching, tag requirements should be expressed as
 * a function factory returning a unary predicate over entities.
 */
inline std::function<bool(const EntityKey &)>
requireTags(const FWorld &World, const TArray<Tag> &Tags) {
  return [&World, Tags](const EntityKey &Entity) {
    return allArray<Tag>(
        Tags,
        [&World, &Entity](const Tag &TagValue) {
          return hasTag(createHasTagRequest(World, Entity, TagValue));
        });
  };
}

/**
 * @brief Creates a reusable predicate that checks an optional domain guard.
 * @signature inline std::function<bool(const EntityKey &)> requireOptionalDomain(const FWorld &World, const func::Maybe<DomainPathKey> &Domain)
 *
 * User Story: As ECS system matching, optional domain guards should be
 * first-class unary predicates that compose with tag predicates.
 */
inline std::function<bool(const EntityKey &)>
requireOptionalDomain(const FWorld &World,
                      const func::Maybe<DomainPathKey> &Domain) {
  return [&World, Domain](const EntityKey &Entity) {
    return func::match(
        Domain,
        [&World, &Entity](const DomainPathKey &DomainValue) {
          return entityInDomain(
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
      {requireTags(World, Descriptor.RequiredTags),
       requireOptionalDomain(World, Descriptor.Domain)});
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
  return foldArray<EntityKey, FWorld>(
      Request.World, Matching,
      [&Request](const FWorld &World, const EntityKey &Entity) {
        return Request.System(createSystemExecutionPayload(
            World, Entity,
            gatherUnwrap(createGatherUnwrapRequest(World, Entity,
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
      filterArray<EntityKey>(ComponentMatches, MatchesDescriptor);
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
 * @brief Creates a reusable predicate that checks for a key in a map.
 * @signature template <typename Key, typename Value> inline std::function<bool(const Key &)> mapHasKey(const TMap<Key, Value> &Values)
 *
 * User Story: As registry validation, repeated existence checks should be one
 * map predicate factory rather than per-domain request/factory pairs.
 */
template <typename Key, typename Value>
inline std::function<bool(const Key &)>
mapHasKey(const TMap<Key, Value> &Values) {
  return [&Values](const Key &KeyValue) { return Values.Contains(KeyValue); };
}

/**
 * @brief Creates a reusable Either validator for required map keys.
 * @signature template <typename Key, typename Value> inline std::function<func::Either<FString, bool>(const Key &)> requireMapKey(const TMap<Key, Value> &Values, std::function<FString(const Key &)> ErrorMessage)
 *
 * User Story: As registry validation, missing-key failures should be a generic
 * validation function that composes with Either bind.
 */
template <typename Key, typename Value>
inline std::function<func::Either<FString, bool>(const Key &)> requireMapKey(
    const TMap<Key, Value> &Values,
    std::function<FString(const Key &)> ErrorMessage) {
  const std::function<bool(const Key &)> Exists = mapHasKey<Key, Value>(Values);
  return [Exists, ErrorMessage](const Key &KeyValue) {
    return Exists(KeyValue) ? func::make_right<FString, bool>(true)
                            : func::make_left<FString, bool>(
                                  ErrorMessage(KeyValue));
  };
}

/**
 * @brief Validates every item with an Either-returning unary validator.
 * @signature template <typename Item> inline func::Either<FString, bool> validateEach(const TArray<Item> &Items, std::function<func::Either<FString, bool>(const Item &)> Validate, int32 Index = 0)
 *
 * User Story: As ECS validation, list validation should reuse one recursive
 * Either pipeline rather than open-coded loops.
 */
template <typename Item>
inline func::Either<FString, bool> validateEach(
    const TArray<Item> &Items,
    std::function<func::Either<FString, bool>(const Item &)> Validate,
    int32 Index = 0) {
  return Index >= Items.Num()
             ? func::make_right<FString, bool>(true)
             : func::ebind(Validate(Items[Index]),
                           [&Items, Validate, Index](const bool &) {
                             return validateEach<Item>(Items, Validate,
                                                       Index + 1);
                           });
}

/**
 * @brief Creates a reusable domain-exists validator for a registry.
 * @signature inline std::function<func::Either<FString, bool>(const DomainPathKey &)> requireDomain(const FDomainRegistry &Registry)
 *
 * User Story: As registry validation, domain validation should be a reusable
 * function factory over a registry.
 */
inline std::function<func::Either<FString, bool>(const DomainPathKey &)>
requireDomain(const FDomainRegistry &Registry) {
  return requireMapKey<DomainPathKey, FDomainNode>(
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
  return requireMapKey<ComponentType, FComponentSchema>(
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
  return entityInDomain(createEntityInDomainRequest(
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
 * and component validation through functional_core Either semantics.
 */
inline func::Either<FString, bool>
validateSystemSpec(const FValidateSystemSpecRequest &Request) {
  return func::ebind(
      requireDomain(Request.Registry)(Request.Spec.Domain),
      [&Request](const bool &) {
        return validateEach<ComponentType>(
            Request.Spec.RequiredComponents,
            requireComponentSchema(Request.Registry));
      });
}

typedef func::ArgDispatcher<int32, FComponentValue, FString>
    FComponentValueFormatter;

/**
 * @brief Converts a component value kind into a dispatcher key.
 * @signature inline int32 componentValueKindKey(EComponentValueKind Kind)
 *
 * User Story: As ECS inspection code, enum formatting should use a stable
 * dispatcher key that composes with functional_core dispatch helpers.
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
 * @brief Builds the component value formatter through functional composition.
 * @signature inline FComponentValueFormatter createComponentValueFormatter()
 *
 * User Story: As ECS inspection code, component formatting should be a
 * dispatcher assembled from reusable registrations instead of a branch ladder.
 */
inline FComponentValueFormatter createComponentValueFormatter() {
  return (func::pipe(
              func::create_arg_dispatcher<int32, FComponentValue, FString>())
          | registerComponentValueFormatter(
                EComponentValueKind::None,
                [](const FComponentValue &) { return FString(TEXT("None")); })
          | registerComponentValueFormatter(
                EComponentValueKind::Bool,
                [](const FComponentValue &Value) {
                  return Value.BoolValue ? FString(TEXT("true"))
                                         : FString(TEXT("false"));
                })
          | registerComponentValueFormatter(
                EComponentValueKind::Int,
                [](const FComponentValue &Value) {
                  return FString::Printf(TEXT("%lld"), Value.IntValue);
                })
          | registerComponentValueFormatter(
                EComponentValueKind::Float,
                [](const FComponentValue &Value) {
                  return FString::SanitizeFloat(Value.FloatValue);
                })
          | registerComponentValueFormatter(
                EComponentValueKind::Text,
                [](const FComponentValue &Value) { return Value.TextValue; })
          | registerComponentValueFormatter(
                EComponentValueKind::Vec2,
                [](const FComponentValue &Value) {
                  return Value.Vec2Value.ToString();
                })
          | registerComponentValueFormatter(
                EComponentValueKind::Vec3,
                [](const FComponentValue &Value) {
                  return Value.Vec3Value.ToString();
                })
          | registerComponentValueFormatter(
                EComponentValueKind::Map,
                [](const FComponentValue &Value) {
                  return FString::Printf(TEXT("Map(len=%d)"),
                                         Value.MapValue.Num());
                })
          | registerComponentValueFormatter(
                EComponentValueKind::List,
                [](const FComponentValue &Value) {
                  return FString::Printf(TEXT("List(len=%d)"),
                                         Value.ListValue.Num());
                }))
      .val;
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
 * @signature inline FTagInspectionSelector inspectTagsInWorld(const FWorld &World)
 *
 * User Story: As display inspection code, tag lookup should be a unary selector
 * over entity keys instead of nullable pointer logic.
 */
inline FTagInspectionSelector inspectTagsInWorld(const FWorld &World) {
  return [&World](const EntityKey &Entity) {
    return func::match(
        findMapValue<EntityKey, TArray<Tag>>(World.Tags, Entity),
        [](const TArray<Tag> &Tags) { return Tags; },
        []() { return TArray<Tag>(); });
  };
}

/**
 * @brief Builds a selector for entity domains used by inspection.
 * @signature inline FDomainInspectionSelector inspectDomainsInWorld(const FWorld &World)
 *
 * User Story: As display inspection code, domain lookup should be a unary
 * selector over entity keys instead of nullable pointer logic.
 */
inline FDomainInspectionSelector inspectDomainsInWorld(const FWorld &World) {
  return [&World](const EntityKey &Entity) {
    return func::match(
        findMapValue<EntityKey, TArray<DomainPathKey>>(World.EntityDomains,
                                                      Entity),
        [](const TArray<DomainPathKey> &Domains) { return Domains; },
        []() { return TArray<DomainPathKey>(); });
  };
}

/**
 * @brief Builds curried component inspection transforms for one world.
 * @signature inline FEntityComponentInspectionFactory inspectComponentsInWorld(const FWorld &World)
 *
 * User Story: As ECS inspection code, component display data should fold
 * through curried transforms instead of component table loops.
 */
inline FEntityComponentInspectionFactory
inspectComponentsInWorld(const FWorld &World) {
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
 * @signature inline FEntityInspection inspect(const FWorld &World, const EntityKey &Entity)
 *
 * User Story: As display code, entity inspection should derive read-only
 * projection data from selectors and ECS lookups without owning logic.
 */
inline FEntityInspection inspect(const FWorld &World, const EntityKey &Entity) {
  FEntityInspection Inspection = createEntityInspection(Entity);
  Inspection.Tags = inspectTagsInWorld(World)(Entity);
  Inspection.Domains = inspectDomainsInWorld(World)(Entity);
  const FComponentInspectionTransformFactory AddComponent =
      inspectComponentsInWorld(World)(Entity);
  return foldArray<ComponentType, FEntityInspection>(
      Inspection, mapKeys<ComponentType, ComponentTable>(World.Components),
      [AddComponent](const FEntityInspection &Acc,
                     const ComponentType &Type) {
        return AddComponent(Type)(Acc);
      });
}

} // namespace ecs

#endif // DEMO_PROJECT_CORE_ECS_HPP
