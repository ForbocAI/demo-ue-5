#pragma once

#include "Features/Components/Adapters/Types.h"
#include "Core/ecs.hpp"

#include <initializer_list>

// Component selectors: pure value lookups, atom/domain/path construction,
// and component value field traversal. No ECS world mutation, no projection.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

// --- Atom/Domain construction (pure string helpers) ---

inline FString ComponentAtom(const char *Atom) {
  return FString(UTF8_TO_TCHAR(Atom));
}

inline TArray<FString> ComponentAtoms(std::initializer_list<const char *> Atoms) {
  const TArray<const char *> AtomList(Atoms);
  return func::map_array<const char *, FString>(
      AtomList, [](const char *Atom) { return ComponentAtom(Atom); });
}

inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms) {
  return ComponentAtoms(Atoms);
}

inline TArray<FString> ComponentDomain(std::initializer_list<const char *> Atoms,
                                       const FString &Tail) {
  TArray<FString> Segments = ComponentAtoms(Atoms);
  Segments.Add(Tail);
  return Segments;
}

inline TArray<TArray<FString>>
ComponentDomains(std::initializer_list<FComponentDomainDeclaration> Domains) {
  const TArray<FComponentDomainDeclaration> DomainList(Domains);
  return func::map_array<FComponentDomainDeclaration, TArray<FString>>(
      DomainList, [](const FComponentDomainDeclaration &Domain) {
        return Domain.Segments;
      });
}

inline FComponentDomainDeclaration::FComponentDomainDeclaration(
    std::initializer_list<const char *> InSegments)
    : Segments(ComponentAtoms(InSegments)) {}

inline ecs::ComponentType ComponentPath(const FString &Domain,
                                        const FString &Name) {
  return FString::Printf(TEXT("%s/%s"), *Domain, *Name);
}

// --- Component value field traversal (pure selectors) ---

inline func::Maybe<ecs::FComponentValue>
ComponentValueFromShared(const TSharedPtr<ecs::FComponentValue> &Value) {
  return Value.IsValid() ? func::just<ecs::FComponentValue>(*Value)
                         : func::nothing<ecs::FComponentValue>();
}

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

inline ecs::FComponentValue
ComponentValueMap(std::initializer_list<FComponentValueField> Fields) {
  return ecs::createMapComponentValue(ComponentValueFields(Fields));
}

// --- Registered component field/group helpers ---

inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration(
    const char *InName)
    : Name(ComponentAtom(InName)), Path(ComponentAtoms({InName})) {}

inline FRegisteredComponentFieldDeclaration::FRegisteredComponentFieldDeclaration(
    const char *InName, std::initializer_list<const char *> InPath)
    : Name(ComponentAtom(InName)), Path(ComponentAtoms(InPath)) {}

inline FRegisteredComponentFields
RegisteredComponentFields(
    std::initializer_list<FRegisteredComponentFieldDeclaration> Declarations) {
  return {TArray<FRegisteredComponentFieldDeclaration>(Declarations)};
}

inline FRegisteredComponentGroupDeclaration::FRegisteredComponentGroupDeclaration(
    const char *InDomain,
    std::initializer_list<FRegisteredComponentFieldDeclaration> InFields)
    : Domain(ComponentAtom(InDomain)),
      Fields{TArray<FRegisteredComponentFieldDeclaration>(InFields)} {}

// --- Component text lookup (pure enum-to-string selector) ---

template <typename Value>
TComponentTextDeclaration<Value>::TComponentTextDeclaration(Value InExpected, const char *InText)
    : Expected(InExpected), Text(ComponentAtom(InText)) {}

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

template <typename Value>
FString ComponentText(
    Value Current,
    std::initializer_list<TComponentTextDeclaration<Value>> Declarations) {
  return ComponentText<Value>(Current,
                              TArray<TComponentTextDeclaration<Value>>(
                                  Declarations));
}

template <typename Value> FString RegisteredComponentText(Value Current) {
  return ComponentText<Value>(Current,
                              TComponentTextRegistry<Value>::Declarations());
}

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
