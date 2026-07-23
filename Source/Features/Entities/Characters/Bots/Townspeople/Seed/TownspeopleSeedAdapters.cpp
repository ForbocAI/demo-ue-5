#include "Features/Entities/Characters/Bots/CharactersBotsAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FTownspersonRoutePointSource {
  float EastLots;
  float NorthLots;
};

struct FInteractionSource {
  FString Intent;
  FString Prompt;
  FString DefaultPlayerLine;
  FString PinnedResponse;
};

struct FTownspersonSource {
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
using FInteractionSource = BotSeedTypes::FInteractionSource;
using FTownspersonRoutePointSource = BotSeedTypes::FTownspersonRoutePointSource;
using FTownspersonSource = BotSeedTypes::FTownspersonSource;

template <>
struct TJsonTextValueRegistry<ETownspersonInteractionIntent> {
  /** User Story: As a bots townspeople seed consumer, I need to invoke values through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn static const TArray<TValueDeclaration<ETownspersonInteractionIntent>> & Values() */
  static const TArray<TValueDeclaration<ETownspersonInteractionIntent>> &
  Values() {
    static const TArray<TValueDeclaration<ETownspersonInteractionIntent>>
        RegisteredValues = {
            {"general", ETownspersonInteractionIntent::General},
            {"dialogue", ETownspersonInteractionIntent::Dialogue},
            {"memory", ETownspersonInteractionIntent::Memory},
            {"combat_validation",
             ETownspersonInteractionIntent::CombatValidation}};
    return RegisteredValues;
  }
};

JSON_SETTINGS_REGISTRY(FTownspersonRoutePointSource, EastLots, NorthLots);
JSON_SETTINGS_REGISTRY(FInteractionSource, Intent, Prompt, DefaultPlayerLine,
                       PinnedResponse);
JSON_SETTINGS_REGISTRY(FTownspersonSource, Id, Name, Role, Persona,
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
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FTownspersonRouteLotsProjector)(
    const FTownspersonRouteLotsRequest &);

struct FTownspersonPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FTownspersonRouteLotsProjector ProjectLots;
};

struct FTownspersonBuildRequest {
  FTownspersonSource Fields;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

/** User Story: As a bots townspeople seed consumer, I need to invoke townsperson route lots through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn FLevelLocalPoint TownspersonRouteLots(const FTownspersonRouteLotsRequest &Request) */
FLevelLocalPoint
TownspersonRouteLots(const FTownspersonRouteLotsRequest &Request) {
  return LevelLayoutAdapters::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutAdapters::CharacterHeightOffset(Request.Geometry)});
}

/** User Story: As a bots townspeople seed consumer, I need to invoke interaction intent from json through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn ETownspersonInteractionIntent InteractionIntentFromJson( const FString &Intent) */
ETownspersonInteractionIntent InteractionIntentFromJson(
    const FString &Intent) {
  return JsonAdapters::RequireRegisteredTextValue<
      ETownspersonInteractionIntent>(
      Intent, TEXT("townsperson interaction intent"));
}

/** User Story: As a bots townspeople seed consumer, I need to invoke townsperson patrol route from fields through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn TArray<FLevelLocalPoint> TownspersonPatrolRouteFromFields( const FTownspersonPatrolRouteFieldsRequest &Request) */
TArray<FLevelLocalPoint>
TownspersonPatrolRouteFromFields(
    const FTownspersonPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Game::Data::FGeometrySettings Geometry =
      Request.Geometry;
  const FTownspersonRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FTownspersonRoutePointSource, FLevelLocalPoint>(
      JsonAdapters::MapSettingsJsonValues<FTownspersonRoutePointSource>(
          Request.Points, JSON_SETTINGS_ATOMS(EastLots, NorthLots)),
      [Geometry, ProjectLots](const FTownspersonRoutePointSource &Fields) {
        return ProjectLots({Geometry, Fields.EastLots, Fields.NorthLots});
      });
}

/** User Story: As a bots townspeople seed consumer, I need to invoke townsperson from fields through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn FTownspersonSeed TownspersonFromFields( const FTownspersonBuildRequest &Request) */
FTownspersonSeed TownspersonFromFields(
    const FTownspersonBuildRequest &Request) {
  const FInteractionSource Interaction =
      JsonAdapters::ReadSettingsFields<FInteractionSource>(
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

/** User Story: As a bots townspeople seed consumer, I need to invoke build townsperson seed through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn TArray<FTownspersonSeed> BuildTownspersonSeed( const FBotSeedBuildRequest &Request) */
TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request) {
  return func::map_array<FTownspersonSource, FTownspersonSeed>(
      JsonAdapters::MapSettingsJsonValues<FTownspersonSource>(
          JsonAdapters::LoadRequiredArrayFromContent(
              {Request.RelativeJsonPath}),
          JSON_SETTINGS_ATOMS(Id, Name, Role, Persona, Interaction,
                              PatrolRoute)),
      [&Request](const FTownspersonSource &Fields) {
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
  /** User Story: As a bots townspeople seed consumer, I need to invoke declarations through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn static const TArray< TComponentTextDeclaration<ETownspersonInteractionIntent>> &Declarations() */
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
  /** User Story: As a bots townspeople seed consumer, I need to invoke fields through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn static const TArray< TComponentSourceValueFieldDeclaration<FTownspersonSeed>> &Fields() */
  static const TArray<
      TComponentSourceValueFieldDeclaration<FTownspersonSeed>>
      &Fields() {
    static const TArray<TComponentSourceValueFieldDeclaration<
        FTownspersonSeed>>
        SourceFields = ComponentSourceFieldDeclarations<FTownspersonSeed>({
            {"Id", &FTownspersonSeed::Id},
            {"Name", &FTownspersonSeed::Name},
            {"Role", &FTownspersonSeed::Role},
            {"Persona", &FTownspersonSeed::Persona},
            {"InteractionPrompt", &FTownspersonSeed::InteractionPrompt},
            {"DefaultPlayerLine", &FTownspersonSeed::DefaultPlayerLine},
            {"PinnedResponse", &FTownspersonSeed::PinnedResponse},
            {"InteractionIntent", &FTownspersonSeed::InteractionIntent},
            {"PatrolRoute", &FTownspersonSeed::PatrolRoute}});
    return SourceFields;
  }
};

template <>
struct TComponentSourceProjector<FTownspersonSeed> {
  /** User Story: As a bots townspeople seed consumer, I need to invoke the callable value through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn ecs::FComponentValue operator()(const FTownspersonSeed &Townsperson) const */
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

/** User Story: As a bots townspeople seed consumer, I need to invoke bot entity key through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn extern ecs::EntityKey BotEntityKey(const FString &Id) */
extern ecs::EntityKey BotEntityKey(const FString &Id);

/** User Story: As a bots townspeople seed consumer, I need to invoke project townsperson through a stable signature so the bots townspeople seed workflow remains explicit and composable. @fn ecs::FWorld ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) */
ecs::FWorld
ProjectTownsperson(const FProjectTownspersonEntityPayload &Payload) {
  return ComponentsAdapters::ProjectEntityCatalog(
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
