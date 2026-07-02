#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

#include <initializer_list>

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

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

struct FRouteLotsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
};

typedef FLevelLocalPoint (*FRouteLotsProjector)(const FRouteLotsRequest &);

struct FPatrolRouteFieldsRequest {
  TArray<TSharedPtr<FJsonValue>> Points;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FRouteLotsProjector ProjectLots;
};

struct FTownspersonBuildRequest {
  FTownspersonFields Fields;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FHorseRouteBuildRequest {
  FHorseRouteFields Fields;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
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
  const func::Maybe<ETownspersonInteractionIntent> Parsed =
      JsonAdapters::ReadTextValue<ETownspersonInteractionIntent>(
          Intent, {{"general", ETownspersonInteractionIntent::General},
                   {"dialogue", ETownspersonInteractionIntent::Dialogue},
                   {"memory", ETownspersonInteractionIntent::Memory},
                   {"combat_validation",
                    ETownspersonInteractionIntent::CombatValidation}});
  checkf(Parsed.hasValue, TEXT("Invalid townsperson interaction intent: %s"),
         *Intent);
  return Parsed.value;
}

FRoutePointFields ReadRoutePointFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FRoutePointFields>(
      Object, JSON_SETTINGS_FIELDS(FRoutePointFields, EastLots, NorthLots));
}

FInteractionFields
ReadInteractionFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FInteractionFields>(
      Object, JSON_SETTINGS_FIELDS(FInteractionFields, Intent, Prompt,
                                   DefaultPlayerLine, PinnedResponse));
}

FTownspersonFields
ReadTownspersonFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FTownspersonFields>(
      Object, JSON_SETTINGS_FIELDS(FTownspersonFields, Id, Name, Role,
                                   Persona, Interaction, PatrolRoute));
}

FHorseRouteFields ReadHorseRouteFields(const TSharedPtr<FJsonObject> &Object) {
  return JsonAdapters::ReadSettingsFields<FHorseRouteFields>(
      Object, JSON_SETTINGS_FIELDS(FHorseRouteFields, Id, Name, bMountedRider,
                                   PatrolRoute));
}

TArray<FLevelLocalPoint>
PatrolRouteFromFields(const FPatrolRouteFieldsRequest &Request) {
  const ForbocAI::Demo::Data::FLevelGeometrySettings Geometry =
      Request.Geometry;
  const FRouteLotsProjector ProjectLots = Request.ProjectLots;
  return func::map_array<FRoutePointFields, FLevelLocalPoint>(
      JsonAdapters::MapJsonValues<FRoutePointFields>(Request.Points,
                                                     ReadRoutePointFields),
      [Geometry, ProjectLots](const FRoutePointFields &Fields) {
        return ProjectLots({Geometry, Fields.EastLots, Fields.NorthLots});
      });
}

FTownspersonSeed TownspersonFromFields(
    const FTownspersonBuildRequest &Request) {
  const FInteractionFields Interaction =
      ReadInteractionFields(Request.Fields.Interaction);
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
      JsonAdapters::ReadObjectArrayField<FTownspersonFields>(
          Root, "Townspeople", ReadTownspersonFields),
      [&Request](const FTownspersonFields &Fields) {
        return TownspersonFromFields({Fields, Request.Geometry});
      });
}

TArray<FHorseRouteSeed> BuildHorseRouteSeed(
    const FBotSeedBuildRequest &Request) {
  const TSharedPtr<FJsonObject> Root =
      JsonAdapters::LoadRequiredObjectFromContent({Request.RelativeJsonPath});
  return func::map_array<FHorseRouteFields, FHorseRouteSeed>(
      JsonAdapters::ReadObjectArrayField<FHorseRouteFields>(
          Root, "Horses", ReadHorseRouteFields),
      [&Request](const FHorseRouteFields &Fields) {
        return HorseRouteFromFields({Fields, Request.Geometry});
      });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
