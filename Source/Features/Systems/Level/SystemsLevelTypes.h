#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/ComponentsLevelTypes.h"
#include "Features/Components/Rendering/Distance/Lod/ComponentsRenderingDistanceLodTypes.h"
#include "Features/Components/Rendering/Texture/ComponentsRenderingTextureTypes.h"
#include "Features/Components/Rendering/Profile/ComponentsRenderingProfileTypes.h"
#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutTypes.h"
#include "Features/Systems/Landmarks/SystemsLandmarksTypes.h"
#include "Features/Systems/Nature/SystemsNatureTypes.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class EScaleMode {
  Building,
  LongFeature,
  Pad
};

enum class EAnchorMode {
  BuildingLots,
  FeatureLots,
  PostOfficeLots,
  World
};

enum class ELabelHeightMode {
  Explicit,
  LabelForScale,
  AboveBlock
};

struct FScaleSeed {
  EScaleMode Mode;
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
  float FrontageFeet;
  float Stories;
  float LengthLots;
};

struct FBlockSeed {
  FString Id;
  FString Name;
  EAnchorMode Anchor;
  float EastLots;
  float NorthLots;
  FVector WorldLocation;
  float YawDegrees;
  FScaleSeed Scale;
  ELevelRetroTexture Texture;
};

struct FLabelSeed {
  FString Id;
  FString Text;
  EAnchorMode Anchor;
  ELabelHeightMode Height;
  float EastLots;
  float NorthLots;
  float HeightOffset;
  float WorldSizeScale;
  FScaleSeed ReferenceScale;
};

struct FSectionSeed {
  TArray<FBlockSeed> Blocks;
  TArray<FLabelSeed> Labels;
};

struct FLayoutSeed {
  FSectionSeed Terrain;
  FSectionSeed Town;
  FSectionSeed Mine;
  TArray<FLabelSeed> OverlayLabels;
};

struct FBlockSpawn {
  FString Name;
  FVector Location = FVector::ZeroVector;
  FRotator Rotation = FRotator::ZeroRotator;
  FVector Scale = FVector::OneVector;
  ELevelRetroTexture Texture = ELevelRetroTexture::MarkerPaint;
  FLevelDistanceLodStage Lod;
};

struct FLabelSpawn {
  FString Text;
  FVector Location = FVector::ZeroVector;
  float WorldSize = 42.0f;
  FLevelDistanceLodStage Lod;
};

struct FSectionSpawn {
  TArray<FBlockSpawn> Blocks;
  TArray<FLabelSpawn> Labels;
};

struct FBlockSpawnRequest {
  FBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FLabelSpawnRequest {
  FLabelSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FSectionSpawnRequest {
  FSectionSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FOverlaySectionSpawnRequest {
  FLayoutSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FNatureSectionSpawnRequest {
  TArray<FFeatureSeed> Features;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FLandmarkSectionSpawnRequest {
  TArray<FLandmark> Landmarks;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
};

struct FWorldRouteRequest {
  TArray<FLevelLocalPoint> Route;
  FLevelTerrainData TerrainData;
};

struct FSystemPayload {
  FString Id;
};

struct FSystemState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  ForbocAI::Game::Data::FTerrainSourceSettings TerrainSources;
  ForbocAI::Game::Data::FCsvSettings Csv;
  ForbocAI::Game::Data::FDataSourceSettings DataSources;
  ForbocAI::Game::Data::FGeometrySettings Geometry;
  bool bReady = false;
};

inline bool operator==(const FSystemPayload &Left,
                       const FSystemPayload &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FSystemPayload &Left,
                       const FSystemPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSystemState &Left,
                       const FSystemState &Right) {
  return Left.bReady == Right.bReady &&
         Left.TerrainSources == Right.TerrainSources &&
         Left.Csv == Right.Csv &&
         Left.DataSources == Right.DataSources &&
         Left.Geometry == Right.Geometry &&
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

inline bool operator!=(const FSystemState &Left,
                       const FSystemState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FScaleSeed &Left,
                       const FScaleSeed &Right) {
  return Left.Mode == Right.Mode &&
         FMath::IsNearlyEqual(Left.WidthFeet, Right.WidthFeet) &&
         FMath::IsNearlyEqual(Left.DepthFeet, Right.DepthFeet) &&
         FMath::IsNearlyEqual(Left.HeightFeet, Right.HeightFeet) &&
         FMath::IsNearlyEqual(Left.FrontageFeet, Right.FrontageFeet) &&
         FMath::IsNearlyEqual(Left.Stories, Right.Stories) &&
         FMath::IsNearlyEqual(Left.LengthLots, Right.LengthLots);
}

inline bool operator!=(const FScaleSeed &Left,
                       const FScaleSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBlockSeed &Left,
                       const FBlockSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Anchor == Right.Anchor &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         Left.WorldLocation == Right.WorldLocation &&
         FMath::IsNearlyEqual(Left.YawDegrees, Right.YawDegrees) &&
         Left.Scale == Right.Scale && Left.Texture == Right.Texture;
}

inline bool operator!=(const FBlockSeed &Left,
                       const FBlockSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLabelSeed &Left,
                       const FLabelSeed &Right) {
  return Left.Id == Right.Id && Left.Text == Right.Text &&
         Left.Anchor == Right.Anchor && Left.Height == Right.Height &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset) &&
         FMath::IsNearlyEqual(Left.WorldSizeScale, Right.WorldSizeScale) &&
         Left.ReferenceScale == Right.ReferenceScale;
}

inline bool operator!=(const FLabelSeed &Left,
                       const FLabelSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSectionSeed &Left,
                       const FSectionSeed &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FSectionSeed &Left,
                       const FSectionSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLayoutSeed &Left,
                       const FLayoutSeed &Right) {
  return Left.Terrain == Right.Terrain && Left.Town == Right.Town &&
         Left.Mine == Right.Mine && Left.OverlayLabels == Right.OverlayLabels;
}

inline bool operator!=(const FLayoutSeed &Left,
                       const FLayoutSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBlockSpawn &Left,
                       const FBlockSpawn &Right) {
  return Left.Name == Right.Name && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale &&
         Left.Texture == Right.Texture && Left.Lod == Right.Lod;
}

inline bool operator!=(const FBlockSpawn &Left,
                       const FBlockSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLabelSpawn &Left,
                       const FLabelSpawn &Right) {
  return Left.Text == Right.Text && Left.Location == Right.Location &&
         FMath::IsNearlyEqual(Left.WorldSize, Right.WorldSize) &&
         Left.Lod == Right.Lod;
}

inline bool operator!=(const FLabelSpawn &Left,
                       const FLabelSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSectionSpawn &Left,
                       const FSectionSpawn &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FSectionSpawn &Left,
                       const FSectionSpawn &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
