#pragma once

#include "Features/Entities/Environments/Nature/Seed/SeedAdapters.h"

#include "Features/Components/Data/Json/Settings/JsonSettingsAdapters.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Components/ComponentsAdapters.h"
#include "Features/Entities/EntitiesAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace NatureAdapters {

namespace JsonAdapters = ForbocAI::Game::Data::JsonAdapters;

enum class ENatureScaleMode { LongFeature, Pad };

struct FNatureScaleModeSource {
  FString Mode;
};

struct FNaturePadScaleSource {
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
};

struct FNatureLongFeatureScaleSource {
  float WidthFeet;
  float LengthLots;
  float HeightFeet;
};

struct FNatureFeatureSource {
  FString Id;
  FString Name;
  FString Kind;
  float EastLots;
  float NorthLots;
  TSharedPtr<FJsonObject> Scale;
};

struct FFeatureLotsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  float EastLots;
  float NorthLots;
  FVector Scale;
};

struct FScaleFieldsRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  TSharedPtr<FJsonObject> Object;
};

typedef FVector (*FScaleFieldsProjector)(const FScaleFieldsRequest &);

struct FScaleProjectorDeclaration {
  ENatureScaleMode Mode;
  FScaleFieldsProjector Project;

  FScaleProjectorDeclaration() = default;

  FScaleProjectorDeclaration(ENatureScaleMode InMode,
                             FScaleFieldsProjector InProject)
      : Mode(InMode), Project(InProject) {}
};

struct FFeatureBuildRequest {
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  FNatureFeatureSource Fields;
};

} // namespace NatureAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
