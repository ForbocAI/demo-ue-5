#pragma once

#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EFeatureKind> {
  /** User Story: As a nature seed project consumer, I need to invoke declarations through a stable signature so the nature seed project workflow remains explicit and composable. @fn static const TArray<TComponentTextDeclaration<EFeatureKind>> &Declarations() */
  static const TArray<TComponentTextDeclaration<EFeatureKind>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EFeatureKind>>
        RegisteredCases = {{EFeatureKind::Water, "Water"},
                           {EFeatureKind::Rock, "Rock"},
                           {EFeatureKind::TreeGrove, "TreeGrove"},
                           {EFeatureKind::Shrub, "Shrub"},
                           {EFeatureKind::PCGMarker, "PCGMarker"},
                           {EFeatureKind::WaterSystemMarker,
                            "WaterSystemMarker"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FFeatureSeed> {
  /** User Story: As a nature seed project consumer, I need to invoke fields through a stable signature so the nature seed project workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FFeatureSeed>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FFeatureSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FFeatureSeed>>
        SourceFields = ComponentSourceFieldDeclarations<FFeatureSeed>(
            {{"Id", &FFeatureSeed::Id},
             {"Name", &FFeatureSeed::Name},
             {"Kind", &FFeatureSeed::Kind},
             {"LocalLocation", &FFeatureSeed::Location},
             {"Scale", &FFeatureSeed::Scale}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FFeatureSeed> {
  /** User Story: As a nature seed project consumer, I need to invoke the callable value through a stable signature so the nature seed project workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FFeatureSeed &NatureFeature) const */
  ecs::FComponentValue
  operator()(const FFeatureSeed &NatureFeature) const {
    return ComponentSourceValueMap(
        NatureFeature, {"Id", "Name", "Kind", "LocalLocation", "Scale"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a nature seed project consumer, I need to invoke nature entity key through a stable signature so the nature seed project workflow remains explicit and composable. @fn ecs::EntityKey NatureEntityKey(const FString &Id) */
ecs::EntityKey NatureEntityKey(const FString &Id) {
  return FString::Printf(TEXT("nature:%s"), *Id);
}

/** User Story: As a nature seed project consumer, I need to invoke project nature feature through a stable signature so the nature seed project workflow remains explicit and composable. @fn ecs::FWorld ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) */
ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
            return NatureEntityKey(PayloadValue.Feature.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Environments", "Nature"},
                   {"Systems", "Nature"}})),
          [](const FProjectNatureFeatureEntityPayload &PayloadValue)
              -> const FFeatureSeed & {
            return PayloadValue.Feature;
          },
          RegisteredComponentGroups<FFeatureSeed>(
              {{"Components/Data", {"Id", "Name", "Kind"}},
               {"Components/Spatial", {"LocalLocation", "Scale"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
