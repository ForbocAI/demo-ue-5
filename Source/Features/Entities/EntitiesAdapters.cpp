#include "Features/Entities/EntitiesAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace EntitiesAdapters {
namespace {

struct FApplyEntityProjectionPayload {
  ecs::FWorld World;
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
};

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

/**
 * @brief Applies entity domain and component batches to a world.
 * @signature ecs::FWorld ApplyEntityProjection(const FApplyEntityProjectionPayload &Payload)
 *
 * User Story: As entity adapters, repeated domain/component projection should
 * use neutral component primitives rather than per-entity scalar helper calls.
 */
ecs::FWorld
ApplyEntityProjection(const FApplyEntityProjectionPayload &Payload) {
  return (func::pipe(Payload.World) |
          [&Payload](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomainSteps(
                {Next, Payload.Domains, 0});
          } |
          [&Payload](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponentSteps(
                {Next, Payload.Components, 0});
          })
      .val;
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
  const ecs::EntityKey Entity = LandmarkEntityKey(Payload.Landmark.Id);
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add(
      {Entity, {TEXT("Entities"), TEXT("Environments"), TEXT("Landmarks")}});
  Domains.Add({Entity, {TEXT("Systems"), TEXT("Landmarks")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Data/Id"),
                  ecs::textValue(Payload.Landmark.Id)});
  Components.Add({Entity, TEXT("Components/Data/Label"),
                  ecs::textValue(Payload.Landmark.Label)});
  Components.Add({Entity, TEXT("Components/Data/Kind"),
                  ecs::textValue(LandmarkKindText(Payload.Landmark.Kind))});
  Components.Add({Entity, TEXT("Components/Spatial/Location"),
                  ecs::vec3Value(Payload.Landmark.Location)});
  Components.Add({Entity, TEXT("Components/Spatial/Scale"),
                  ecs::vec3Value(Payload.Landmark.Scale)});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

ecs::FWorld
ProjectNatureFeature(const FProjectNatureFeatureEntityPayload &Payload) {
  const ecs::EntityKey Entity = NatureEntityKey(Payload.Feature.Id);
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add(
      {Entity, {TEXT("Entities"), TEXT("Environments"), TEXT("Nature")}});
  Domains.Add({Entity, {TEXT("Systems"), TEXT("Nature")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Data/Id"),
                  ecs::textValue(Payload.Feature.Id)});
  Components.Add({Entity, TEXT("Components/Data/Name"),
                  ecs::textValue(Payload.Feature.Name)});
  Components.Add({Entity, TEXT("Components/Data/Kind"),
                  ecs::textValue(NatureKindText(Payload.Feature.Kind))});
  Components.Add({Entity, TEXT("Components/Spatial/LocalLocation"),
                  ComponentsAdapters::LocalPointValue(
                      Payload.Feature.Location)});
  Components.Add({Entity, TEXT("Components/Spatial/Scale"),
                  ecs::vec3Value(Payload.Feature.Scale)});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) {
  const ecs::EntityKey Entity = BotEntityKey(Payload.Townsperson.Id);
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add({Entity,
               {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                TEXT("Townspeople")}});
  Domains.Add({Entity,
               {TEXT("Systems"), TEXT("Bots"), TEXT("Townspeople")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Data/Id"),
                  ecs::textValue(Payload.Townsperson.Id)});
  Components.Add({Entity, TEXT("Components/Data/Name"),
                  ecs::textValue(Payload.Townsperson.Name)});
  Components.Add({Entity, TEXT("Components/Bots/Role"),
                  ecs::textValue(Payload.Townsperson.Role)});
  Components.Add({Entity, TEXT("Components/Bots/Persona"),
                  ecs::textValue(Payload.Townsperson.Persona)});
  Components.Add({Entity, TEXT("Components/Bots/InteractionPrompt"),
                  ecs::textValue(Payload.Townsperson.InteractionPrompt)});
  Components.Add({Entity, TEXT("Components/Bots/DefaultPlayerLine"),
                  ecs::textValue(Payload.Townsperson.DefaultPlayerLine)});
  Components.Add({Entity, TEXT("Components/Bots/PinnedResponse"),
                  ecs::textValue(Payload.Townsperson.PinnedResponse)});
  Components.Add(
      {Entity, TEXT("Components/Bots/InteractionIntent"),
       ecs::textValue(
           InteractionIntentText(Payload.Townsperson.InteractionIntent))});
  Components.Add(
      {Entity, TEXT("Components/Spatial/PatrolRoute"),
       ecs::listValue(
           ComponentsAdapters::LocalPointList(Payload.Townsperson.PatrolRoute))});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

ecs::FWorld ProjectHorse(const FProjectHorseEntityPayload &Payload) {
  const ecs::EntityKey Entity = BotEntityKey(Payload.Horse.Id);
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add({Entity,
               {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                TEXT("Horses")}});
  Domains.Add({Entity, {TEXT("Systems"), TEXT("Bots"), TEXT("Horses")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Data/Id"),
                  ecs::textValue(Payload.Horse.Id)});
  Components.Add({Entity, TEXT("Components/Data/Name"),
                  ecs::textValue(Payload.Horse.Name)});
  Components.Add({Entity, TEXT("Components/Bots/MountedRider"),
                  ecs::boolValue(Payload.Horse.bMountedRider)});
  Components.Add(
      {Entity, TEXT("Components/Spatial/PatrolRoute"),
       ecs::listValue(
           ComponentsAdapters::LocalPointList(Payload.Horse.PatrolRoute))});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

ecs::FWorld ProjectBot(const FProjectBotEntityPayload &Payload) {
  const ecs::EntityKey Entity = BotEntityKey(Payload.Bot.Id);
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add(
      {Entity, {TEXT("Entities"), TEXT("Characters"), TEXT("Bots")}});
  Domains.Add({Entity, {TEXT("Systems"), TEXT("Bots")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Data/Id"),
                  ecs::textValue(Payload.Bot.Id)});
  Components.Add({Entity, TEXT("Components/Data/DisplayName"),
                  ecs::textValue(Payload.Bot.DisplayName)});
  Components.Add({Entity, TEXT("Components/Data/Kind"),
                  ecs::textValue(BotKindText(Payload.Bot.Kind))});
  Components.Add({Entity, TEXT("Components/Bots/Alignment"),
                  ecs::textValue(BotAlignmentText(Payload.Bot.Alignment))});
  Components.Add({Entity, TEXT("Components/Bots/Active"),
                  ecs::boolValue(Payload.Bot.bActive)});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

ecs::FWorld ProjectPlayer(const FProjectPlayerEntityPayload &Payload) {
  const ecs::EntityKey Entity = PlayerEntityKey();
  TArray<ComponentsAdapters::FEcsDomainProjectionStep> Domains;
  Domains.Add({Entity,
               {TEXT("Entities"), TEXT("Characters"), TEXT("Player")}});

  TArray<ComponentsAdapters::FEcsComponentProjectionStep> Components;
  Components.Add({Entity, TEXT("Components/Lifecycle/Ready"),
                  ecs::boolValue(Payload.Player.bReady)});
  Components.Add({Entity, TEXT("Components/Data/HasLastActionId"),
                  ecs::boolValue(Payload.Player.LastActionId.hasValue)});
  Components.Add({Entity, TEXT("Components/Data/LastActionId"),
                  ecs::textValue(Payload.Player.LastActionId.hasValue
                                     ? Payload.Player.LastActionId.value
                                     : FString())});
  return ApplyEntityProjection({Payload.World, Domains, Components});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
