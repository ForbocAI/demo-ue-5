#pragma once

#include "Core/ecs.hpp"

#include "Features/Components/Spatial/LevelLayoutSlice.h"

#include <initializer_list>
#include <type_traits>

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

FComponentProjectionBinding
ComponentBinding(const ecs::ComponentType &Type,
                 const ecs::FComponentValue &Value);

struct FProjectEntityPayload {
  ecs::FWorld World;
  ecs::EntityKey Entity;
  TArray<TArray<FString>> Domains;
  TArray<FComponentProjectionBinding> Components;
};

ecs::FComponentValue ProjectComponentValue(const ecs::FComponentValue &Value);
ecs::FComponentValue ProjectComponentValue(const FString &Value);
ecs::FComponentValue ProjectComponentValue(bool Value);
ecs::FComponentValue ProjectComponentValue(int32 Value);
ecs::FComponentValue ProjectComponentValue(int64 Value);
ecs::FComponentValue ProjectComponentValue(float Value);
ecs::FComponentValue ProjectComponentValue(double Value);
ecs::FComponentValue ProjectComponentValue(const FVector &Value);
ecs::FComponentValue
ProjectComponentValue(const TArray<ecs::FComponentValue> &Value);
ecs::FComponentValue ProjectComponentValue(const FLevelLocalPoint &Value);
ecs::FComponentValue ProjectComponentValue(const FRotator &Value);
ecs::FComponentValue ProjectComponentValue(const TArray<FLevelLocalPoint> &Value);
ecs::FComponentValue ProjectComponentValue(const TArray<FString> &Value);

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

struct FComponentDomainDeclaration {
  TArray<FString> Segments;

  FComponentDomainDeclaration(std::initializer_list<const char *> InSegments)
      : Segments(ComponentAtoms(InSegments)) {}
};

inline TArray<TArray<FString>>
ComponentDomains(std::initializer_list<FComponentDomainDeclaration> Domains) {
  const TArray<FComponentDomainDeclaration> DomainList(Domains);
  return func::map_array<FComponentDomainDeclaration, TArray<FString>>(
      DomainList, [](const FComponentDomainDeclaration &Domain) {
        return Domain.Segments;
      });
}

struct FComponentValueField {
  FString Name;
  ecs::FComponentValue Value;

  template <typename ValueType>
  FComponentValueField(const char *InName, const ValueType &InValue)
      : Name(ComponentAtom(InName)), Value(ProjectComponentValue(InValue)) {}
};

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

template <typename Value> struct TComponentTextDeclaration {
  Value Expected;
  FString Text;

  TComponentTextDeclaration(Value InExpected, const char *InText)
      : Expected(InExpected), Text(ComponentAtom(InText)) {}
};

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

template <typename Value> struct TComponentTextRegistry;

template <typename Value> FString RegisteredComponentText(Value Current) {
  return ComponentText<Value>(Current,
                              TComponentTextRegistry<Value>::Declarations());
}

template <typename Value>
typename std::enable_if<std::is_enum<Value>::value,
                        ecs::FComponentValue>::type
ProjectComponentValue(Value ValueText) {
  return ProjectComponentValue(RegisteredComponentText<Value>(ValueText));
}

template <typename Source> struct TComponentSourceProjector;

template <typename Source>
ecs::FComponentValue ProjectComponentSourceValue(const Source &SourceValue) {
  return TComponentSourceProjector<Source>()(SourceValue);
}

template <typename Source> struct TComponentSourceValueFieldDeclaration {
  FString Name;
  TFunction<ecs::FComponentValue(const Source &)> Project;

  TComponentSourceValueFieldDeclaration()
      : Name(),
        Project([](const Source &) {
          return ProjectComponentValue(FString());
        }) {}

  template <typename Value>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member)
      : Name(ComponentAtom(InName)),
        Project([Member](const Source &SourceValue) {
          return ProjectComponentValue(SourceValue.*Member);
        }) {}

  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(Value))
      : Name(ComponentAtom(InName)),
        Project([Member, Convert](const Source &SourceValue) {
          return ProjectComponentValue(Convert(SourceValue.*Member));
        }) {}

  template <typename Value, typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Value Source::*Member,
                                        Output (*Convert)(const Value &))
      : Name(ComponentAtom(InName)),
        Project([Member, Convert](const Source &SourceValue) {
          return ProjectComponentValue(Convert(SourceValue.*Member));
        }) {}

  template <typename Output>
  TComponentSourceValueFieldDeclaration(const char *InName,
                                        Output (*ProjectValue)(const Source &))
      : Name(ComponentAtom(InName)),
        Project([ProjectValue](const Source &SourceValue) {
          return ProjectComponentValue(ProjectValue(SourceValue));
        }) {}
};

template <typename Source> struct TComponentSourceValueFieldRegistry;

template <typename Source>
inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields(
    const Source &SourceValue,
    std::initializer_list<TComponentSourceValueFieldDeclaration<Source>>
        Fields) {
  const TArray<TComponentSourceValueFieldDeclaration<Source>> FieldList(Fields);
  return func::fold_array<TComponentSourceValueFieldDeclaration<Source>,
                          TMap<FString, ecs::FComponentValue>>(
      FieldList, TMap<FString, ecs::FComponentValue>(),
      [&SourceValue](const TMap<FString, ecs::FComponentValue> &Acc,
                     const TComponentSourceValueFieldDeclaration<Source>
                         &Field) {
        TMap<FString, ecs::FComponentValue> Next = Acc;
        Next.Add(Field.Name, Field.Project(SourceValue));
        return Next;
      });
}

template <typename Source>
inline func::Maybe<TComponentSourceValueFieldDeclaration<Source>>
FindComponentSourceValueField(const char *FieldAtom) {
  const FString FieldName = ComponentAtom(FieldAtom);
  const TArray<TComponentSourceValueFieldDeclaration<Source>> &Fields =
      TComponentSourceValueFieldRegistry<Source>::Fields();
  return func::fold_array<
      TComponentSourceValueFieldDeclaration<Source>,
      func::Maybe<TComponentSourceValueFieldDeclaration<Source>>>(
      Fields, func::nothing<TComponentSourceValueFieldDeclaration<Source>>(),
      [FieldName](const func::Maybe<TComponentSourceValueFieldDeclaration<
                      Source>> &Current,
                  const TComponentSourceValueFieldDeclaration<Source> &Field) {
        return Current.hasValue || Field.Name != FieldName
                   ? Current
                   : func::just(Field);
      });
}

template <typename Source>
inline TMap<FString, ecs::FComponentValue> ComponentSourceValueFields(
    const Source &SourceValue, const TArray<const char *> &FieldAtoms) {
  return func::fold_array<const char *, TMap<FString, ecs::FComponentValue>>(
      FieldAtoms, TMap<FString, ecs::FComponentValue>(),
      [&SourceValue](const TMap<FString, ecs::FComponentValue> &Acc,
                     const char *FieldAtom) {
        const func::Maybe<TComponentSourceValueFieldDeclaration<Source>>
            Field = FindComponentSourceValueField<Source>(FieldAtom);
        check(Field.hasValue);
        TMap<FString, ecs::FComponentValue> Next = Acc;
        Next.Add(Field.value.Name, Field.value.Project(SourceValue));
        return Next;
      });
}

template <typename Source>
inline ecs::FComponentValue ComponentSourceValueMap(
    const Source &SourceValue,
    std::initializer_list<TComponentSourceValueFieldDeclaration<Source>>
        Fields) {
  return ecs::createMapComponentValue(
      ComponentSourceValueFields<Source>(SourceValue, Fields));
}

template <typename Source>
inline ecs::FComponentValue ComponentSourceValueMap(
    const Source &SourceValue, std::initializer_list<const char *> FieldAtoms) {
  return ecs::createMapComponentValue(ComponentSourceValueFields<Source>(
      SourceValue, TArray<const char *>(FieldAtoms)));
}

template <typename Source> struct TAppendComponentBindingProjection {
  const Source &SourceValue;

  template <typename ProjectBinding>
  TArray<FComponentProjectionBinding>
  operator()(const TArray<FComponentProjectionBinding> &Acc,
             const ProjectBinding &Project) const {
    return func::append_value<FComponentProjectionBinding>(
        Acc, Project(SourceValue));
  }
};

template <typename Source>
using TComponentProjection =
    std::function<FComponentProjectionBinding(const Source &)>;

struct FRegisteredComponentFieldDeclaration {
  FString Name;
  TArray<FString> Path;

  FRegisteredComponentFieldDeclaration() {}

  FRegisteredComponentFieldDeclaration(const char *InName)
      : Name(ComponentAtom(InName)), Path(ComponentAtoms({InName})) {}

  FRegisteredComponentFieldDeclaration(
      const char *InName, std::initializer_list<const char *> InPath)
      : Name(ComponentAtom(InName)), Path(ComponentAtoms(InPath)) {}
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
      std::initializer_list<FRegisteredComponentFieldDeclaration> InFields)
      : Domain(ComponentAtom(InDomain)),
        Fields{TArray<FRegisteredComponentFieldDeclaration>(InFields)} {}
};

inline ecs::ComponentType ComponentPath(const FString &Domain,
                                        const FString &Name);

inline func::Maybe<ecs::FComponentValue>
SelectComponentValuePath(const ecs::FComponentValue &Value,
                         const TArray<FString> &Path);

template <typename Source> struct TAppendSelectedComponentProjection {
  FString Domain;

  TArray<TComponentProjection<Source>>
  operator()(const TArray<TComponentProjection<Source>> &Acc,
             const FRegisteredComponentFieldDeclaration &Field) const {
    const FString ComponentDomain = Domain;
    return func::append_value<TComponentProjection<Source>>(
        Acc, TComponentProjection<Source>(
                 [ComponentDomain, Field](const Source &SourceValue) {
                   const func::Maybe<ecs::FComponentValue> Selected =
                       SelectComponentValuePath(
                           ProjectComponentSourceValue(SourceValue),
                           Field.Path);
                   checkf(Selected.hasValue,
                          TEXT("Missing component projection field: %s"),
                          *Field.Name);
                   return ComponentBinding(
                       ComponentPath(ComponentDomain, Field.Name),
                       Selected.value);
                 }));
  }
};

inline FRegisteredComponentFields
RegisteredComponentFields(
    std::initializer_list<FRegisteredComponentFieldDeclaration> Declarations) {
  return {TArray<FRegisteredComponentFieldDeclaration>(Declarations)};
}

inline ecs::ComponentType ComponentPath(const FString &Domain,
                                        const FString &Name) {
  return FString::Printf(TEXT("%s/%s"), *Domain, *Name);
}

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

template <typename Source>
TArray<TComponentProjection<Source>>
ComponentGroup(const FString &Domain,
               const FRegisteredComponentFields &Fields) {
  return func::fold_array<FRegisteredComponentFieldDeclaration,
                          TArray<TComponentProjection<Source>>>(
      Fields.Fields, TArray<TComponentProjection<Source>>(),
      TAppendSelectedComponentProjection<Source>{Domain});
}

template <typename Source> struct TAppendRegisteredComponentGroup {
  TArray<TComponentProjection<Source>>
  operator()(const TArray<TComponentProjection<Source>> &Acc,
             const FRegisteredComponentGroupDeclaration &Group) const {
    return func::append_values<TComponentProjection<Source>>(
        Acc, ComponentGroup<Source>(Group.Domain, Group.Fields));
  }
};

template <typename Source>
TArray<TComponentProjection<Source>>
RegisteredComponentGroups(
    std::initializer_list<FRegisteredComponentGroupDeclaration> Groups) {
  const TArray<FRegisteredComponentGroupDeclaration> GroupList(Groups);
  return func::fold_array<FRegisteredComponentGroupDeclaration,
                          TArray<TComponentProjection<Source>>>(
      GroupList, TArray<TComponentProjection<Source>>(),
      TAppendRegisteredComponentGroup<Source>());
}

template <typename Source>
TArray<FComponentProjectionBinding>
ProjectComponentBindings(
    const Source &SourceValue,
    const TArray<TComponentProjection<Source>> &ProjectBindingsValue) {
  return func::fold_array<TComponentProjection<Source>,
                          TArray<FComponentProjectionBinding>>(
      ProjectBindingsValue, TArray<FComponentProjectionBinding>(),
      TAppendComponentBindingProjection<Source>{SourceValue});
}

ecs::FWorld ProjectResource(const FProjectResourcePayload &Payload);
ecs::FWorld ProjectEntity(const FProjectEntityPayload &Payload);

template <typename Payload, typename SelectEntity, typename SelectDomains,
          typename SelectSource, typename ComponentCatalog>
struct TEntityCatalogProjection {
  SelectEntity SelectEntityValue;
  SelectDomains SelectDomainValues;
  SelectSource SelectSourceValue;
  ComponentCatalog Components;
};

template <typename Payload, typename SelectEntity, typename SelectDomains,
          typename SelectSource, typename ComponentCatalog>
ecs::FWorld ProjectPayloadEntityCatalogWith(
    const Payload &PayloadValue,
    const TEntityCatalogProjection<Payload, SelectEntity, SelectDomains,
                                   SelectSource, ComponentCatalog>
        &Projection) {
  const ecs::EntityKey Entity = Projection.SelectEntityValue(PayloadValue);
  return ProjectEntity(
      {PayloadValue.World, Entity,
       Projection.SelectDomainValues(PayloadValue),
       ProjectComponentBindings(Projection.SelectSourceValue(PayloadValue),
                                Projection.Components)});
}

ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point);
ecs::FComponentValue RotationValue(const FRotator &Rotation);
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points);
TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values);

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
