#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/Json/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<EBotEntityKind> {
  static const TArray<TComponentTextDeclaration<EBotEntityKind>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotEntityKind>>
        RegisteredCases = {{EBotEntityKind::Townsperson, "Townsperson"},
                           {EBotEntityKind::Horse, "Horse"}};
    return RegisteredCases;
  }
};

template <> struct TComponentTextRegistry<EBotAlignment> {
  static const TArray<TComponentTextDeclaration<EBotAlignment>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<EBotAlignment>>
        RegisteredCases = {{EBotAlignment::Friendly, "Friendly"},
                           {EBotAlignment::Neutral, "Neutral"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FBotEntity> {
  static const TArray<TComponentSourceValueFieldDeclaration<FBotEntity>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FBotEntity>>
        RegisteredFields = {{"Id", &FBotEntity::Id},
                            {"DisplayName", &FBotEntity::DisplayName},
                            {"Kind", &FBotEntity::Kind},
                            {"Alignment", &FBotEntity::Alignment},
                            {"Active", &FBotEntity::bActive}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FBotEntity> {
  ecs::FComponentValue operator()(const FBotEntity &Bot) const {
    return ComponentSourceValueMap(
        Bot, {"Id", "DisplayName", "Kind", "Alignment", "Active"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey BotEntityKey(const FString &Id) {
  return FString::Printf(TEXT("bot:%s"), *Id);
}

ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectBotEntityPayload &PayloadValue) {
            return BotEntityKey(PayloadValue.Bot.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Characters", "Bots"}, {"Systems", "Bots"}})),
          [](const FProjectBotEntityPayload &PayloadValue) -> const FBotEntity & {
            return PayloadValue.Bot;
          },
          RegisteredComponentGroups<FBotEntity>(
              {{"Components/Data", {"Id", "DisplayName", "Kind"}},
               {"Components/Bots", {"Alignment", "Active"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
