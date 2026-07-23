#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

namespace {

/** User Story: As a entities characters player consumer, I need to invoke has action id through a stable signature so the entities characters player workflow remains explicit and composable. @fn bool HasActionId(func::Maybe<FString> ActionId) */
bool HasActionId(func::Maybe<FString> ActionId) {
  return ActionId.hasValue;
}

/** User Story: As a entities characters player consumer, I need to invoke action id text through a stable signature so the entities characters player workflow remains explicit and composable. @fn FString ActionIdText(func::Maybe<FString> ActionId) */
FString ActionIdText(func::Maybe<FString> ActionId) {
  return ActionId.hasValue ? ActionId.value : FString();
}

} // namespace

template <> struct TComponentSourceValueFieldRegistry<FPlayerState> {
  /** User Story: As a entities characters player consumer, I need to invoke fields through a stable signature so the entities characters player workflow remains explicit and composable. @fn static const TArray<TComponentSourceValueFieldDeclaration<FPlayerState>> &Fields() */
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
  /** User Story: As a entities characters player consumer, I need to invoke the callable value through a stable signature so the entities characters player workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FPlayerState &Player) const */
  ecs::FComponentValue operator()(const FPlayerState &Player) const {
    return ComponentSourceValueMap(
        Player, {"Ready", "HasActionId", "ActionId"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

/** User Story: As a entities characters player consumer, I need to invoke player entity key through a stable signature so the entities characters player workflow remains explicit and composable. @fn ecs::EntityKey PlayerEntityKey() */
ecs::EntityKey PlayerEntityKey() { return TEXT("player:local"); }

/** User Story: As a entities characters player consumer, I need to invoke project player through a stable signature so the entities characters player workflow remains explicit and composable. @fn ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) */
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
