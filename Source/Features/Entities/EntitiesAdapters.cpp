#include "Features/Entities/EntitiesAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace EntitiesAdapters {
namespace {

/**
 * @brief Converts landmark enum state into stable ECS text.
 * @signature FString LandmarkKindText(ELandmarkKind Kind)
 *
 * User Story: As entity projection code, landmark enum cases should be
 * explicit and fail on impossible values instead of falling back silently.
 */
FString LandmarkKindText(ELandmarkKind Kind) {
  switch (Kind) {
  case ELandmarkKind::Building:
    return TEXT("Building");
  case ELandmarkKind::Road:
    return TEXT("Road");
  case ELandmarkKind::Creek:
    return TEXT("Creek");
  case ELandmarkKind::TerrainMarker:
    return TEXT("TerrainMarker");
  case ELandmarkKind::Mine:
    return TEXT("Mine");
  case ELandmarkKind::Cemetery:
    return TEXT("Cemetery");
  case ELandmarkKind::Park:
    return TEXT("Park");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Converts nature enum state into stable ECS text.
 * @signature FString NatureKindText(ENatureFeatureKind Kind)
 *
 * User Story: As entity projection code, nature enum cases should be explicit
 * and fail on impossible values instead of falling back silently.
 */
FString NatureKindText(ENatureFeatureKind Kind) {
  switch (Kind) {
  case ENatureFeatureKind::Water:
    return TEXT("Water");
  case ENatureFeatureKind::Rock:
    return TEXT("Rock");
  case ENatureFeatureKind::TreeGrove:
    return TEXT("TreeGrove");
  case ENatureFeatureKind::Shrub:
    return TEXT("Shrub");
  case ENatureFeatureKind::PCGMarker:
    return TEXT("PCGMarker");
  case ENatureFeatureKind::WaterSystemMarker:
    return TEXT("WaterSystemMarker");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Converts townsperson interaction intent into stable ECS text.
 * @signature FString InteractionIntentText(ETownspersonInteractionIntent Intent)
 *
 * User Story: As entity projection code, townsperson intent enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString InteractionIntentText(ETownspersonInteractionIntent Intent) {
  switch (Intent) {
  case ETownspersonInteractionIntent::General:
    return TEXT("General");
  case ETownspersonInteractionIntent::Dialogue:
    return TEXT("Dialogue");
  case ETownspersonInteractionIntent::Memory:
    return TEXT("Memory");
  case ETownspersonInteractionIntent::CombatValidation:
    return TEXT("CombatValidation");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Converts bot kind enum state into stable ECS text.
 * @signature FString BotKindText(EBotEntityKind Kind)
 *
 * User Story: As bot entity projection code, bot kind enum cases should be
 * explicit and fail on impossible values instead of falling back silently.
 */
FString BotKindText(EBotEntityKind Kind) {
  switch (Kind) {
  case EBotEntityKind::Townsperson:
    return TEXT("Townsperson");
  case EBotEntityKind::Horse:
    return TEXT("Horse");
  }
  checkNoEntry();
  return FString();
}

/**
 * @brief Converts bot alignment enum state into stable ECS text.
 * @signature FString BotAlignmentText(EBotAlignment Alignment)
 *
 * User Story: As bot entity projection code, bot alignment enum cases should
 * be explicit and fail on impossible values instead of falling back silently.
 */
FString BotAlignmentText(EBotAlignment Alignment) {
  switch (Alignment) {
  case EBotAlignment::Friendly:
    return TEXT("Friendly");
  case EBotAlignment::Neutral:
    return TEXT("Neutral");
  }
  checkNoEntry();
  return FString();
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

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
LandmarkComponents(const FLandmark &Landmark) {
  return {{TEXT("Components/Data/Id"), ecs::textValue(Landmark.Id)},
          {TEXT("Components/Data/Label"), ecs::textValue(Landmark.Label)},
          {TEXT("Components/Data/Kind"),
           ecs::textValue(LandmarkKindText(Landmark.Kind))},
          {TEXT("Components/Spatial/Location"),
           ecs::vec3Value(Landmark.Location)},
          {TEXT("Components/Spatial/Scale"), ecs::vec3Value(Landmark.Scale)}};
}

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
NatureComponents(const FNatureFeatureSeed &Feature) {
  return {{TEXT("Components/Data/Id"), ecs::textValue(Feature.Id)},
          {TEXT("Components/Data/Name"), ecs::textValue(Feature.Name)},
          {TEXT("Components/Data/Kind"),
           ecs::textValue(NatureKindText(Feature.Kind))},
          {TEXT("Components/Spatial/LocalLocation"),
           ComponentsAdapters::LocalPointValue(Feature.Location)},
          {TEXT("Components/Spatial/Scale"), ecs::vec3Value(Feature.Scale)}};
}

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
TownspersonComponents(const FTownspersonSeed &Townsperson) {
  return {{TEXT("Components/Data/Id"), ecs::textValue(Townsperson.Id)},
          {TEXT("Components/Data/Name"), ecs::textValue(Townsperson.Name)},
          {TEXT("Components/Bots/Role"), ecs::textValue(Townsperson.Role)},
          {TEXT("Components/Bots/Persona"),
           ecs::textValue(Townsperson.Persona)},
          {TEXT("Components/Bots/InteractionPrompt"),
           ecs::textValue(Townsperson.InteractionPrompt)},
          {TEXT("Components/Bots/DefaultPlayerLine"),
           ecs::textValue(Townsperson.DefaultPlayerLine)},
          {TEXT("Components/Bots/PinnedResponse"),
           ecs::textValue(Townsperson.PinnedResponse)},
          {TEXT("Components/Bots/InteractionIntent"),
           ecs::textValue(
               InteractionIntentText(Townsperson.InteractionIntent))},
          {TEXT("Components/Spatial/PatrolRoute"),
           ecs::listValue(
               ComponentsAdapters::LocalPointList(Townsperson.PatrolRoute))}};
}

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
HorseComponents(const FHorseRouteSeed &Horse) {
  return {{TEXT("Components/Data/Id"), ecs::textValue(Horse.Id)},
          {TEXT("Components/Data/Name"), ecs::textValue(Horse.Name)},
          {TEXT("Components/Bots/MountedRider"),
           ecs::boolValue(Horse.bMountedRider)},
          {TEXT("Components/Spatial/PatrolRoute"),
           ecs::listValue(ComponentsAdapters::LocalPointList(Horse.PatrolRoute))}};
}

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
BotComponents(const FBotEntity &Bot) {
  return {{TEXT("Components/Data/Id"), ecs::textValue(Bot.Id)},
          {TEXT("Components/Data/DisplayName"),
           ecs::textValue(Bot.DisplayName)},
          {TEXT("Components/Data/Kind"), ecs::textValue(BotKindText(Bot.Kind))},
          {TEXT("Components/Bots/Alignment"),
           ecs::textValue(BotAlignmentText(Bot.Alignment))},
          {TEXT("Components/Bots/Active"), ecs::boolValue(Bot.bActive)}};
}

TArray<ComponentsAdapters::FEcsComponentProjectionBinding>
PlayerComponents(const FPlayerState &Player) {
  return {{TEXT("Components/Lifecycle/Ready"),
           ecs::boolValue(Player.bReady)},
          {TEXT("Components/Data/HasLastActionId"),
           ecs::boolValue(Player.LastActionId.hasValue)},
          {TEXT("Components/Data/LastActionId"),
           ecs::textValue(Player.LastActionId.hasValue
                              ? Player.LastActionId.value
                              : FString())}};
}

} // namespace

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
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectLandmarkEntityPayload &PayloadValue) {
        return LandmarkEntityKey(PayloadValue.Landmark.Id);
      },
      [](const FProjectLandmarkEntityPayload &) { return LandmarkDomains(); },
      [](const FProjectLandmarkEntityPayload &PayloadValue) {
        return LandmarkComponents(PayloadValue.Landmark);
      });
}

ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
        return NatureEntityKey(PayloadValue.Feature.Id);
      },
      [](const FProjectNatureFeatureEntityPayload &) { return NatureDomains(); },
      [](const FProjectNatureFeatureEntityPayload &PayloadValue) {
        return NatureComponents(PayloadValue.Feature);
      });
}

ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectTownspersonEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Townsperson.Id);
      },
      [](const FProjectTownspersonEntityPayload &) {
        return TownspersonDomains();
      },
      [](const FProjectTownspersonEntityPayload &PayloadValue) {
        return TownspersonComponents(PayloadValue.Townsperson);
      });
}

ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectHorseEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Horse.Id);
      },
      [](const FProjectHorseEntityPayload &) { return HorseDomains(); },
      [](const FProjectHorseEntityPayload &PayloadValue) {
        return HorseComponents(PayloadValue.Horse);
      });
}

ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectBotEntityPayload &PayloadValue) {
        return BotEntityKey(PayloadValue.Bot.Id);
      },
      [](const FProjectBotEntityPayload &) { return BotDomains(); },
      [](const FProjectBotEntityPayload &PayloadValue) {
        return BotComponents(PayloadValue.Bot);
      });
}

ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityWith(
      Payload,
      [](const FProjectPlayerEntityPayload &) { return PlayerEntityKey(); },
      [](const FProjectPlayerEntityPayload &) { return PlayerDomains(); },
      [](const FProjectPlayerEntityPayload &PayloadValue) {
        return PlayerComponents(PayloadValue.Player);
      });
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
