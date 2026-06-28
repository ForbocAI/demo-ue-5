#include "Features/Entities/Characters/Bots/BotsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"

namespace ForbocAI {
namespace Demo {
namespace Level {
namespace BotsAdapters {

namespace JsonAdapters = ForbocAI::Demo::Data::JsonAdapters;

namespace {

constexpr const TCHAR *TownspeopleDataPath =
    TEXT("Data/french_gulch_townspeople.json");
constexpr const TCHAR *HorseDataPath = TEXT("Data/french_gulch_horses.json");

struct FRouteLotsRequest {
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
};

typedef TFunction<FLevelLocalPoint(const FRouteLotsRequest &)>
    FRouteLotsProjector;

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

struct FPatrolRouteFromJsonRequest {
  TSharedPtr<FJsonObject> RouteOwner;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  FRouteLotsProjector ProjectLots;
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

TArray<FLevelLocalPoint>
PatrolRouteFromJson(const FPatrolRouteFromJsonRequest &Request) {
  const JsonAdapters::FJsonArrayReader Array =
      JsonAdapters::ArrayIn(Request.RouteOwner);
  const ForbocAI::Demo::Data::FLevelGeometrySettings Geometry =
      Request.Geometry;
  const FRouteLotsProjector ProjectLots = Request.ProjectLots;
  return JsonAdapters::MapJsonValues<FLevelLocalPoint>(
      Array(TEXT("patrol_route")),
      [Geometry, ProjectLots](const TSharedPtr<FJsonObject> &Point) {
        const JsonAdapters::FJsonFloatReader Float =
            JsonAdapters::FloatIn(Point);
        return ProjectLots(
            {Geometry, Float(TEXT("east_lots")), Float(TEXT("north_lots"))});
      });
}

FString ReadInteractionString(const FInteractionStringRequest &Request) {
  checkf(Request.Interaction.hasValue,
         TEXT("Townsperson interaction object is required"));
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.Interaction.value);
  return String(*Request.FieldName);
}

ETownspersonInteractionIntent ReadInteractionIntent(
    const func::Maybe<TSharedPtr<FJsonObject>> &Interaction) {
  checkf(Interaction.hasValue,
         TEXT("Townsperson interaction object is required"));
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Interaction.value);
  return InteractionIntentFromJson(String(TEXT("intent")));
}

FTownspersonSeed TownspersonFromJson(
    const FTownspersonFromJsonRequest &Request) {
  const JsonAdapters::FJsonObjectReader Object =
      JsonAdapters::ObjectIn(Request.TownspersonObject);
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.TownspersonObject);
  const func::Maybe<TSharedPtr<FJsonObject>> Interaction =
      Object(TEXT("interaction"));

  FTownspersonSeed Seed;
  Seed.Id = String(TEXT("id"));
  Seed.Name = String(TEXT("name"));
  Seed.Role = String(TEXT("role"));
  Seed.Persona = String(TEXT("persona"));
  Seed.InteractionPrompt =
      ReadInteractionString({Interaction, TEXT("prompt")});
  Seed.DefaultPlayerLine =
      ReadInteractionString({Interaction, TEXT("default_player_line")});
  Seed.PinnedResponse =
      ReadInteractionString({Interaction, TEXT("pinned_response")});
  Seed.InteractionIntent = ReadInteractionIntent(Interaction);
  Seed.PatrolRoute = PatrolRouteFromJson(
      {Request.TownspersonObject, Request.Geometry, TownspersonRouteLots});
  return Seed;
}

FHorseRouteSeed HorseRouteFromJson(
    const FHorseRouteFromJsonRequest &Request) {
  const JsonAdapters::FJsonStringReader String =
      JsonAdapters::StringIn(Request.HorseObject);
  const JsonAdapters::FJsonBoolReader Bool =
      JsonAdapters::BoolIn(Request.HorseObject);
  FHorseRouteSeed Seed;
  Seed.Id = String(TEXT("id"));
  Seed.Name = String(TEXT("name"));
  Seed.bMountedRider = Bool(TEXT("mounted_rider"));
  Seed.PatrolRoute =
      PatrolRouteFromJson({Request.HorseObject, Request.Geometry,
                           HorseRouteLots});
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> Build1899TownspersonSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return func::match(
      JsonAdapters::LoadObjectFromContent({TownspeopleDataPath}),
      [&Geometry](const TSharedPtr<FJsonObject> &Root) {
        const JsonAdapters::FJsonArrayReader Array = JsonAdapters::ArrayIn(Root);
        return JsonAdapters::MapJsonValues<FTownspersonSeed>(
            Array(TEXT("townspeople")),
            [&Geometry](const TSharedPtr<FJsonObject> &TownspersonObject) {
              return TownspersonFromJson({TownspersonObject, Geometry});
            });
      },
      []() {
        checkf(false, TEXT("Townsperson seed JSON is required"));
        return TArray<FTownspersonSeed>();
      });
}

TArray<FHorseRouteSeed> Build1899HorseRouteSeed(
    const ForbocAI::Demo::Data::FLevelGeometrySettings &Geometry) {
  return func::match(
      JsonAdapters::LoadObjectFromContent({HorseDataPath}),
      [&Geometry](const TSharedPtr<FJsonObject> &Root) {
        const JsonAdapters::FJsonArrayReader Array = JsonAdapters::ArrayIn(Root);
        return JsonAdapters::MapJsonValues<FHorseRouteSeed>(
            Array(TEXT("horses")),
            [&Geometry](const TSharedPtr<FJsonObject> &HorseObject) {
              return HorseRouteFromJson({HorseObject, Geometry});
            });
      },
      []() {
        checkf(false, TEXT("Horse route seed JSON is required"));
        return TArray<FHorseRouteSeed>();
      });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
