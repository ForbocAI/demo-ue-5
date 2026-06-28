#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/DataAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotsAdapters {

namespace DataAdapters = ForbocAI::Demo::Data::DataAdapters;

namespace {

constexpr const TCHAR *TownspeopleDataPath =
    TEXT("Data/french_gulch_townspeople.json");
constexpr const TCHAR *HorseDataPath = TEXT("Data/french_gulch_horses.json");

struct FRouteLotsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
};

struct FInteractionStringRequest {
  func::Maybe<TSharedPtr<FJsonObject>> Interaction;
  FString FieldName;
};

struct FTownspersonFromJsonRequest {
  TSharedPtr<FJsonObject> TownspersonObject;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FHorseRouteFromJsonRequest {
  TSharedPtr<FJsonObject> HorseObject;
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
      func::multi_match<FString, ETownspersonInteractionIntent>(
          Intent.ToLower(),
          {
              func::when<FString, ETownspersonInteractionIntent>(
                  func::equals<FString>(TEXT("general")),
                  [](const FString &) {
                    return ETownspersonInteractionIntent::General;
                  }),
              func::when<FString, ETownspersonInteractionIntent>(
                  func::equals<FString>(TEXT("dialogue")),
                  [](const FString &) {
                    return ETownspersonInteractionIntent::Dialogue;
                  }),
              func::when<FString, ETownspersonInteractionIntent>(
                  func::equals<FString>(TEXT("memory")),
                  [](const FString &) {
                    return ETownspersonInteractionIntent::Memory;
                  }),
              func::when<FString, ETownspersonInteractionIntent>(
                  func::equals<FString>(TEXT("combat_validation")),
                  [](const FString &) {
                    return ETownspersonInteractionIntent::CombatValidation;
                  }),
          });
  checkf(Parsed.hasValue, TEXT("Invalid townsperson interaction intent: %s"),
         *Intent);
  return Parsed.value;
}

TArray<FLevelLocalPoint> TownspersonPatrolRouteFromJson(
    const TSharedPtr<FJsonObject> &TownspersonObject,
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return (func::pipe(
              DataAdapters::ReadArray({TownspersonObject, TEXT("patrol_route")})) |
          [&Geometry](const TArray<TSharedPtr<FJsonValue>> &RouteValues)
              -> TArray<FLevelLocalPoint> {
            return DataAdapters::MapJsonValues<FLevelLocalPoint>(
                {RouteValues, [&Geometry](const TSharedPtr<FJsonObject> &Point) {
                  return TownspersonRouteLots(
                      {Geometry,
                       DataAdapters::ReadFloat({Point, TEXT("east_lots")}),
                       DataAdapters::ReadFloat({Point, TEXT("north_lots")})});
                }});
          })
      .val;
}

TArray<FLevelLocalPoint> HorsePatrolRouteFromJson(
    const TSharedPtr<FJsonObject> &HorseObject,
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return (func::pipe(
              DataAdapters::ReadArray({HorseObject, TEXT("patrol_route")})) |
          [&Geometry](const TArray<TSharedPtr<FJsonValue>> &RouteValues)
              -> TArray<FLevelLocalPoint> {
            return DataAdapters::MapJsonValues<FLevelLocalPoint>(
                {RouteValues, [&Geometry](const TSharedPtr<FJsonObject> &Point) {
                  return HorseRouteLots(
                      {Geometry,
                       DataAdapters::ReadFloat({Point, TEXT("east_lots")}),
                       DataAdapters::ReadFloat({Point, TEXT("north_lots")})});
                }});
          })
      .val;
}

FString ReadInteractionString(const FInteractionStringRequest &Request) {
  checkf(Request.Interaction.hasValue,
         TEXT("Townsperson interaction object is required"));
  return DataAdapters::ReadString({Request.Interaction.value,
                                   Request.FieldName});
}

ETownspersonInteractionIntent ReadInteractionIntent(
    const func::Maybe<TSharedPtr<FJsonObject>> &Interaction) {
  checkf(Interaction.hasValue,
         TEXT("Townsperson interaction object is required"));
  return InteractionIntentFromJson(
      DataAdapters::ReadString({Interaction.value, TEXT("intent")}));
}

FTownspersonSeed TownspersonFromJson(
    const FTownspersonFromJsonRequest &Request) {
  const func::Maybe<TSharedPtr<FJsonObject>> Interaction =
      DataAdapters::ReadObject({Request.TownspersonObject, TEXT("interaction")});

  FTownspersonSeed Seed;
  Seed.Id = DataAdapters::ReadString({Request.TownspersonObject, TEXT("id")});
  Seed.Name =
      DataAdapters::ReadString({Request.TownspersonObject, TEXT("name")});
  Seed.Role =
      DataAdapters::ReadString({Request.TownspersonObject, TEXT("role")});
  Seed.Persona =
      DataAdapters::ReadString({Request.TownspersonObject, TEXT("persona")});
  Seed.InteractionPrompt =
      ReadInteractionString({Interaction, TEXT("prompt")});
  Seed.DefaultPlayerLine =
      ReadInteractionString({Interaction, TEXT("default_player_line")});
  Seed.PinnedResponse =
      ReadInteractionString({Interaction, TEXT("pinned_response")});
  Seed.InteractionIntent = ReadInteractionIntent(Interaction);
  Seed.PatrolRoute =
      TownspersonPatrolRouteFromJson(Request.TownspersonObject,
                                     Request.Geometry);
  return Seed;
}

FHorseRouteSeed HorseRouteFromJson(
    const FHorseRouteFromJsonRequest &Request) {
  FHorseRouteSeed Seed;
  Seed.Id = DataAdapters::ReadString({Request.HorseObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({Request.HorseObject, TEXT("name")});
  Seed.bMountedRider =
      DataAdapters::ReadBool({Request.HorseObject, TEXT("mounted_rider")});
  Seed.PatrolRoute =
      HorsePatrolRouteFromJson(Request.HorseObject, Request.Geometry);
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> Build1899TownspersonSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return func::match(
      DataAdapters::LoadObjectFromContent({TownspeopleDataPath}),
      [&Geometry](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FTownspersonSeed>(
            {DataAdapters::ReadArray({Root, TEXT("townspeople")}),
             [&Geometry](const TSharedPtr<FJsonObject> &TownspersonObject) {
               return TownspersonFromJson({TownspersonObject, Geometry});
             }});
      },
      []() { return TArray<FTownspersonSeed>(); });
}

TArray<FHorseRouteSeed> Build1899HorseRouteSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return func::match(
      DataAdapters::LoadObjectFromContent({HorseDataPath}),
      [&Geometry](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FHorseRouteSeed>(
            {DataAdapters::ReadArray({Root, TEXT("horses")}),
             [&Geometry](const TSharedPtr<FJsonObject> &HorseObject) {
               return HorseRouteFromJson({HorseObject, Geometry});
             }});
      },
      []() { return TArray<FHorseRouteSeed>(); });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
