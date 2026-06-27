#pragma once

#include "CoreMinimal.h"

class FLevelOrthoData;
class FLevelTerrainData;

namespace ForbocAI {
namespace Demo {
namespace Level {
struct FHorseRouteSeed;
struct FLandmark;
struct FNatureFeatureSeed;
struct FTownspersonSeed;

namespace RuntimeAdapters {

class FRuntimeLevelSession {
public:
  FRuntimeLevelSession();

  void Seed(const FLevelTerrainData &TerrainData,
            const FLevelOrthoData &OrthoData);

  TArray<FLandmark> SelectLandmarks() const;
  TArray<FNatureFeatureSeed> SelectNatureFeatures() const;
  TArray<FTownspersonSeed> SelectTownspeople() const;
  TArray<FHorseRouteSeed> SelectHorses() const;

private:
  struct FStore;
  TSharedPtr<FStore> Store;
};

} // namespace RuntimeAdapters
} // namespace Level
} // namespace Demo
} // namespace ForbocAI
