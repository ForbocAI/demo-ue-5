#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

namespace {

bool HasLastActionId(func::Maybe<FString> LastActionId) {
  return LastActionId.hasValue;
}

FString LastActionIdText(func::Maybe<FString> LastActionId) {
  return LastActionId.hasValue ? LastActionId.value : FString();
}

} // namespace

template <> struct TComponentSourceValueFieldRegistry<FPlayerState> {
  static const TArray<TComponentSourceValueFieldDeclaration<FPlayerState>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<FPlayerState>>
        RegisteredFields = {
            {"Ready", &FPlayerState::bReady},
            {"HasLastActionId", &FPlayerState::LastActionId, HasLastActionId},
            {"LastActionId", &FPlayerState::LastActionId, LastActionIdText}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FPlayerState> {
  ecs::FComponentValue operator()(const FPlayerState &Player) const {
    return ComponentSourceValueMap(
        Player, {"Ready", "HasLastActionId", "LastActionId"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey PlayerEntityKey() { return TEXT("player:local"); }

ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
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
               {"Components/Data", {"HasLastActionId", "LastActionId"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
