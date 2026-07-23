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

/** User Story: As a features systems route consumer, I need to invoke initial route bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds InitialRouteBounds(const FLevelLocalPoint &Point) */
FLocalRouteBounds InitialRouteBounds(const FLevelLocalPoint &Point) {
  return {FVector2D(Point.EastWest, Point.NorthSouth),
          FVector2D(Point.EastWest, Point.NorthSouth), true};
}

/** User Story: As a features systems route consumer, I need to invoke extend route bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds ExtendRouteBounds(const FLocalRouteBounds &Bounds, const FLevelLocalPoint &Point) */
FLocalRouteBounds ExtendRouteBounds(const FLocalRouteBounds &Bounds,
                                    const FLevelLocalPoint &Point) {
  return {FVector2D(FMath::Min(Bounds.Min.X, Point.EastWest),
                    FMath::Min(Bounds.Min.Y, Point.NorthSouth)),
          FVector2D(FMath::Max(Bounds.Max.X, Point.EastWest),
                    FMath::Max(Bounds.Max.Y, Point.NorthSouth)),
          true};
}

/** User Story: As a features systems route consumer, I need to invoke reduce route point bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds ReduceRoutePointBounds(FLocalRouteBounds Bounds, const FLevelLocalPoint &Point) */
FLocalRouteBounds ReduceRoutePointBounds(FLocalRouteBounds Bounds,
                                         const FLevelLocalPoint &Point) {
  return Bounds.bValid ? ExtendRouteBounds(Bounds, Point)
                       : InitialRouteBounds(Point);
}

/** User Story: As a features systems route consumer, I need to invoke reduce townsperson bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds ReduceTownspersonBounds(FLocalRouteBounds Bounds, const FTownspersonSeed &Seed) */
FLocalRouteBounds ReduceTownspersonBounds(FLocalRouteBounds Bounds,
                                          const FTownspersonSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

/** User Story: As a features systems route consumer, I need to invoke reduce horse bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds ReduceHorseBounds(FLocalRouteBounds Bounds, const FHorseRouteSeed &Seed) */
FLocalRouteBounds ReduceHorseBounds(FLocalRouteBounds Bounds,
                                    const FHorseRouteSeed &Seed) {
  return func::fold_indexed(Seed.PatrolRoute,
                            static_cast<size_t>(Seed.PatrolRoute.Num()), Bounds,
                            ReduceRoutePointBounds);
}

/** User Story: As a features systems route consumer, I need to invoke dynamic route bounds through a stable signature so the features systems route workflow remains explicit and composable. @fn FLocalRouteBounds DynamicRouteBounds(const FRuntimeState &State) */
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

/** User Story: As a features systems route consumer, I need to invoke route bounds center through a stable signature so the features systems route workflow remains explicit and composable. @fn FVector RouteBoundsCenter(const FLocalRouteBounds &Bounds, const FVector &Fallback) */
FVector RouteBoundsCenter(const FLocalRouteBounds &Bounds,
                          const FVector &Fallback) {
  return Bounds.bValid ? FVector((Bounds.Min.X + Bounds.Max.X) * 0.5f,
                                 (Bounds.Min.Y + Bounds.Max.Y) * 0.5f, 0.0f)
                       : Fallback;
}

} // namespace

/** User Story: As a features systems route consumer, I need to invoke select actor route bounds center through a stable signature so the features systems route workflow remains explicit and composable. @fn FVector SelectActorRouteBoundsCenter(const FRuntimeState &State, const FVector &Fallback) */
FVector SelectActorRouteBoundsCenter(const FRuntimeState &State,
                                     const FVector &Fallback) {
  return RouteBoundsCenter(DynamicRouteBounds(State), Fallback);
}

/** User Story: As a features systems route consumer, I need to invoke select actor route bounds center through a stable signature so the features systems route workflow remains explicit and composable. @fn FVector SelectActorRouteBoundsCenter(const FVector &Fallback) */
FVector SelectActorRouteBoundsCenter(const FVector &Fallback) {
  return SelectActorRouteBoundsCenter(SelectState(), Fallback);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
