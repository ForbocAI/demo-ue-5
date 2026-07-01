#include "Features/Entities/EntitiesAdapters.h"

#include "Core/ue_fp.hpp"
#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace {

/**
 * @brief Converts landmark enum state into stable ECS text.
 * @signature FString LandmarkKindText(ELandmarkKind Kind)
 *
 * User Story: As entity projection code, landmark enum cases should be
 * explicit and fail on impossible values instead of falling back silently.
 */
FString LandmarkKindText(ELandmarkKind Kind) {
  const func::Maybe<FString> Text =
      func::multi_match<ELandmarkKind, FString>(
          Kind, {func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Building),
                     [](const ELandmarkKind &) { return FString(TEXT("Building")); }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Road),
                     [](const ELandmarkKind &) { return FString(TEXT("Road")); }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Creek),
                     [](const ELandmarkKind &) { return FString(TEXT("Creek")); }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::TerrainMarker),
                     [](const ELandmarkKind &) {
                       return FString(TEXT("TerrainMarker"));
                     }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Mine),
                     [](const ELandmarkKind &) { return FString(TEXT("Mine")); }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Cemetery),
                     [](const ELandmarkKind &) { return FString(TEXT("Cemetery")); }),
                 func::when<ELandmarkKind, FString>(
                     func::equals(ELandmarkKind::Park),
                     [](const ELandmarkKind &) { return FString(TEXT("Park")); })});
  check(Text.hasValue);
  return Text.value;
}

/**
 * @brief Converts nature enum state into stable ECS text.
 * @signature FString NatureKindText(ENatureFeatureKind Kind)
 *
 * User Story: As entity projection code, nature enum cases should be explicit
 * and fail on impossible values instead of falling back silently.
 */
FString NatureKindText(ENatureFeatureKind Kind) {
  const func::Maybe<FString> Text =
      func::multi_match<ENatureFeatureKind, FString>(
          Kind, {func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::Water),
                     [](const ENatureFeatureKind &) { return FString(TEXT("Water")); }),
                 func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::Rock),
                     [](const ENatureFeatureKind &) { return FString(TEXT("Rock")); }),
                 func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::TreeGrove),
                     [](const ENatureFeatureKind &) {
                       return FString(TEXT("TreeGrove"));
                     }),
                 func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::Shrub),
                     [](const ENatureFeatureKind &) { return FString(TEXT("Shrub")); }),
                 func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::PCGMarker),
                     [](const ENatureFeatureKind &) {
                       return FString(TEXT("PCGMarker"));
                     }),
                 func::when<ENatureFeatureKind, FString>(
                     func::equals(ENatureFeatureKind::WaterSystemMarker),
                     [](const ENatureFeatureKind &) {
                       return FString(TEXT("WaterSystemMarker"));
                     })});
  check(Text.hasValue);
  return Text.value;
}

/**
 * @brief Converts townsperson interaction intent into stable ECS text.
 * @signature FString InteractionIntentText(ETownspersonInteractionIntent Intent)
 *
 * User Story: As entity projection code, townsperson intent enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString InteractionIntentText(ETownspersonInteractionIntent Intent) {
  const func::Maybe<FString> Text =
      func::multi_match<ETownspersonInteractionIntent, FString>(
          Intent, {func::when<ETownspersonInteractionIntent, FString>(
                       func::equals(ETownspersonInteractionIntent::General),
                       [](const ETownspersonInteractionIntent &) {
                         return FString(TEXT("General"));
                       }),
                   func::when<ETownspersonInteractionIntent, FString>(
                       func::equals(ETownspersonInteractionIntent::Dialogue),
                       [](const ETownspersonInteractionIntent &) {
                         return FString(TEXT("Dialogue"));
                       }),
                   func::when<ETownspersonInteractionIntent, FString>(
                       func::equals(ETownspersonInteractionIntent::Memory),
                       [](const ETownspersonInteractionIntent &) {
                         return FString(TEXT("Memory"));
                       }),
                   func::when<ETownspersonInteractionIntent, FString>(
                       func::equals(
                           ETownspersonInteractionIntent::CombatValidation),
                       [](const ETownspersonInteractionIntent &) {
                         return FString(TEXT("CombatValidation"));
                       })});
  check(Text.hasValue);
  return Text.value;
}

/**
 * @brief Converts bot kind enum state into stable ECS text.
 * @signature FString BotKindText(EBotEntityKind Kind)
 *
 * User Story: As bot entity projection code, bot kind enum cases should be
 * explicit and fail on impossible values instead of falling back silently.
 */
FString BotKindText(EBotEntityKind Kind) {
  const func::Maybe<FString> Text =
      func::multi_match<EBotEntityKind, FString>(
          Kind, {func::when<EBotEntityKind, FString>(
                     func::equals(EBotEntityKind::Townsperson),
                     [](const EBotEntityKind &) {
                       return FString(TEXT("Townsperson"));
                     }),
                 func::when<EBotEntityKind, FString>(
                     func::equals(EBotEntityKind::Horse),
                     [](const EBotEntityKind &) { return FString(TEXT("Horse")); })});
  check(Text.hasValue);
  return Text.value;
}

/**
 * @brief Converts bot alignment enum state into stable ECS text.
 * @signature FString BotAlignmentText(EBotAlignment Alignment)
 *
 * User Story: As bot entity projection code, bot alignment enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString BotAlignmentText(EBotAlignment Alignment) {
  const func::Maybe<FString> Text =
      func::multi_match<EBotAlignment, FString>(
          Alignment, {func::when<EBotAlignment, FString>(
                          func::equals(EBotAlignment::Friendly),
                          [](const EBotAlignment &) {
                            return FString(TEXT("Friendly"));
                          }),
                      func::when<EBotAlignment, FString>(
                          func::equals(EBotAlignment::Neutral),
                          [](const EBotAlignment &) {
                            return FString(TEXT("Neutral"));
                          })});
  check(Text.hasValue);
  return Text.value;
}

TArray<TArray<FString>> LandmarkDomains() {
  return {{TEXT("Entities"), TEXT("Environments"), TEXT("Landmarks")},
          {TEXT("Systems"), TEXT("Landmarks")}};
}

TArray<TArray<FString>> NatureDomains() {
  return {{TEXT("Entities"), TEXT("Environments"), TEXT("Nature")},
          {TEXT("Systems"), TEXT("Nature")}};
}

TArray<TArray<FString>> TownspersonDomains() {
  return {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
           TEXT("Townspeople")},
          {TEXT("Systems"), TEXT("Bots"), TEXT("Townspeople")}};
}

TArray<TArray<FString>> HorseDomains() {
  return {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
           TEXT("Horses")},
          {TEXT("Systems"), TEXT("Bots"), TEXT("Horses")}};
}

TArray<TArray<FString>> BotDomains() {
  return {{TEXT("Entities"), TEXT("Characters"), TEXT("Bots")},
          {TEXT("Systems"), TEXT("Bots")}};
}

TArray<TArray<FString>> PlayerDomains() {
  return {{TEXT("Entities"), TEXT("Characters"), TEXT("Player")}};
}

FString LastActionIdText(const func::Maybe<FString> &LastActionId) {
  return LastActionId.hasValue ? LastActionId.value : FString();
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FLandmark> {
  ecs::FComponentValue operator()(const FLandmark &Landmark) const {
    return ComponentValueMap({{"Id", Landmark.Id},
                              {"Label", Landmark.Label},
                              {"Kind", LandmarkKindText(Landmark.Kind)},
                              {"Location", Landmark.Location},
                              {"Scale", Landmark.Scale}});
  }
};

template <>
struct TComponentSourceProjector<FNatureFeatureSeed> {
  ecs::FComponentValue
  operator()(const FNatureFeatureSeed &NatureFeature) const {
    return ComponentValueMap({{"Id", NatureFeature.Id},
                              {"Name", NatureFeature.Name},
                              {"Kind", NatureKindText(NatureFeature.Kind)},
                              {"LocalLocation", NatureFeature.Location},
                              {"Scale", NatureFeature.Scale}});
  }
};

template <>
struct TComponentSourceProjector<FTownspersonSeed> {
  ecs::FComponentValue
  operator()(const FTownspersonSeed &Townsperson) const {
    return ComponentValueMap(
        {{"Id", Townsperson.Id},
         {"Name", Townsperson.Name},
         {"Role", Townsperson.Role},
         {"Persona", Townsperson.Persona},
         {"InteractionPrompt", Townsperson.InteractionPrompt},
         {"DefaultPlayerLine", Townsperson.DefaultPlayerLine},
         {"PinnedResponse", Townsperson.PinnedResponse},
         {"InteractionIntent",
          InteractionIntentText(Townsperson.InteractionIntent)},
         {"PatrolRoute", Townsperson.PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FHorseRouteSeed> {
  ecs::FComponentValue operator()(const FHorseRouteSeed &Horse) const {
    return ComponentValueMap({{"Id", Horse.Id},
                              {"Name", Horse.Name},
                              {"MountedRider", Horse.bMountedRider},
                              {"PatrolRoute", Horse.PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FBotEntity> {
  ecs::FComponentValue operator()(const FBotEntity &Bot) const {
    return ComponentValueMap({{"Id", Bot.Id},
                              {"DisplayName", Bot.DisplayName},
                              {"Kind", BotKindText(Bot.Kind)},
                              {"Alignment", BotAlignmentText(Bot.Alignment)},
                              {"Active", Bot.bActive}});
  }
};

template <>
struct TComponentSourceProjector<FPlayerState> {
  ecs::FComponentValue operator()(const FPlayerState &Player) const {
    return ComponentValueMap(
        {{"Ready", Player.bReady},
         {"HasLastActionId", Player.LastActionId.hasValue},
         {"LastActionId", LastActionIdText(Player.LastActionId)}});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

ecs::EntityKey BotEntityKey(const FString &Id) {
  return FString::Printf(TEXT("bot:%s"), *Id);
}

ecs::EntityKey PlayerEntityKey() { return TEXT("player:local"); }

ecs::EntityKey LandmarkEntityKey(const FString &Id) {
  return FString::Printf(TEXT("landmark:%s"), *Id);
}

ecs::EntityKey NatureEntityKey(const FString &Id) {
  return FString::Printf(TEXT("nature:%s"), *Id);
}

ecs::FWorld ProjectLandmark(const FProjectLandmarkEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectLandmarkEntityPayload &PayloadValue) {
        return LandmarkEntityKey(PayloadValue.Landmark.Id);
      },
      func::constant<TArray<TArray<FString>>>(LandmarkDomains()),
      [](const FProjectLandmarkEntityPayload &PayloadValue)
          -> const FLandmark & {
        return PayloadValue.Landmark;
      },
      RegisteredComponentGroups<FLandmark>(
          {{"Components/Data", {"Id", "Label", "Kind"}},
           {"Components/Spatial", {"Location", "Scale"}}}));
}

ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
        return NatureEntityKey(PayloadValue.Feature.Id);
      },
      func::constant<TArray<TArray<FString>>>(NatureDomains()),
      [](const FProjectNatureFeatureEntityPayload &PayloadValue)
          -> const FNatureFeatureSeed & {
        return PayloadValue.Feature;
      },
      RegisteredComponentGroups<FNatureFeatureSeed>(
          {{"Components/Data", {"Id", "Name", "Kind"}},
           {"Components/Spatial", {"LocalLocation", "Scale"}}}));
}

ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectTownspersonEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Townsperson.Id);
      },
      func::constant<TArray<TArray<FString>>>(TownspersonDomains()),
      [](const FProjectTownspersonEntityPayload &PayloadValue)
          -> const FTownspersonSeed & {
        return PayloadValue.Townsperson;
      },
      RegisteredComponentGroups<FTownspersonSeed>(
          {{"Components/Data", {"Id", "Name"}},
           {"Components/Bots",
            {"Role", "Persona", "InteractionPrompt", "DefaultPlayerLine",
             "PinnedResponse", "InteractionIntent"}},
           {"Components/Spatial", {"PatrolRoute"}}}));
}

ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectHorseEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Horse.Id);
      },
      func::constant<TArray<TArray<FString>>>(HorseDomains()),
      [](const FProjectHorseEntityPayload &PayloadValue)
          -> const FHorseRouteSeed & {
        return PayloadValue.Horse;
      },
      RegisteredComponentGroups<FHorseRouteSeed>(
          {{"Components/Data", {"Id", "Name"}},
           {"Components/Bots", {"MountedRider"}},
           {"Components/Spatial", {"PatrolRoute"}}}));
}

ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectBotEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Bot.Id);
      },
      func::constant<TArray<TArray<FString>>>(BotDomains()),
      [](const FProjectBotEntityPayload &PayloadValue) -> const FBotEntity & {
        return PayloadValue.Bot;
      },
      RegisteredComponentGroups<FBotEntity>(
          {{"Components/Data", {"Id", "DisplayName", "Kind"}},
           {"Components/Bots", {"Alignment", "Active"}}}));
}

ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      [](const FProjectPlayerEntityPayload &) { return PlayerEntityKey(); },
      func::constant<TArray<TArray<FString>>>(PlayerDomains()),
      [](const FProjectPlayerEntityPayload &PayloadValue)
          -> const FPlayerState & {
        return PayloadValue.Player;
      },
      RegisteredComponentGroups<FPlayerState>(
          {{"Components/Lifecycle", {"Ready"}},
           {"Components/Data", {"HasLastActionId", "LastActionId"}}}));
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
