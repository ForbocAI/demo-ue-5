#pragma once

#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

struct FFrenchGulchHorseRouteSeed {
  FString Name;
  TArray<FFrenchGulchLocalPoint> PatrolRoute;
};

namespace FrenchGulchHorses {
TArray<FFrenchGulchHorseRouteSeed> Build1899HorseRouteSeed();
} // namespace FrenchGulchHorses

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
