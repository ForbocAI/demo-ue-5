#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

struct FRoutePointFields {
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

struct FHorseRouteFields {
  FString Id;
  FString Name;
  bool bMountedRider;
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
using FHorseRouteFields = BotSeedTypes::FHorseRouteFields;
using FInteractionFields = BotSeedTypes::FInteractionFields;
using FRoutePointFields = BotSeedTypes::FRoutePointFields;
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

JSON_SETTINGS_REGISTRY(FRoutePointFields, EastLots, NorthLots);
JSON_SETTINGS_REGISTRY(FInteractionFields, Intent, Prompt, DefaultPlayerLine,
                       PinnedResponse);
JSON_SETTINGS_REGISTRY(FTownspersonFields, Id, Name, Role, Persona,
                       Interaction, PatrolRoute);
JSON_SETTINGS_REGISTRY(FHorseRouteFields, Id, Name, bMountedRider,
                       PatrolRoute);

} // namespace JsonAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI

namespace ForbocAI {
namespace Game {
namespace Level {
namespace BotsAdapters {
namespace {

struct FRouteLotsRequest {
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FRouteLotsProjector)(const FRouteLotsRequest &);

struct FPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
  FRouteLotsProjector ProjectLots;
};

struct FTownspersonBuildRequest {
  FTownspersonFields Fields;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FHorseRouteBuildRequest {
  FHorseRouteFields Fields;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

FLevelLocalPoint TownspersonRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutSlice::CharacterHeightOffset(Request.Geometry)});
}

FLevelLocalPoint HorseRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      {Request.Geometry, Request.EastLots, Request.NorthLots,
       LevelLayoutSlice::RoadSurfaceClearance(Request.Geometry)});
}

ETownspersonInteractionIntent InteractionIntentFromJson(
    const FString &Intent) {
  return JsonAdapters::RequireRegisteredTextValue<
      ETownspersonInteractionIntent>(
      Intent, TEXT("townsperson interaction intent"));
}

TArray<FLevelLocalPoint>
PatrolRouteFromFields(const FPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Game::Data::FLevelGeometrySettings Geometry =
      Request.Geometry;
  const FRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FRoutePointFields, FLevelLocalPoint>(
      JsonAdapters::MapSettingsJsonValues<FRoutePointFields>(
          Request.Points, JSON_SETTINGS_ATOMS(EastLots, NorthLots)),
      [Geometry, ProjectLots](const FRoutePointFields &Fields) {
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
  Seed.PatrolRoute = PatrolRouteFromFields(
      {Request.Fields.PatrolRoute, Request.Geometry, TownspersonRouteLots});
  return Seed;
}

FHorseRouteSeed HorseRouteFromFields(
    const FHorseRouteBuildRequest &Request) {
  FHorseRouteSeed Seed;
  Seed.Id = Request.Fields.Id;
  Seed.Name = Request.Fields.Name;
  Seed.bMountedRider = Request.Fields.bMountedRider;
  Seed.PatrolRoute = PatrolRouteFromFields(
      {Request.Fields.PatrolRoute, Request.Geometry, HorseRouteLots});
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> BuildTownspersonSeed(
    const FBotSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FTownspersonFields, FTownspersonSeed>(
      JsonAdapters::ReadSettingsObjectArrayField<FTownspersonFields>(
          Root, "Townspeople",
          JSON_SETTINGS_ATOMS(Id, Name, Role, Persona, Interaction,
                              PatrolRoute)),
      [&Request](const FTownspersonFields &Fields) {
        return TownspersonFromFields({Fields, Request.Geometry});
      });
}

TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FHorseRouteFields, FHorseRouteSeed>(
      JsonAdapters::ReadSettingsObjectArrayField<FHorseRouteFields>(
          Root, "Horses",
          JSON_SETTINGS_ATOMS(Id, Name, bMountedRider, PatrolRoute)),
      [&Request](const FHorseRouteFields &Fields) {
        return HorseRouteFromFields({Fields, Request.Geometry});
      });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
