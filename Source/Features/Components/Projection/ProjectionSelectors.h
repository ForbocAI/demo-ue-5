#pragma once

#include "Features/Components/Projection/ComponentsProjectionAdapters.h"
#include "Core/ecs.hpp"

#include <initializer_list>

// Component selectors: pure value lookups, atom/domain/path construction,
// and component value field traversal. No ECS world mutation, no projection.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

// --- Atom/Domain construction (pure string helpers) ---

/** User Story: As a features components projection consumer, I need to invoke component atom through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FString ComponentAtom(const char *Atom) */
inline FString ComponentAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

/** User Story: As a features components projection consumer, I need to invoke component atoms through a stable signature so the features components projection workflow remains explicit and composable. @fn inline TArray<FString> ComponentAtoms(std::initializer_list<const char *> Atoms) */
inline TArray<FString> ComponentAtoms(std::initializer_list<const char *> Atoms) {
  const TArray<const char *> AtomList(Atoms);
  return func::map_array<const char *, FString>(
      AtomList, [](const char *Atom) { return ComponentAtom(Atom); });
}

/** User Story: As a features components projection consumer, I need to invoke component domain through a stable signature so the features components projection workflow remains explicit and composable. @fn inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms) */
inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms) {
  return ComponentAtoms(Atoms);
}

/** User Story: As a features components projection consumer, I need to invoke component domain through a stable signature so the features components projection workflow remains explicit and composable. @fn inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms, const FString &Tail) */
inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms,
                                       const FString &Tail) {
  TArray<FString> Segments = ComponentAtoms(Atoms);
  Segments.Add(Tail);
  return Segments;
}

/** User Story: As a features components projection consumer, I need to invoke component domains through a stable signature so the features components projection workflow remains explicit and composable. @fn inline TArray<TArray<FString>> ComponentDomains(std::initializer_list<FComponentDomainDeclaration> Domains) */
inline TArray<TArray<FString>>
ComponentDomains(std::initializer_list<FComponentDomainDeclaration> Domains) {
  const TArray<FComponentDomainDeclaration> DomainList(Domains);
  return func::map_array<FComponentDomainDeclaration, TArray<FString>>(
      DomainList, [](const FComponentDomainDeclaration &Domain) {
        return Domain.Segments;
      });
}

/** User Story: As a features components projection consumer, I need to invoke fcomponent domain declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FComponentDomainDeclaration::FComponentDomainDeclaration( std::initializer_list<const char *> InSegments) */
inline FComponentDomainDeclaration::FComponentDomainDeclaration(
    std::initializer_list<const char *> InSegments)
    : Segments(ComponentAtoms(InSegments)) {}

/** User Story: As a features components projection consumer, I need to invoke component path through a stable signature so the features components projection workflow remains explicit and composable. @fn inline ecs::ComponentType ComponentPath(const FString &Domain, const FString &Name) */
inline ecs::ComponentType ComponentPath(const FString &Domain,
                                        const FString &Name) {
  return FString::Printf(TEXT("%s/%s"), *Domain, *Name);
}

// --- Component value field traversal (pure selectors) ---

/** User Story: As a features components projection consumer, I need to invoke component value from shared through a stable signature so the features components projection workflow remains explicit and composable. @fn inline func::Maybe<ecs::FComponentValue> ComponentValueFromShared(const TSharedPtr<ecs::FComponentValue> &Value) */
inline func::Maybe<ecs::FComponentValue>
ComponentValueFromShared(const TSharedPtr<ecs::FComponentValue> &Value) {
  return Value.IsValid() ? func::just<ecs::FComponentValue>(*Value)
                         : func::nothing<ecs::FComponentValue>();
}

/** User Story: As a features components projection consumer, I need to invoke find component value field through a stable signature so the features components projection workflow remains explicit and composable. @fn inline func::Maybe<ecs::FComponentValue> FindComponentValueField(const ecs::FComponentValue &Value, const FString &Name) */
inline func::Maybe<ecs::FComponentValue>
FindComponentValueField(const ecs::FComponentValue &Value,
                        const FString &Name) {
  return func::match(
      func::find_map_value<FString, TSharedPtr<ecs::FComponentValue>>(
          Value.MapValue, Name),
      [](const TSharedPtr<ecs::FComponentValue> &Found) {
        return ComponentValueFromShared(Found);
      },
      []() { return func::nothing<ecs::FComponentValue>(); });
}

/** User Story: As a features components projection consumer, I need to invoke select component value path through a stable signature so the features components projection workflow remains explicit and composable. @fn inline func::Maybe<ecs::FComponentValue> SelectComponentValuePath(const ecs::FComponentValue &Value, const TArray<FString> &Path) */
inline func::Maybe<ecs::FComponentValue>
SelectComponentValuePath(const ecs::FComponentValue &Value,
                         const TArray<FString> &Path) {
  return func::fold_array<FString, func::Maybe<ecs::FComponentValue>>(
      Path, func::just<ecs::FComponentValue>(Value),
      [](const func::Maybe<ecs::FComponentValue> &Acc,
         const FString &Segment) {
        return func::match(
            Acc,
            [&Segment](const ecs::FComponentValue &Current) {
              return FindComponentValueField(Current, Segment);
            },
            []() { return func::nothing<ecs::FComponentValue>(); });
      });
}

// --- Component value field map construction ---

/** User Story: As a features components projection consumer, I need to invoke component value fields through a stable signature so the features components projection workflow remains explicit and composable. @fn inline TMap<FString, ecs::FComponentValue> ComponentValueFields(std::initializer_list<FComponentValueField> Fields) */
inline TMap<FString, ecs::FComponentValue>
ComponentValueFields(std::initializer_list<FComponentValueField> Fields) {
  const TArray<FComponentValueField> FieldList(Fields);
  return func::fold_array<FComponentValueField,
                          TMap<FString, ecs::FComponentValue>>(
      FieldList, TMap<FString, ecs::FComponentValue>(),
      [](const TMap<FString, ecs::FComponentValue> &Acc,
         const FComponentValueField &Field) {
        TMap<FString, ecs::FComponentValue> Next = Acc;
        Next.Add(Field.Name, Field.Value);
        return Next;
      });
}

/** User Story: As a features components projection consumer, I need to invoke component value map through a stable signature so the features components projection workflow remains explicit and composable. @fn inline ecs::FComponentValue ComponentValueMap(std::initializer_list<FComponentValueField> Fields) */
inline ecs::FComponentValue
ComponentValueMap(std::initializer_list<FComponentValueField> Fields) {
  return ecs::createMapComponentValue(ComponentValueFields(Fields));
}

// --- Registered component field/group helpers ---

/** User Story: As a features components projection consumer, I need to invoke fregistered component field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration( const char *InName) */
inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration(
    const char *InName)
    : Name(ComponentAtom(InName)), Path(ComponentAtoms({InName})) {}

/** User Story: As a features components projection consumer, I need to invoke fregistered component field declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration( const char *InName, std::initializer_list<const char *> InPath) */
inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration(
    const char *InName, std::initializer_list<const char *> InPath)
    : Name(ComponentAtom(InName)), Path(ComponentAtoms(InPath)) {}

/** User Story: As a features components projection consumer, I need to invoke registered component fields through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FRegisteredComponentFieldSet RegisteredComponentFields( std::initializer_list<FRegisteredComponentFieldDeclaration> Declarations) */
inline FRegisteredComponentFieldSet
RegisteredComponentFields(
    std::initializer_list<FRegisteredComponentFieldDeclaration> Declarations) {
  return {TArray<FRegisteredComponentFieldDeclaration>(Declarations)};
}

/** User Story: As a features components projection consumer, I need to invoke fregistered component group declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn inline FRegisteredComponentGroupDeclaration::FRegisteredComponentGroupDeclaration( const char *InDomain, std::initializer_list<FRegisteredComponentFieldDeclaration> InFields) */
inline FRegisteredComponentGroupDeclaration::FRegisteredComponentGroupDeclaration(
    const char *InDomain,
    std::initializer_list<FRegisteredComponentFieldDeclaration> InFields)
    : Domain(ComponentAtom(InDomain)),
      Fields{TArray<FRegisteredComponentFieldDeclaration>(InFields)} {}

// --- Component text lookup (pure enum-to-string selector) ---

/** User Story: As a features components projection consumer, I need to invoke tcomponent text declaration through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value> TComponentTextDeclaration<Value>::TComponentTextDeclaration(Value InExpected, const char *InText) */
template <typename Value>
TComponentTextDeclaration<Value>::TComponentTextDeclaration(Value InExpected, const char *InText)
    : Expected(InExpected), Text(ComponentAtom(InText)) {}

/** User Story: As a features components projection consumer, I need to invoke component text through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value> FString ComponentText( Value Current, const TArray<TComponentTextDeclaration<Value>> &Declarations) */
template <typename Value>
FString ComponentText(
    Value Current,
    const TArray<TComponentTextDeclaration<Value>> &Declarations) {
  const func::Maybe<FString> Text = func::fold_indexed(
      Declarations, static_cast<size_t>(Declarations.Num()),
      func::nothing<FString>(),
      [Current](const func::Maybe<FString> &Acc,
                const TComponentTextDeclaration<Value> &Declaration) {
        return Acc.hasValue || Declaration.Expected != Current
                   ? Acc
                   : func::just(Declaration.Text);
      });
  check(Text.hasValue);
  return Text.value;
}

/** User Story: As a features components projection consumer, I need to invoke component text through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value> FString ComponentText( Value Current, std::initializer_list<TComponentTextDeclaration<Value>> Declarations) */
template <typename Value>
FString ComponentText(
    Value Current,
    std::initializer_list<TComponentTextDeclaration<Value>> Declarations) {
  return ComponentText<Value>(Current,
                              TArray<TComponentTextDeclaration<Value>>(
                                  Declarations));
}

/** User Story: As a features components projection consumer, I need to invoke registered component text through a stable signature so the features components projection workflow remains explicit and composable. @fn template <typename Value> FString RegisteredComponentText(Value Current) */
template <typename Value> FString RegisteredComponentText(Value Current) {
  return ComponentText<Value>(Current,
                              TComponentTextRegistry<Value>::Declarations());
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
