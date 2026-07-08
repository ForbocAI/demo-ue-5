#include "Features/Entities/Characters/Bots/Adapters.h"

#include "Features/Components/Data/Json/Settings/Adapters.h"
#include "Features/Components/Spatial/Level/Layout/Adapters.h"
#include "Features/Components/Adapters.h"
#include "Features/Entities/Adapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FTownspersonRoutePointFields {
  float EastLots;
  float NorthLots;
};

struct FInteractionFields {
  FString Intent;
  FString Prompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
};

struct FTownspersonFields {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  TSharedPtr<FJsonObject> Interaction;
  TArray<TSharedPtr<FJsonValue>> PatrolRoute;
};

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Data {
namespace JsonAdapters {

namespace LevelTypes = ForbocAI::Game::Level;
namespace BotSeedTypes = ForbocAI::Game::Level::BotsAdapters;

using ETownspersonInteractionIntent =
    LevelTypes::ETownspersonInteractionIntent;
using FInteractionFields = BotSeedTypes::FInteractionFields;
using FTownspersonRoutePointFields = BotSeedTypes::FTownspersonRoutePointFields;
using FTownspersonFields = BotSeedTypes::FTownspersonFields;

template <>
struct TJsonTextValueRegistry<ETownspersonInteractionIntent> {
  static const TArray<TTextValueDeclaration<ETownspersonInteractionIntent>> &
  Values() {
    static const TArray<TTextValueDeclaration<ETownspersonInteractionIntent>>
        RegisteredValues = {
            {"general", ETownspersonInteractionIntent::General},
            {"dialogue", ETownspersonInteractionIntent::Dialogue},
            {"memory", ETownspersonInteractionIntent::Memory},
            {"combat_validation",
             ETownspersonInteractionIntent::CombatValidation}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FTownspersonRoutePointFields, EastLots, NorthLots);
JSON_SETTINGS_REGISTRY(FInteractionFields, Intent, Prompt, DefaultPlayerLine,
                       PinnedResponse);
JSON_SETTINGS_REGISTRY(FTownspersonFields, Id, Name, Role, Persona,
                       Interaction, PatrolRoute);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {
namespace {

struct FTownspersonRouteLotsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FTownspersonRouteLotsProjector)(
    const FTownspersonRouteLotsRequest &);

struct FTownspersonPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FTownspersonRouteLotsProjector ProjectLots;
};

struct FTownspersonBuildRequest {
  FTownspersonFields Fields;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

FLevelLocalPoint
TownspersonRouteLots(const FTownspersonRouteLotsRequest &Request) {
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutAdapters::CharacterHeightOffset(Request.Geometry)});
}

ETownspersonInteractionIntent InteractionIntentFromJson(
    const FString &Intent) {
  return JsonAdapters::RequireRegisteredTextValue<
      ETownspersonInteractionIntent>(
      Intent, TEXT("townsperson interaction intent"));
}

TArray<FLevelLocalPoint>
TownspersonPatrolRouteFromFields(
    const FTownspersonPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
      Request.Geometry;
  const FTownspersonRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FTownspersonRoutePointFields, FLevelLocalPoint>(
      JsonAdapters::MapSettingsJsonValues<FTownspersonRoutePointFields>(
          Request.Points, JSON_SETTINGS_ATOMS(EastLots, NorthLots)),
      [Geometry, ProjectLots](const FTownspersonRoutePointFields &Fields) {
        return ProjectLots({Geometry, Fields.EastLots, Fields.NorthLots});
      });
}

FTownspersonSeed TownspersonFromFields(
    const FTownspersonBuildRequest &Request) {
  const FInteractionFields Interaction =
      JsonAdapters::ReadSettingsFields<FInteractionFields>(
          Request.Fields.Interaction,
          JSON_SETTINGS_ATOMS(Intent, Prompt, DefaultPlayerLine,
                              PinnedResponse));
  FTownspersonSeed Seed;
  Seed.Id = Request.Fields.Id;
  Seed.Name = Request.Fields.Name;
  Seed.Role = Request.Fields.Role;
  Seed.Persona = Request.Fields.Persona;
  Seed.InteractionPrompt = Interaction.Prompt;
  Seed.DefaultPlayerLine = Interaction.DefaultPlayerLine;
  Seed.PinnedResponse = Interaction.PinnedResponse;
  Seed.InteractionIntent = InteractionIntentFromJson(Interaction.Intent);
  Seed.PatrolRoute = TownspersonPatrolRouteFromFields(
      {Request.Fields.PatrolRoute, Request.Geometry, TownspersonRouteLots});
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FTownspersonFields, FTownspersonSeed>(
      JsonAdapters::ReadSettingsObjectArrayField<FTownspersonFields>(
          "Townspeople",
          JSON_SETTINGS_ATOMS(Id, Name, Role, Persona, Interaction,
                              PatrolRoute))(Root),
      [&Request](const FTownspersonFields &Fields) {
        return TownspersonFromFields({Fields, Request.Geometry});
      });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace ComponentsAdapters {

template <> struct TComponentTextRegistry<ETownspersonInteractionIntent> {
  static const TArray<
      TComponentTextDeclaration<ETownspersonInteractionIntent>>
      &Declarations() {
    static const TArray<TComponentTextDeclaration<
        ETownspersonInteractionIntent>>
        RegisteredCases = {
            {ETownspersonInteractionIntent::General, "General"},
            {ETownspersonInteractionIntent::Dialogue, "Dialogue"},
            {ETownspersonInteractionIntent::Memory, "Memory"},
            {ETownspersonInteractionIntent::CombatValidation,
             "CombatValidation"}};
    return RegisteredCases;
  }
};

template <> struct TComponentSourceValueFieldRegistry<FTownspersonSeed> {
  static const TArray<
      TComponentSourceValueFieldDeclaration<FTownspersonSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FTownspersonSeed>>
        RegisteredFields = {
            {"Id", &FTownspersonSeed::Id},
            {"Name", &FTownspersonSeed::Name},
            {"Role", &FTownspersonSeed::Role},
            {"Persona", &FTownspersonSeed::Persona},
            {"InteractionPrompt", &FTownspersonSeed::InteractionPrompt},
            {"DefaultPlayerLine", &FTownspersonSeed::DefaultPlayerLine},
            {"PinnedResponse", &FTownspersonSeed::PinnedResponse},
            {"InteractionIntent", &FTownspersonSeed::InteractionIntent},
            {"PatrolRoute", &FTownspersonSeed::PatrolRoute}};
    return RegisteredFields;
  }
};

template <>
struct TComponentSourceProjector<FTownspersonSeed> {
  ecs::FComponentValue
  operator()(const FTownspersonSeed &Townsperson) const {
    return ComponentSourceValueMap(
        Townsperson,
        {"Id", "Name", "Role", "Persona", "InteractionPrompt",
         "DefaultPlayerLine", "PinnedResponse", "InteractionIntent",
         "PatrolRoute"});
  }
};

} // namespace ComponentsAdapters

namespace EntitiesAdapters {

using ComponentsAdapters::RegisteredComponentGroups;

extern ecs::EntityKey BotEntityKey(const FString &Id);

ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) {
  return ComponentsAdapters::ProjectPayloadEntityCatalogWith(
      Payload,
      ComponentsAdapters::TEntityCatalogProjection{
          [](const FProjectTownspersonEntityPayload &PayloadValue) {
            return BotEntityKey(PayloadValue.Townsperson.Id);
          },
          func::constant<TArray<TArray<FString>>>(
              ComponentsAdapters::ComponentDomains(
                  {{"Entities", "Characters", "Bots", "Townspeople"},
                   {"Systems", "Bots", "Townspeople"}})),
          [](const FProjectTownspersonEntityPayload &PayloadValue)
              -> const FTownspersonSeed & {
            return PayloadValue.Townsperson;
          },
          RegisteredComponentGroups<FTownspersonSeed>(
              {{"Components/Data", {"Id", "Name"}},
               {"Components/Bots",
                {"Role", "Persona", "InteractionPrompt", "DefaultPlayerLine",
                 "PinnedResponse", "InteractionIntent"}},
               {"Components/Spatial", {"PatrolRoute"}}})});
}

} // namespace EntitiesAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
