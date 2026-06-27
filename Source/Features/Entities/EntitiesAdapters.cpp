#include "Features/Entities/EntitiesAdapters.h"

#include "Features/Components/ComponentsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace EntitiesAdapters {
namespace {

FString LandmarkKindText(ELandmarkKind Kind) {
  return Kind == ELandmarkKind::Building
             ? TEXT("Building")
             : Kind == ELandmarkKind::Road
                   ? TEXT("Road")
                   : Kind == ELandmarkKind::Creek
                         ? TEXT("Creek")
                         : Kind == ELandmarkKind::TerrainMarker
                               ? TEXT("TerrainMarker")
                               : Kind == ELandmarkKind::Mine
                                     ? TEXT("Mine")
                                     : Kind == ELandmarkKind::Cemetery
                                           ? TEXT("Cemetery")
                                           : TEXT("Park");
}

FString NatureKindText(ENatureFeatureKind Kind) {
  return Kind == ENatureFeatureKind::Water
             ? TEXT("Water")
             : Kind == ENatureFeatureKind::Rock
                   ? TEXT("Rock")
                   : Kind == ENatureFeatureKind::TreeGrove
                         ? TEXT("TreeGrove")
                         : Kind == ENatureFeatureKind::Shrub
                               ? TEXT("Shrub")
                               : Kind == ENatureFeatureKind::PCGMarker
                                     ? TEXT("PCGMarker")
                                     : TEXT("WaterSystemMarker");
}

FString InteractionIntentText(ETownspersonInteractionIntent Intent) {
  return Intent == ETownspersonInteractionIntent::Dialogue
             ? TEXT("Dialogue")
             : Intent == ETownspersonInteractionIntent::Memory
                   ? TEXT("Memory")
                   : Intent == ETownspersonInteractionIntent::CombatValidation
                         ? TEXT("CombatValidation")
                         : TEXT("General");
}

FString BotKindText(EBotEntityKind Kind) {
  return Kind == EBotEntityKind::Horse ? TEXT("Horse") : TEXT("Townsperson");
}

FString BotAlignmentText(EBotAlignment Alignment) {
  return Alignment == EBotAlignment::Friendly ? TEXT("Friendly")
                                              : TEXT("Neutral");
}

} // namespace

ecs::EntityKey BotEntityKey(const FString &Id) {
  return FString::Printf(TEXT("bot:%s"), *Id);
}

ecs::EntityKey PlayerEntityKey() {
  return TEXT("player:local");
}

ecs::EntityKey LandmarkEntityKey(const FString &Id) {
  return FString::Printf(TEXT("landmark:%s"), *Id);
}

ecs::EntityKey NatureEntityKey(const FString &Id) {
  return FString::Printf(TEXT("nature:%s"), *Id);
}

ecs::FWorld ProjectLandmark(ecs::FWorld World, const FLandmark &Landmark) {
  const ecs::EntityKey Entity = LandmarkEntityKey(Landmark.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Environments"), TEXT("Landmarks")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Landmarks")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Id"), Landmark.Id);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Label"), Landmark.Label);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Kind"),
                LandmarkKindText(Landmark.Kind));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/Location"),
                Landmark.Location);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/Scale"), Landmark.Scale);
          })
      .val;
}

ecs::FWorld ProjectNatureFeature(ecs::FWorld World,
                                 const FNatureFeatureSeed &Feature) {
  const ecs::EntityKey Entity = NatureEntityKey(Feature.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Environments"), TEXT("Nature")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Nature")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Id"), Feature.Id);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Name"), Feature.Name);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Kind"),
                NatureKindText(Feature.Kind));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithComponent(
                Next, Entity, TEXT("Components/Spatial/LocalLocation"),
                ComponentsAdapters::LocalPointValue(Feature.Location));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithVec3(
                Next, Entity, TEXT("Components/Spatial/Scale"), Feature.Scale);
          })
      .val;
}

ecs::FWorld ProjectTownsperson(ecs::FWorld World,
                               const FTownspersonSeed &Townsperson) {
  const ecs::EntityKey Entity = BotEntityKey(Townsperson.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                 TEXT("Townspeople")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Systems"), TEXT("Bots"), TEXT("Townspeople")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Id"), Townsperson.Id);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Name"), Townsperson.Name);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/Role"), Townsperson.Role);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/Persona"),
                Townsperson.Persona);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/InteractionPrompt"),
                Townsperson.InteractionPrompt);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/DefaultPlayerLine"),
                Townsperson.DefaultPlayerLine);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/PinnedResponse"),
                Townsperson.PinnedResponse);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/InteractionIntent"),
                InteractionIntentText(Townsperson.InteractionIntent));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Spatial/PatrolRoute"),
                ComponentsAdapters::LocalPointList(Townsperson.PatrolRoute));
          })
      .val;
}

ecs::FWorld ProjectHorse(ecs::FWorld World, const FHorseRouteSeed &Horse) {
  const ecs::EntityKey Entity = BotEntityKey(Horse.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Characters"), TEXT("Bots"),
                 TEXT("Horses")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Systems"), TEXT("Bots"), TEXT("Horses")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Id"), Horse.Id);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Name"), Horse.Name);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/MountedRider"),
                Horse.bMountedRider);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithList(
                Next, Entity, TEXT("Components/Spatial/PatrolRoute"),
                ComponentsAdapters::LocalPointList(Horse.PatrolRoute));
          })
      .val;
}

ecs::FWorld ProjectBot(ecs::FWorld World, const FBotEntity &Bot) {
  const ecs::EntityKey Entity = BotEntityKey(Bot.Id);
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Characters"), TEXT("Bots")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity, {TEXT("Systems"), TEXT("Bots")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Id"), Bot.Id);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/DisplayName"),
                Bot.DisplayName);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/Kind"),
                BotKindText(Bot.Kind));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Bots/Alignment"),
                BotAlignmentText(Bot.Alignment));
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Bots/Active"), Bot.bActive);
          })
      .val;
}

ecs::FWorld ProjectPlayer(ecs::FWorld World, const FPlayerState &Player) {
  const ecs::EntityKey Entity = PlayerEntityKey();
  return (func::pipe(World) |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithDomain(
                Next, Entity,
                {TEXT("Entities"), TEXT("Characters"), TEXT("Player")});
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Lifecycle/Ready"),
                Player.bReady);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithBool(
                Next, Entity, TEXT("Components/Data/HasLastActionId"),
                Player.LastActionId.hasValue);
          } |
          [&](ecs::FWorld Next) {
            return ComponentsAdapters::WithText(
                Next, Entity, TEXT("Components/Data/LastActionId"),
                Player.LastActionId.hasValue ? Player.LastActionId.value
                                             : FString());
          })
      .val;
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
