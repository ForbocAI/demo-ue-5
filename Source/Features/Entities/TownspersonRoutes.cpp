#include "Features/Entities/TownspersonRoutes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace TownspersonRoutes {
namespace {

FMapLocalPoint RouteLots(float EastLots, float NorthLots) {
  return MapLayout::FromPostOfficeLots(EastLots, NorthLots,
                                       MapLayout::CharacterHeightOffset());
}

struct FTownspersonSource {
  const TCHAR *Id;
  const TCHAR *Name;
  const TCHAR *Role;
  const TCHAR *Persona;
  std::initializer_list<FMapLocalPoint> Route;
};

FTownspersonSeed Townsperson(const FTownspersonSource &Source) {
  FTownspersonSeed Seed;
  Seed.Id = FString(Source.Id);
  Seed.Name = FString(Source.Name);
  Seed.Role = FString(Source.Role);
  Seed.Persona = FString(Source.Persona);
  Seed.PatrolRoute.Reserve(static_cast<int32>(Source.Route.size()));
  for (const FMapLocalPoint &Point : Source.Route) {
    Seed.PatrolRoute.Add(Point);
  }
  return Seed;
}

} // namespace

TArray<FTownspersonSeed> Build1899TownspersonSeed() {
  return {
      Townsperson({TEXT("elias-reed"), TEXT("Elias Reed"), TEXT("Miner"),
                   TEXT("I know which shafts are paying and which timbers are tired."),
                   {RouteLots(-1.8f, 5.3f), RouteLots(-2.825f, 6.725f),
                    RouteLots(-0.5f, 2.6f)}}),
      Townsperson({TEXT("clara-bell"), TEXT("Clara Bell"), TEXT("Postmaster"),
                   TEXT("I sort mail, freight news, and every rumor that rides Main Street."),
                   {RouteLots(0.0f, -0.175f), RouteLots(0.9f, 0.4f),
                    RouteLots(-0.35f, 0.625f)}}),
      Townsperson({TEXT("deputy-hale"), TEXT("Deputy Hale"), TEXT("Deputy"),
                   TEXT("I keep the peace between claim jumpers, teamsters, and drifters."),
                   {RouteLots(2.3f, 1.8f), RouteLots(3.475f, 4.2f),
                    RouteLots(2.9f, -0.85f)}}),
      Townsperson({TEXT("mara-quinn"), TEXT("Mara Quinn"), TEXT("Traveler"),
                   TEXT("I came over the ridge with news from Redding and a wary horse."),
                   {RouteLots(1.525f, -2.5f), RouteLots(3.825f, 1.15f),
                    RouteLots(4.775f, -1.15f)}}),
      Townsperson({TEXT("samuel-pike"), TEXT("Samuel Pike"), TEXT("Hotel Keeper"),
                   TEXT("Beds, meals, and claims gossip all pass through the hotel desk."),
                   {RouteLots(3.85f, 1.175f), RouteLots(2.9f, -0.95f),
                    RouteLots(4.6f, 0.4f)}}),
  };
}

} // namespace TownspersonRoutes
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
