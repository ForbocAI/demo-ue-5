#pragma once

#include "CoreMinimal.h"
#include "FrenchGulch/Components/FrenchGulchLayout.h"

namespace ForbocAI {
namespace Demo {
namespace FrenchGulch {

enum class EFrenchGulchNatureFeatureKind {
  Water,
  Rock,
  TreeGrove,
  Shrub,
  PCGMarker,
  WaterSystemMarker
};

struct FFrenchGulchNatureFeatureSeed {
  FString Name;
  EFrenchGulchNatureFeatureKind Kind;
  FFrenchGulchLocalPoint Location;
  FVector Scale;
};

namespace FrenchGulchNature {
TArray<FFrenchGulchNatureFeatureSeed> BuildClearCreekNatureSeed();
} // namespace FrenchGulchNature

} // namespace FrenchGulch
} // namespace Demo
} // namespace ForbocAI
