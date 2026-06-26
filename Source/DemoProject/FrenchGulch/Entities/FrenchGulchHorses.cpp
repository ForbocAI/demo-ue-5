#include "FrenchGulch/Entities/FrenchGulchHorses.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {
namespace FrenchGulchHorses {
namespace {
FFrenchGulchLocalPoint HorseRouteLots(float EastLots, float NorthLots) {
  return FrenchGulchLayout::FromPostOfficeLots(
      EastLots, NorthLots, FrenchGulchLayout::RoadSurfaceClearance());
}

FFrenchGulchHorseRouteSeed HorseRoute(
    const TCHAR *Name, std::initializer_list<FFrenchGulchLocalPoint> Route) {
  FFrenchGulchHorseRouteSeed Seed;
  Seed.Name = FString(Name);
  Seed.PatrolRoute.Reserve(static_cast<int32>(Route.size()));
  for (const FFrenchGulchLocalPoint &Point : Route) {
    Seed.PatrolRoute.Add(Point);
  }
  return Seed;
}
} // namespace

TArray<FFrenchGulchHorseRouteSeed> Build1899HorseRouteSeed() {
  return {
      HorseRoute(TEXT("Sorrel at livery"),
                 {HorseRouteLots(4.75f, -1.45f),
                  HorseRouteLots(2.75f, -1.05f),
                  HorseRouteLots(1.2f, -2.55f)}),
      HorseRoute(TEXT("Post road mare"),
                 {HorseRouteLots(0.2f, -0.65f),
                  HorseRouteLots(1.55f, 0.1f),
                  HorseRouteLots(3.6f, 0.95f)}),
      HorseRoute(TEXT("Mine pack horse"),
                 {HorseRouteLots(-1.35f, 4.85f),
                  HorseRouteLots(-2.55f, 6.6f),
                  HorseRouteLots(-0.45f, 2.8f)}),
  };
}

} // namespace FrenchGulchHorses
} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
