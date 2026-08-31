#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EBotEntityKind> {
  /** User Story: As a entities characters bots consumer, I need to invoke declarations through a stable signature so the entities characters bots workflow remains explicit and composable. @fn static const TArray<TComponentTextDeclaration<EBotEntityKind>> &Declarations() */
  static const TArray<TComponentTextDeclaration<EBotEntityKind>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotEntityKind>>
        RegisteredCases = {{EBotEntityKind::Townsperson, "Townsperson"},
                           {EBotEntityKind::Horse, "Horse"}};
    return RegisteredCases;
  }
};

template <> struct TComponentTextRegistry<EBotAlignment> {
  /** User Story: As a entities characters bots consumer, I need to invoke declarations through a stable signature so the entities characters bots workflow remains explicit and composable. @fn static const TArray<TComponentTextDeclaration<EBotAlignment>> &Declarations() */
  static const TArray<TComponentTextDeclaration<EBotAlignment>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotAlignment>>
        RegisteredCases = {{EBotAlignment::Friendly, "Friendly"},
                           {EBotAlignment::Neutral, "Neutral"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotEntity> {
  /** User Story: As a entities characters bots consumer, I need to invoke fields through a stable signature so the entities characters bots workflow remains explicit and composable. @fn static const TArray<TComponentSourceValueFieldDeclaration<FBotEntity>> &Fields() */
  static const TArray<TComponentSourceValueFieldDeclaration<FBotEntity>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FBotEntity>>
        SourceFields = ComponentSourceFieldDeclarations<FBotEntity>(
            {{"Id", &FBotEntity::Id},
             {"DisplayName", &FBotEntity::DisplayName},
             {"Kind", &FBotEntity::Kind},
             {"Alignment", &FBotEntity::Alignment},
             {"Active", &FBotEntity::bActive}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FBotEntity> {
  /** User Story: As a entities characters bots consumer, I need to invoke the callable value through a stable signature so the entities characters bots workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FBotEntity &Bot) const */
  ecs::FComponentValue operator()(const FBotEntity &Bot) const {
    return ComponentSourceValueMap(
        Bot, {"Id", "DisplayName", "Kind", "Alignment", "Active"});
  }
};

} // namespace ComponentsAdapters

namespace BotsAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a entities characters bots consumer, I need to invoke bot entity key through a stable signature so the entities characters bots workflow remains explicit and composable. @fn ecs::EntityKey BotEntityKey(const FString &Id) */
ecs::EntityKey BotEntityKey(const FString &Id) {
  return EntitiesAdapters::PrefixedEntityKey({TEXT(FORBOCAI_DEMOUE5_AUTHORED_STRINGV24A1D1331AB5), Id});
}

/** User Story: As a entities characters bots consumer, I need to invoke project bot through a stable signature so the entities characters bots workflow remains explicit and composable. @fn ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) */
ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectBotEntityPayload &PayloadValue) {
            return BotEntityKey(PayloadValue.EntityValue.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Characters", "Bots"}, {"Systems", "Bots"}})),
          [](const FProjectBotEntityPayload &PayloadValue) -> const FBotEntity & {
            return PayloadValue.EntityValue;
          },
          RegisteredComponentGroups<FBotEntity>(
              {{"Components/Data", {"Id", "DisplayName", "Kind"}},
               {"Components/Bots", {"Alignment", "Active"}}})});
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
