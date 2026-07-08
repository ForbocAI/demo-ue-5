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

  FComponentDomainDeclaration(std::initializer_list<const char *> InSegments);
};

struct FComponentValueField {
  FString Name;
  ecs::FComponentValue Value;

  template <typename ValueType>
  FComponentValueField(const char *InName, const ValueType &InValue);
};

template <typename Value> struct TComponentTextDeclaration {
  Value Expected;
  FString Text;

  TComponentTextDeclaration(Value InExpected, const char *InText);
};

template <typename Value> struct TComponentTextRegistry;

template <typename Source> struct TComponentSourceValueFieldDeclaration {
  FString Name;
  TFunction<ecs::FComponentValue(const Source &)> Project;

  TComponentSourceValueFieldDeclaration();

  template <typename Value>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member);

  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(Value));

  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(const Value &));

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

  FRegisteredComponentFieldDeclaration() {}

  FRegisteredComponentFieldDeclaration(const char *InName);

  FRegisteredComponentFieldDeclaration(
      const char *InName, std::initializer_list<const char *> InPath);
};

struct FRegisteredComponentFields {
  TArray<FRegisteredComponentFieldDeclaration> Fields;
};

struct FRegisteredComponentGroupDeclaration {
  FString Domain;
  FRegisteredComponentFields Fields;

  FRegisteredComponentGroupDeclaration() {}

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
