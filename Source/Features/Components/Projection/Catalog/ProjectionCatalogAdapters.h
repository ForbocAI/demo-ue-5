#pragma once

#include "Features/Components/Value/ComponentsValueAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <typename Source> struct TAppendComponentBindingProjection {
  const Source &SourceValue;

  /** User Story: As a component catalog consumer, I need to project one registered binding through a stable signature so catalog composition remains explicit and reusable. @fn template <typename ProjectBinding> TArray<FComponentProjectionBinding> operator()(const TArray<FComponentProjectionBinding> &Acc, const ProjectBinding &Project) const */
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

  /** User Story: As a component catalog consumer, I need to append a selected field projection through a stable signature so catalog composition remains explicit and reusable. @fn TArray<TComponentProjection<Source>> operator()(const TArray<TComponentProjection<Source>> &Acc, const FRegisteredComponentFieldDeclaration &Field) const */
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

/** User Story: As a component catalog consumer, I need to compose a registered component group through a stable signature so catalog projection remains deterministic. @fn template <typename Source> TArray<TComponentProjection<Source>> ComponentGroup(const FString &Domain, const FRegisteredComponentFieldSet &Fields) */
template <typename Source>
TArray<TComponentProjection<Source>>
ComponentGroup(const FString &Domain,
               const FRegisteredComponentFieldSet &Fields) {
  return func::fold_array<FRegisteredComponentFieldDeclaration,
                          TArray<TComponentProjection<Source>>>(
      Fields.Fields, TArray<TComponentProjection<Source>>(),
      TAppendSelectedComponentProjection<Source>{Domain});
}

template <typename Source> struct TAppendRegisteredComponentGroup {
  /** User Story: As a component catalog consumer, I need to append a registered group through a stable signature so group composition remains deterministic. @fn TArray<TComponentProjection<Source>> operator()(const TArray<TComponentProjection<Source>> &Acc, const FRegisteredComponentGroupDeclaration &Group) const */
  TArray<TComponentProjection<Source>>
  operator()(const TArray<TComponentProjection<Source>> &Acc,
             const FRegisteredComponentGroupDeclaration &Group) const {
    return func::append_values<TComponentProjection<Source>>(
        Acc, ComponentGroup<Source>(Group.Domain, Group.Fields));
  }
};

/**
 * User Story: As a component catalog consumer, I need to compose all registered groups through a stable signature so entity projection remains reusable.
 * @fn template <typename Source> TArray<TComponentProjection<Source>> RegisteredComponentGroups( std::initializer_list<FRegisteredComponentGroupDeclaration> Groups)
 */
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

/**
 * User Story: As a component catalog consumer, I need to project all component bindings through a stable signature so ECS updates remain deterministic.
 * @fn template <typename Source> TArray<FComponentProjectionBinding> ProjectComponentBindings( const Source &SourceValue, const TArray<TComponentProjection<Source>> &ProjectBindingsValue)
 */
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

/** User Story: As a component catalog consumer, I need to project a resource through a stable signature so ECS resource updates remain explicit. @fn ecs::FWorld ProjectResource(const FProjectResourcePayload &Payload) */
ecs::FWorld ProjectResource(const FProjectResourcePayload &Payload);

/** User Story: As a component catalog consumer, I need to project an entity through a stable signature so ECS entity updates remain explicit. @fn ecs::FWorld ProjectEntity(const FProjectEntityPayload &Payload) */
ecs::FWorld ProjectEntity(const FProjectEntityPayload &Payload);

/**
 * User Story: As a component catalog consumer, I need to project a typed entity catalog through a stable signature so domain catalogs remain composable.
 * @fn template <typename Payload, typename SelectEntity, typename SelectDomains, typename SelectSource, typename ComponentCatalog> ecs::FWorld ProjectEntityCatalog( const Payload &PayloadValue, const TEntityCatalogProjection<SelectEntity, SelectDomains, SelectSource, ComponentCatalog> &Projection)
 */
template <typename Payload, typename SelectEntity, typename SelectDomains,
          typename SelectSource, typename ComponentCatalog>
ecs::FWorld ProjectEntityCatalog(
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

/** User Story: As a component catalog consumer, I need to project a level-local point through a stable signature so spatial values remain serializable. @fn ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point) */
ecs::FComponentValue LocalPointValue(const FLevelLocalPoint &Point);

/** User Story: As a component catalog consumer, I need to project a rotation through a stable signature so spatial values remain serializable. @fn ecs::FComponentValue RotationValue(const FRotator &Rotation) */
ecs::FComponentValue RotationValue(const FRotator &Rotation);

/** User Story: As a component catalog consumer, I need to project level-local points through a stable signature so spatial lists remain serializable. @fn TArray<ecs::FComponentValue> LocalPointList(const TArray<FLevelLocalPoint> &Points) */
TArray<ecs::FComponentValue>
LocalPointList(const TArray<FLevelLocalPoint> &Points);

/** User Story: As a component catalog consumer, I need to project strings through a stable signature so string lists remain serializable. @fn TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values) */
TArray<ecs::FComponentValue> StringList(const TArray<FString> &Values);

} // namespace ComponentsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
