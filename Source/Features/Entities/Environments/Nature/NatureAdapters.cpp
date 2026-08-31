#include "Features/Entities/Environments/Nature/NatureAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EFeatureKind> {
  /** User Story: As a nature entity adapter, I need a declaration catalog for feature kinds so component projection remains data-driven. @fn static const TArray<TComponentTextDeclaration<EFeatureKind>> &Declarations() */
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
  /** User Story: As a nature entity adapter, I need source fields declared as data so one generic component composer can project each feature. @fn static const TArray<TComponentSourceValueFieldDeclaration<FFeatureSeed>> &Fields() */
  static const TArray<TComponentSourceValueFieldDeclaration<FFeatureSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FFeatureSeed>>
        SourceFields = ComponentSourceFieldDeclarations<FFeatureSeed>(
            {{"Id", &FFeatureSeed::Id},
             {"Name", &FFeatureSeed::Name},
             {"Kind", &FFeatureSeed::Kind},
             {"LocalLocation", &FFeatureSeed::Location},
             {"Scale", &FFeatureSeed::Scale}});
    return SourceFields;
  }
};

template <> struct TComponentSourceProjector<FFeatureSeed> {
  /** User Story: As a nature entity adapter, I need one source projection so registered component groups can select fields without custom branching. @fn ecs::FComponentValue operator()(const FFeatureSeed &NatureFeature) const */
  ecs::FComponentValue
  operator()(const FFeatureSeed &NatureFeature) const {
    return ComponentSourceValueMap(
        NatureFeature, {"Id", "Name", "Kind", "LocalLocation", "Scale"});
  }
};

} // namespace ComponentsAdapters

namespace NatureAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a nature entity consumer, I need a stable logical key so nature systems and views address one ECS entity. @fn ecs::EntityKey NatureEntityKey(const FString &Id) */
ecs::EntityKey NatureEntityKey(const FString &Id) {
  return EntitiesAdapters::PrefixedEntityKey({TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV7EA618734932), Id});
}

/** User Story: As a nature entity consumer, I need feature records projected through their domain-owned adapter so the root ECS world remains normalized. @fn ecs::FWorld ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) */
ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
            return NatureEntityKey(PayloadValue.EntityValue.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Environments", "Nature"},
                   {"Systems", "Nature"}})),
          [](const FProjectNatureFeatureEntityPayload &PayloadValue)
              -> const FFeatureSeed & {
            return PayloadValue.EntityValue;
          },
          RegisteredComponentGroups<FFeatureSeed>(
              {{"Components/Data", {"Id", "Name", "Kind"}},
               {"Components/Spatial", {"LocalLocation", "Scale"}}})});
}

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
