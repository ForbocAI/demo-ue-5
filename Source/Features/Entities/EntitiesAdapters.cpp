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
  return ComponentsAdapters::ComponentText(
      Kind, {{ELandmarkKind::Building, "Building"},
             {ELandmarkKind::Road, "Road"},
             {ELandmarkKind::Creek, "Creek"},
             {ELandmarkKind::TerrainMarker, "TerrainMarker"},
             {ELandmarkKind::Mine, "Mine"},
             {ELandmarkKind::Cemetery, "Cemetery"},
             {ELandmarkKind::Park, "Park"}});
}

/**
 * @brief Converts nature enum state into stable ECS text.
 * @signature FString NatureKindText(ENatureFeatureKind Kind)
 *
 * User Story: As entity projection code, nature enum cases should be explicit
 * and fail on impossible values instead of falling back silently.
 */
FString NatureKindText(ENatureFeatureKind Kind) {
  return ComponentsAdapters::ComponentText(
      Kind, {{ENatureFeatureKind::Water, "Water"},
             {ENatureFeatureKind::Rock, "Rock"},
             {ENatureFeatureKind::TreeGrove, "TreeGrove"},
             {ENatureFeatureKind::Shrub, "Shrub"},
             {ENatureFeatureKind::PCGMarker, "PCGMarker"},
             {ENatureFeatureKind::WaterSystemMarker, "WaterSystemMarker"}});
}

/**
 * @brief Converts townsperson interaction intent into stable ECS text.
 * @signature FString InteractionIntentText(ETownspersonInteractionIntent Intent)
 *
 * User Story: As entity projection code, townsperson intent enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString InteractionIntentText(ETownspersonInteractionIntent Intent) {
  return ComponentsAdapters::ComponentText(
      Intent, {{ETownspersonInteractionIntent::General, "General"},
               {ETownspersonInteractionIntent::Dialogue, "Dialogue"},
               {ETownspersonInteractionIntent::Memory, "Memory"},
               {ETownspersonInteractionIntent::CombatValidation,
                "CombatValidation"}});
}

/**
 * @brief Converts bot kind enum state into stable ECS text.
 * @signature FString BotKindText(EBotEntityKind Kind)
 *
 * User Story: As bot entity projection code, bot kind enum cases should be
 * explicit and fail on impossible values instead of falling back silently.
 */
FString BotKindText(EBotEntityKind Kind) {
  return ComponentsAdapters::ComponentText(
      Kind, {{EBotEntityKind::Townsperson, "Townsperson"},
             {EBotEntityKind::Horse, "Horse"}});
}

/**
 * @brief Converts bot alignment enum state into stable ECS text.
 * @signature FString BotAlignmentText(EBotAlignment Alignment)
 *
 * User Story: As bot entity projection code, bot alignment enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString BotAlignmentText(EBotAlignment Alignment) {
  return ComponentsAdapters::ComponentText(
      Alignment, {{EBotAlignment::Friendly, "Friendly"},
                  {EBotAlignment::Neutral, "Neutral"}});
}

TArray<TArray<FString>> LandmarkDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Environments", "Landmarks"},
       {"Systems", "Landmarks"}});
}

TArray<TArray<FString>> NatureDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Environments", "Nature"}, {"Systems", "Nature"}});
}

TArray<TArray<FString>> TownspersonDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Characters", "Bots", "Townspeople"},
       {"Systems", "Bots", "Townspeople"}});
}

TArray<TArray<FString>> HorseDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Characters", "Bots", "Horses"},
       {"Systems", "Bots", "Horses"}});
}

TArray<TArray<FString>> BotDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Characters", "Bots"}, {"Systems", "Bots"}});
}

TArray<TArray<FString>> PlayerDomains() {
  return ComponentsAdapters::ComponentDomains(
      {{"Entities", "Characters", "Player"}});
}

bool HasLastActionId(func::Maybe<FString> LastActionId) {
  return LastActionId.hasValue;
}

FString LastActionIdText(func::Maybe<FString> LastActionId) {
  return LastActionId.hasValue ? LastActionId.value : FString();
}

} // namespace

namespace ComponentsAdapters {

template <>
struct TComponentSourceProjector<FLandmark> {
  ecs::FComponentValue operator()(const FLandmark &Landmark) const {
    return ComponentSourceValueMap(
        Landmark, {{"Id", &FLandmark::Id},
                   {"Label", &FLandmark::Label},
                   {"Kind", &FLandmark::Kind, LandmarkKindText},
                   {"Location", &FLandmark::Location},
                   {"Scale", &FLandmark::Scale}});
  }
};

template <>
struct TComponentSourceProjector<FNatureFeatureSeed> {
  ecs::FComponentValue
  operator()(const FNatureFeatureSeed &NatureFeature) const {
    return ComponentSourceValueMap(
        NatureFeature,
        {{"Id", &FNatureFeatureSeed::Id},
         {"Name", &FNatureFeatureSeed::Name},
         {"Kind", &FNatureFeatureSeed::Kind, NatureKindText},
         {"LocalLocation", &FNatureFeatureSeed::Location},
         {"Scale", &FNatureFeatureSeed::Scale}});
  }
};

template <>
struct TComponentSourceProjector<FTownspersonSeed> {
  ecs::FComponentValue
  operator()(const FTownspersonSeed &Townsperson) const {
    return ComponentSourceValueMap(
        Townsperson,
        {{"Id", &FTownspersonSeed::Id},
         {"Name", &FTownspersonSeed::Name},
         {"Role", &FTownspersonSeed::Role},
         {"Persona", &FTownspersonSeed::Persona},
         {"InteractionPrompt", &FTownspersonSeed::InteractionPrompt},
         {"DefaultPlayerLine", &FTownspersonSeed::DefaultPlayerLine},
         {"PinnedResponse", &FTownspersonSeed::PinnedResponse},
         {"InteractionIntent", &FTownspersonSeed::InteractionIntent,
          InteractionIntentText},
         {"PatrolRoute", &FTownspersonSeed::PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FHorseRouteSeed> {
  ecs::FComponentValue operator()(const FHorseRouteSeed &Horse) const {
    return ComponentSourceValueMap(
        Horse, {{"Id", &FHorseRouteSeed::Id},
                {"Name", &FHorseRouteSeed::Name},
                {"MountedRider", &FHorseRouteSeed::bMountedRider},
                {"PatrolRoute", &FHorseRouteSeed::PatrolRoute}});
  }
};

template <>
struct TComponentSourceProjector<FBotEntity> {
  ecs::FComponentValue operator()(const FBotEntity &Bot) const {
    return ComponentSourceValueMap(
        Bot, {{"Id", &FBotEntity::Id},
              {"DisplayName", &FBotEntity::DisplayName},
              {"Kind", &FBotEntity::Kind, BotKindText},
              {"Alignment", &FBotEntity::Alignment, BotAlignmentText},
              {"Active", &FBotEntity::bActive}});
  }
};

template <>
struct TComponentSourceProjector<FPlayerState> {
  ecs::FComponentValue operator()(const FPlayerState &Player) const {
    return ComponentSourceValueMap(
        Player,
        {{"Ready", &FPlayerState::bReady},
         {"HasLastActionId", &FPlayerState::LastActionId, HasLastActionId},
         {"LastActionId", &FPlayerState::LastActionId, LastActionIdText}});
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
