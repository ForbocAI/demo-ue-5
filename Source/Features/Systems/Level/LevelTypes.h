#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/LevelTypes.h"
#include "Features/Components/Rendering/RenderingTypes.h"
#include "Features/Components/Spatial/LevelLayoutSlice.h"
#include "Features/Systems/Landmarks/LandmarkTypes.h"
#include "Features/Systems/Nature/NatureTypes.h"

namespace ForbocAI {
namespace Demo {
namespace Level {

enum class ELevelRuntimeScaleMode {
  Building,
  LongFeature,
  Pad
};

enum class ELevelRuntimeAnchorMode {
  BuildingLots,
  FeatureLots,
  PostOfficeLots,
  World
};

enum class ELevelRuntimeLabelHeightMode {
  Explicit,
  LabelForScale,
  AboveBlock
};

struct FLevelRuntimeScaleSeed {
  ELevelRuntimeScaleMode Mode;
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
  float FrontageFeet;
  float Stories;
  float LengthLots;
};

struct FLevelRuntimeBlockSeed {
  FString Id;
  FString Name;
  ELevelRuntimeAnchorMode Anchor;
  float EastLots;
  float NorthLots;
  FVector WorldLocation;
  FLevelRuntimeScaleSeed Scale;
  ELevelRetroTexture Texture;
};

struct FLevelRuntimeLabelSeed {
  FString Id;
  FString Text;
  ELevelRuntimeAnchorMode Anchor;
  ELevelRuntimeLabelHeightMode Height;
  float EastLots;
  float NorthLots;
  float HeightOffset;
  float WorldSizeScale;
  FLevelRuntimeScaleSeed ReferenceScale;
};

struct FLevelRuntimeSectionSeed {
  TArray<FLevelRuntimeBlockSeed> Blocks;
  TArray<FLevelRuntimeLabelSeed> Labels;
};

struct FLevelRuntimeLayoutSeed {
  FLevelRuntimeSectionSeed Terrain;
  FLevelRuntimeSectionSeed Town;
  FLevelRuntimeSectionSeed Mine;
  TArray<FLevelRuntimeLabelSeed> OverlayLabels;
};

struct FLevelBlockSpawn {
  FString Name;
  FVector Location = FVector::ZeroVector;
  FVector Scale = FVector::OneVector;
  ELevelRetroTexture Texture = ELevelRetroTexture::MarkerPaint;
};

struct FLevelLabelSpawn {
  FString Text;
  FVector Location = FVector::ZeroVector;
  float WorldSize = 42.0f;
};

struct FLevelRuntimeSectionSpawn {
  TArray<FLevelBlockSpawn> Blocks;
  TArray<FLevelLabelSpawn> Labels;
};

struct FLevelRuntimeBlockSpawnRequest {
  FLevelRuntimeBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelRuntimeLabelSpawnRequest {
  FLevelRuntimeLabelSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelRuntimeSectionSpawnRequest {
  FLevelRuntimeSectionSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelOverlaySectionSpawnRequest {
  FLevelRuntimeLayoutSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelNatureSectionSpawnRequest {
  TArray<FNatureFeatureSeed> Features;
  FLevelTerrainData TerrainData;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelLandmarkSectionSpawnRequest {
  TArray<FLandmark> Landmarks;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
};

struct FLevelWorldRouteRequest {
  TArray<FLevelLocalPoint> Route;
  FLevelTerrainData TerrainData;
};

struct FLevelSystemPayload {
  FString Id;
};

struct FLevelSystemState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  ForbocAI::Demo::Data::FLevelTerrainSourceSettings TerrainSources;
  ForbocAI::Demo::Data::FLevelDataSourceSettings DataSources;
  ForbocAI::Demo::Data::FLevelGeometrySettings Geometry;
  bool bReady = false;
};

inline bool operator==(const FLevelSystemPayload &Left,
                       const FLevelSystemPayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FLevelSystemPayload &Left,
                       const FLevelSystemPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelSystemState &Left,
                       const FLevelSystemState &Right) {
  return Left.bReady == Right.bReady &&
         Left.TerrainSources == Right.TerrainSources &&
         Left.DataSources == Right.DataSources &&
         Left.Geometry == Right.Geometry &&
         Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value);
}

inline bool operator!=(const FLevelSystemState &Left,
                       const FLevelSystemState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeScaleSeed &Left,
                       const FLevelRuntimeScaleSeed &Right) {
  return Left.Mode == Right.Mode &&
         FMath::IsNearlyEqual(Left.WidthFeet, Right.WidthFeet) &&
         FMath::IsNearlyEqual(Left.DepthFeet, Right.DepthFeet) &&
         FMath::IsNearlyEqual(Left.HeightFeet, Right.HeightFeet) &&
         FMath::IsNearlyEqual(Left.FrontageFeet, Right.FrontageFeet) &&
         FMath::IsNearlyEqual(Left.Stories, Right.Stories) &&
         FMath::IsNearlyEqual(Left.LengthLots, Right.LengthLots);
}

inline bool operator!=(const FLevelRuntimeScaleSeed &Left,
                       const FLevelRuntimeScaleSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeBlockSeed &Left,
                       const FLevelRuntimeBlockSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Anchor == Right.Anchor &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         Left.WorldLocation == Right.WorldLocation &&
         Left.Scale == Right.Scale && Left.Texture == Right.Texture;
}

inline bool operator!=(const FLevelRuntimeBlockSeed &Left,
                       const FLevelRuntimeBlockSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeLabelSeed &Left,
                       const FLevelRuntimeLabelSeed &Right) {
  return Left.Id == Right.Id && Left.Text == Right.Text &&
         Left.Anchor == Right.Anchor && Left.Height == Right.Height &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset) &&
         FMath::IsNearlyEqual(Left.WorldSizeScale, Right.WorldSizeScale) &&
         Left.ReferenceScale == Right.ReferenceScale;
}

inline bool operator!=(const FLevelRuntimeLabelSeed &Left,
                       const FLevelRuntimeLabelSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeSectionSeed &Left,
                       const FLevelRuntimeSectionSeed &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FLevelRuntimeSectionSeed &Left,
                       const FLevelRuntimeSectionSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeLayoutSeed &Left,
                       const FLevelRuntimeLayoutSeed &Right) {
  return Left.Terrain == Right.Terrain && Left.Town == Right.Town &&
         Left.Mine == Right.Mine && Left.OverlayLabels == Right.OverlayLabels;
}

inline bool operator!=(const FLevelRuntimeLayoutSeed &Left,
                       const FLevelRuntimeLayoutSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelBlockSpawn &Left,
                       const FLevelBlockSpawn &Right) {
  return Left.Name == Right.Name && Left.Location == Right.Location &&
         Left.Scale == Right.Scale && Left.Texture == Right.Texture;
}

inline bool operator!=(const FLevelBlockSpawn &Left,
                       const FLevelBlockSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelLabelSpawn &Left,
                       const FLevelLabelSpawn &Right) {
  return Left.Text == Right.Text && Left.Location == Right.Location &&
         FMath::IsNearlyEqual(Left.WorldSize, Right.WorldSize);
}

inline bool operator!=(const FLevelLabelSpawn &Left,
                       const FLevelLabelSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelRuntimeSectionSpawn &Left,
                       const FLevelRuntimeSectionSpawn &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FLevelRuntimeSectionSpawn &Left,
                       const FLevelRuntimeSectionSpawn &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Demo
} // namespace ForbocAI
