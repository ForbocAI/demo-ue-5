#pragma once

#include "Features/Components/ComponentsAdaptersTypes.h"
#include "Features/Components/ComponentsAdaptersSelectors.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

// ECS boundary adapters: converting application domains into ECS components and projection.
// Owns ProjectComponentValue overloads and ECS binding mapping.

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

FComponentProjectionBinding
ComponentBinding(const ecs::ComponentType &Type,
                 const ecs::FComponentValue &Value);

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

template <typename ValueType>
FComponentValueField::FComponentValueField(const char *InName, const ValueType &InValue)
    : Name(ComponentAtom(InName)), Value(ProjectComponentValue(InValue)) {}

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

template <typename Source>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration()
    : Name(),
      Project([](const Source &) {
        return ProjectComponentValue(FString());
      }) {}

template <typename Source>
template <typename Value>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member)
    : Name(ComponentAtom(InName)),
      Project([Member](const Source &SourceValue) {
        return ProjectComponentValue(SourceValue.*Member);
      }) {}

template <typename Source>
template <typename Value, typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member, Output (*Convert)(Value))
    : Name(ComponentAtom(InName)),
      Project([Member, Convert](const Source &SourceValue) {
        return ProjectComponentValue(Convert(SourceValue.*Member));
      }) {}

template <typename Source>
template <typename Value, typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Value Source::*Member,
    Output (*Convert)(const Value &))
    : Name(ComponentAtom(InName)),
      Project([Member, Convert](const Source &SourceValue) {
        return ProjectComponentValue(Convert(SourceValue.*Member));
      }) {}

template <typename Source>
template <typename Output>
TComponentSourceValueFieldDeclaration<Source>::TComponentSourceValueFieldDeclaration(
    const char *InName, Output (*ProjectValue)(const Source &))
    : Name(ComponentAtom(InName)),
      Project([ProjectValue](const Source &SourceValue) {
        return ProjectComponentValue(ProjectValue(SourceValue));
      }) {}

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
ecs::FWorld ProjectPayloadEntityCatalogWith(
    const Payload &PayloadValue,
    const TEntityCatalogProjection<SelectEntity, SelectDomains,
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
