#pragma once

#include "Core/ecs.hpp"

#include <initializer_list>
#include <type_traits>

// Component types: pure data contracts for ECS projection.
// Payload structs, binding structs, declaration structs, projection structs.
// No domain decisions beyond trivial value comparison.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

struct FProjectResourcePayload {
  ecs::FWorld World;
  ecs::ResourceName Name;
  ecs::FComponentValue Value;
};

struct FComponentProjectionBinding {
  ecs::ComponentType Type;
  ecs::FComponentValue Value;
};

struct FProjectEntityPayload {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  TArray<TArray<FString>> Domains;
  TArray<FComponentProjectionBinding> Components;
};

struct FComponentDomainDeclaration {
  TArray<FString> Segments;

  /** User Story: As a features components projection consumer, I need to invoke fcomponent domain declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FComponentDomainDeclaration(std::initializer_list<const char *> InSegments) */
  FComponentDomainDeclaration(std::initializer_list<const char *> InSegments);
};

struct FComponentValueField {
  FString Name;
  ecs::FComponentValue Value;

  /** User Story: As a features components projection consumer, I need to invoke fcomponent value field through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename ValueType> FComponentValueField(const char *InName, const ValueType &InValue) */
  template <typename ValueType>
  FComponentValueField(const char *InName, const ValueType &InValue);
};

template <typename Value> struct TComponentTextDeclaration {
  Value Expected;
  FString Text;

  /** User Story: As a features components projection consumer, I need to invoke tcomponent text declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn TComponentTextDeclaration(Value InExpected, const char *InText) */
  TComponentTextDeclaration(Value InExpected, const char *InText);
};

template <typename Value> struct TComponentTextRegistry;

template <typename Source> struct TComponentSourceValueFieldDeclaration {
  FString Name;
  TFunction<ecs::FComponentValue(const Source &)> Project;

  /** User Story: As a features components projection consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn TComponentSourceValueFieldDeclaration() */
  TComponentSourceValueFieldDeclaration();

  /** User Story: As a features components projection consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value> TComponentSourceValueFieldDeclaration(const char *InName, Value Source::*Member) */
  template <typename Value>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member);

  /** User Story: As a features components projection consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value, typename Output> TComponentSourceValueFieldDeclaration(const char *InName, Value Source::*Member, Output (*Convert)(Value)) */
  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(Value));

  /** User Story: As a features components projection consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value, typename Output> TComponentSourceValueFieldDeclaration(const char *InName, Value Source::*Member, Output (*Convert)(const Value &)) */
  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(const Value &));

  /** User Story: As a features components projection consumer, I need to invoke tcomponent source value field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Output> TComponentSourceValueFieldDeclaration(const char *InName, Output (*ProjectValue)(const Source &)) */
  template <typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Output (*ProjectValue)(const Source &));
};

template <typename Source> struct TComponentSourceValueFieldRegistry;

template <typename Source>
using TComponentProjection =
    std::function<FComponentProjectionBinding(const Source &)>;

struct FRegisteredComponentFieldDeclaration {
  FString Name;
  TArray<FString> Path;

  /** User Story: As a features components projection consumer, I need to invoke fregistered component field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FRegisteredComponentFieldDeclaration() */
  FRegisteredComponentFieldDeclaration() {}

  /** User Story: As a features components projection consumer, I need to invoke fregistered component field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FRegisteredComponentFieldDeclaration(const char *InName) */
  FRegisteredComponentFieldDeclaration(const char *InName);

  /** User Story: As a features components projection consumer, I need to invoke fregistered component field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FRegisteredComponentFieldDeclaration( const char *InName, std::initializer_list<const char *> InPath) */
  FRegisteredComponentFieldDeclaration(
      const char *InName, std::initializer_list<const char *> InPath);
};

struct FRegisteredComponentFieldSet {
  TArray<FRegisteredComponentFieldDeclaration> Fields;
};

struct FRegisteredComponentGroupDeclaration {
  FString Domain;
  FRegisteredComponentFieldSet Fields;

  /** User Story: As a features components projection consumer, I need to invoke fregistered component group declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FRegisteredComponentGroupDeclaration() */
  FRegisteredComponentGroupDeclaration() {}

  /** User Story: As a features components projection consumer, I need to invoke fregistered component group declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn FRegisteredComponentGroupDeclaration( const char *InDomain, std::initializer_list<FRegisteredComponentFieldDeclaration> InFields) */
  FRegisteredComponentGroupDeclaration(
      const char *InDomain,
      std::initializer_list<FRegisteredComponentFieldDeclaration> InFields);
};

template <typename SelectEntity, typename SelectDomains,
          typename SelectSource, typename ComponentCatalog>
struct TEntityCatalogProjection {
  SelectEntity SelectEntityValue;
  SelectDomains SelectDomainValues;
  SelectSource SelectSourceValue;
  ComponentCatalog Components;
};

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
