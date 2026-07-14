#include "Features/Systems/SystemsSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RuntimeSelectors {

namespace {

struct FLocalRouteBounds {
  FVector2D Min = FVector2D::ZeroVector;
  FVector2D Max = FVector2D::ZeroVector;
  bool bValid = false;
};

FLocalRouteBounds InitialRouteBounds(const FLevelLocalPoint &Point) {
  return {FVector2D(Point.EastWest, Point.NorthSouth),
          FVector2D(Point.EastWest, Point.NorthSouth), true};
}

FLocalRouteBounds ExtendRouteBounds(const FLocalRouteBounds &Bounds,
                                    const FLevelLocalPoint &Point) {
  return {FVector2D(FMath::Min(Bounds.Min.X, Point.EastWest),
                    FMath::Min(Bounds.Min.Y, Point.NorthSouth)),
          FVector2D(FMath::Max(Bounds.Max.X, Point.EastWest),
                    FMath::Max(Bounds.Max.Y, Point.NorthSouth)),
          true};
}

FLocalRouteBounds ReduceRoutePointBounds(FLocalRouteBounds Bounds,
                                         const FLevelLocalPoint &Point) {
  return Bounds.bValid ? ExtendRouteBounds(Bounds, Point)
                       : InitialRouteBounds(Point);
}

FLocalRouteBounds ReduceTownspersonBounds(FLocalRouteBounds Bounds,
                                          const FTownspersonSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

FLocalRouteBounds ReduceHorseBounds(FLocalRouteBounds Bounds,
                                    const FHorseRouteSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

FLocalRouteBounds DynamicRouteBounds(const FRuntimeState &State) {
  const TArray<FTownspersonSeed> Townspeople =
      RuntimeSelectors::SelectTownspeople(State);
  const TArray<FHorseRouteSeed> Horses = RuntimeSelectors::SelectHorses(State);
  const FLocalRouteBounds TownspersonBounds = func::fold_indexed(
      Townspeople, static_cast<size_t>(Townspeople.Num()), FLocalRouteBounds{},
      ReduceTownspersonBounds);
  return func::fold_indexed(Horses, static_cast<size_t>(Horses.Num()),
                            TownspersonBounds, ReduceHorseBounds);
}

FVector RouteBoundsCenter(const FLocalRouteBounds &Bounds,
                          const FVector &Fallback) {
  return Bounds.bValid ? FVector((Bounds.Min.X + Bounds.Max.X) * 0.5f,
                                 (Bounds.Min.Y + Bounds.Max.Y) * 0.5f, 0.0f)
                       : Fallback;
}

} // namespace

FVector SelectActorRouteBoundsCenter(const FRuntimeState &State,
                                     const FVector &Fallback) {
  return RouteBoundsCenter(DynamicRouteBounds(State), Fallback);
}

FVector SelectActorRouteBoundsCenter(const FVector &Fallback) {
  return SelectActorRouteBoundsCenter(SelectState(), Fallback);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
