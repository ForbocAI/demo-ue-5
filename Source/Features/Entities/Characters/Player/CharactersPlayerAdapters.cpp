#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

namespace {

bool HasActionId(func::Maybe<FString> ActionId) {
  return ActionId.hasValue;
}

FString ActionIdText(func::Maybe<FString> ActionId) {
  return ActionId.hasValue ? ActionId.value : FString();
}

} // namespace

template <> struct TComponentSourceValueFieldRegistry<FPlayerState> {
  static const TArray<TComponentSourceValueFieldDeclaration<FPlayerState>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FPlayerState>>
        SourceFields = ComponentSourceFieldDeclarations<FPlayerState>({
            {"Ready", &FPlayerState::bReady},
            {"HasActionId", &FPlayerState::ActionId, HasActionId},
            {"ActionId", &FPlayerState::ActionId, ActionIdText}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FPlayerState> {
  ecs::FComponentValue operator()(const FPlayerState &Player) const {
    return ComponentSourceValueMap(
        Player, {"Ready", "HasActionId", "ActionId"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey PlayerEntityKey() { return TEXT("player:local"); }

ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectPlayerEntityPayload &) { return PlayerEntityKey(); },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Characters", "Player"}})),
          [](const FProjectPlayerEntityPayload &PayloadValue)
              -> const FPlayerState & {
            return PayloadValue.Player;
          },
          RegisteredComponentGroups<FPlayerState>(
              {{"Components/Lifecycle", {"Ready"}},
               {"Components/Data", {"HasActionId", "ActionId"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
