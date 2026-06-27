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
  float EastLots = 0.0f;
  float NorthLots = 0.0f;
};

struct FInteractionStringRequest {
  func::Maybe<TSharedPtr<FJsonObject>> Interaction;
  FString FieldName;
};

FLevelLocalPoint TownspersonRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      Request.EastLots, Request.NorthLots,
      LevelLayoutSlice::CharacterHeightOffset());
}

FLevelLocalPoint HorseRouteLots(const FRouteLotsRequest &Request) {
  return LevelLayoutSlice::FromPostOfficeLots(
      Request.EastLots, Request.NorthLots,
      LevelLayoutSlice::RoadSurfaceClearance());
}

ETownspersonInteractionIntent InteractionIntentFromJson(
    const FString &Intent) {
  return func::or_else(
      func::multi_match<FString, ETownspersonInteractionIntent>(
          Intent.ToLower(),
          {
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
          }),
      ETownspersonInteractionIntent::General);
}

TArray<FLevelLocalPoint> TownspersonPatrolRouteFromJson(
    const TSharedPtr<FJsonObject> &TownspersonObject) {
  return (func::pipe(
              DataAdapters::ReadArray({TownspersonObject, TEXT("patrol_route")})) |
          [](const TArray<TSharedPtr<FJsonValue>> &RouteValues)
              -> TArray<FLevelLocalPoint> {
            return DataAdapters::MapJsonValues<FLevelLocalPoint>(
                {RouteValues, [](const TSharedPtr<FJsonObject> &Point) {
                  return TownspersonRouteLots(
                      {DataAdapters::ReadFloat({Point, TEXT("east_lots")}),
                       DataAdapters::ReadFloat({Point, TEXT("north_lots")})});
                }});
          })
      .val;
}

TArray<FLevelLocalPoint> HorsePatrolRouteFromJson(
    const TSharedPtr<FJsonObject> &HorseObject) {
  return (func::pipe(
              DataAdapters::ReadArray({HorseObject, TEXT("patrol_route")})) |
          [](const TArray<TSharedPtr<FJsonValue>> &RouteValues)
              -> TArray<FLevelLocalPoint> {
            return DataAdapters::MapJsonValues<FLevelLocalPoint>(
                {RouteValues, [](const TSharedPtr<FJsonObject> &Point) {
                  return HorseRouteLots(
                      {DataAdapters::ReadFloat({Point, TEXT("east_lots")}),
                       DataAdapters::ReadFloat({Point, TEXT("north_lots")})});
                }});
          })
      .val;
}

FString ReadInteractionString(const FInteractionStringRequest &Request) {
  return func::match(
      Request.Interaction,
      [&Request](const TSharedPtr<FJsonObject> &InteractionObject) {
        return DataAdapters::ReadString({InteractionObject, Request.FieldName});
      },
      []() { return FString(); });
}

ETownspersonInteractionIntent ReadInteractionIntent(
    const func::Maybe<TSharedPtr<FJsonObject>> &Interaction) {
  return func::match(
      Interaction,
      [](const TSharedPtr<FJsonObject> &InteractionObject) {
        return InteractionIntentFromJson(
            DataAdapters::ReadString({InteractionObject, TEXT("intent")}));
      },
      []() { return ETownspersonInteractionIntent::General; });
}

FTownspersonSeed TownspersonFromJson(
    const TSharedPtr<FJsonObject> &TownspersonObject) {
  const func::Maybe<TSharedPtr<FJsonObject>> Interaction =
      DataAdapters::ReadObject({TownspersonObject, TEXT("interaction")});

  FTownspersonSeed Seed;
  Seed.Id = DataAdapters::ReadString({TownspersonObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({TownspersonObject, TEXT("name")});
  Seed.Role = DataAdapters::ReadString({TownspersonObject, TEXT("role")});
  Seed.Persona = DataAdapters::ReadString({TownspersonObject, TEXT("persona")});
  Seed.InteractionPrompt =
      ReadInteractionString({Interaction, TEXT("prompt")});
  Seed.DefaultPlayerLine =
      ReadInteractionString({Interaction, TEXT("default_player_line")});
  Seed.PinnedResponse =
      ReadInteractionString({Interaction, TEXT("pinned_response")});
  Seed.InteractionIntent = ReadInteractionIntent(Interaction);
  Seed.PatrolRoute = TownspersonPatrolRouteFromJson(TownspersonObject);
  return Seed;
}

FHorseRouteSeed HorseRouteFromJson(const TSharedPtr<FJsonObject> &HorseObject) {
  FHorseRouteSeed Seed;
  Seed.Id = DataAdapters::ReadString({HorseObject, TEXT("id")});
  Seed.Name = DataAdapters::ReadString({HorseObject, TEXT("name")});
  Seed.bMountedRider =
      DataAdapters::ReadBool({HorseObject, TEXT("mounted_rider")});
  Seed.PatrolRoute = HorsePatrolRouteFromJson(HorseObject);
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> Build1899TownspersonSeed() {
  return func::match(
      DataAdapters::LoadObjectFromContent({TownspeopleDataPath}),
      [](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FTownspersonSeed>(
            {DataAdapters::ReadArray({Root, TEXT("townspeople")}),
             TownspersonFromJson});
      },
      []() { return TArray<FTownspersonSeed>(); });
}

TArray<FHorseRouteSeed> Build1899HorseRouteSeed() {
  return func::match(
      DataAdapters::LoadObjectFromContent({HorseDataPath}),
      [](const TSharedPtr<FJsonObject> &Root) {
        return DataAdapters::MapJsonValues<FHorseRouteSeed>(
            {DataAdapters::ReadArray({Root, TEXT("horses")}),
             HorseRouteFromJson});
      },
      []() { return TArray<FHorseRouteSeed>(); });
}

} // namespace BotsAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
