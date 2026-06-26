#pragma once

#include "CoreMinimal.h"
#include "Features/Components/MapLayout.h"

namespace ForbocAI {
namespace Demo {
namespace Map {

struct FMapHorseRouteSeed {
  FString Name;
  bool bMountedRider;
  TArray<FMapLocalPoint> PatrolRoute;
};

namespace HorseRoutes {
TArray<FMapHorseRouteSeed> Build1899HorseRouteSeed();
} // namespace HorseRoutes

} // namespace Map
} // namespace Demo
} // namespace ForbocAI
