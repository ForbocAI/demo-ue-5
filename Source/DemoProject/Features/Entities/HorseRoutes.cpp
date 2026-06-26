#include "Features/Entities/HorseRoutes.h"

namespace ForbocAI {
namespace Demo {
namespace Map {
namespace HorseRoutes {
namespace {
FMapLocalPoint HorseRouteLots(float EastLots, float NorthLots) {
  return MapLayout::FromPostOfficeLots(
      EastLots, NorthLots, MapLayout::RoadSurfaceClearance());
}

struct FHorseRouteSource {
  const TCHAR *Id;
  const TCHAR *Name;
  bool bMountedRider;
  std::initializer_list<FMapLocalPoint> Route;
};

FHorseRouteSeed HorseRoute(const FHorseRouteSource &Source) {
  FHorseRouteSeed Seed;
  Seed.Id = FString(Source.Id);
  Seed.Name = FString(Source.Name);
  Seed.bMountedRider = Source.bMountedRider;
  Seed.PatrolRoute.Reserve(static_cast<int32>(Source.Route.size()));
  for (const FMapLocalPoint &Point : Source.Route) {
    Seed.PatrolRoute.Add(Point);
  }
  return Seed;
}
} // namespace

TArray<FHorseRouteSeed> Build1899HorseRouteSeed() {
  return {
      HorseRoute({TEXT("sorrel-at-livery"), TEXT("Sorrel at livery"), false,
                  {HorseRouteLots(4.75f, -1.45f),
                   HorseRouteLots(2.75f, -1.05f),
                   HorseRouteLots(1.2f, -2.55f)}}),
      HorseRoute({TEXT("post-road-rider"), TEXT("Post road rider"), true,
                  {HorseRouteLots(0.2f, -0.65f),
                   HorseRouteLots(1.55f, 0.1f),
                   HorseRouteLots(3.6f, 0.95f)}}),
      HorseRoute({TEXT("mine-pack-horse"), TEXT("Mine pack horse"), false,
                  {HorseRouteLots(-1.35f, 4.85f),
                   HorseRouteLots(-2.55f, 6.6f),
                   HorseRouteLots(-0.45f, 2.8f)}}),
  };
}

} // namespace HorseRoutes
} // namespace Map
} // namespace Demo
} // namespace ForbocAI
