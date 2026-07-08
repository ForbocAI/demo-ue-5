#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Level/Types.h"
#include "Features/Components/Rendering/Distance/Lod/Types.h"
#include "Features/Components/Rendering/Texture/Types.h"
#include "Features/Components/Rendering/Profile/Types.h"
#include "Features/Components/Spatial/Level/Layout/Types.h"
#include "Features/Systems/Landmarks/Types.h"
#include "Features/Systems/Nature/Types.h"

namespace ForbocAI {
namespace Game {
namespace Level {

enum class ELevelScaleMode {
  Building,
  LongFeature,
  Pad
};

enum class ELevelAnchorMode {
  BuildingLots,
  FeatureLots,
  PostOfficeLots,
  World
};

enum class ELevelLabelHeightMode {
  Explicit,
  LabelForScale,
  AboveBlock
};

struct FLevelScaleSeed {
  ELevelScaleMode Mode;
  float WidthFeet;
  float DepthFeet;
  float HeightFeet;
  float FrontageFeet;
  float Stories;
  float LengthLots;
};

struct FLevelBlockSeed {
  FString Id;
  FString Name;
  ELevelAnchorMode Anchor;
  float EastLots;
  float NorthLots;
  FVector WorldLocation;
  float YawDegrees;
  FLevelScaleSeed Scale;
  ELevelRetroTexture Texture;
};

struct FLevelLabelSeed {
  FString Id;
  FString Text;
  ELevelAnchorMode Anchor;
  ELevelLabelHeightMode Height;
  float EastLots;
  float NorthLots;
  float HeightOffset;
  float WorldSizeScale;
  FLevelScaleSeed ReferenceScale;
};

struct FLevelSectionSeed {
  TArray<FLevelBlockSeed> Blocks;
  TArray<FLevelLabelSeed> Labels;
};

struct FLevelLayoutSeed {
  FLevelSectionSeed Terrain;
  FLevelSectionSeed Town;
  FLevelSectionSeed Mine;
  TArray<FLevelLabelSeed> OverlayLabels;
};

struct FLevelBlockSpawn {
  FString Name;
  FVector Location = FVector::ZeroVector;
  FRotator Rotation = FRotator::ZeroRotator;
  FVector Scale = FVector::OneVector;
  ELevelRetroTexture Texture = ELevelRetroTexture::MarkerPaint;
  FLevelDistanceLodStage Lod;
};

struct FLevelLabelSpawn {
  FString Text;
  FVector Location = FVector::ZeroVector;
  float WorldSize = 42.0f;
  FLevelDistanceLodStage Lod;
};

struct FLevelSectionSpawn {
  TArray<FLevelBlockSpawn> Blocks;
  TArray<FLevelLabelSpawn> Labels;
};

struct FLevelBlockSpawnRequest {
  FLevelBlockSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelLabelSpawnRequest {
  FLevelLabelSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelSectionSpawnRequest {
  FLevelSectionSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelOverlaySectionSpawnRequest {
  FLevelLayoutSeed Seed;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelNatureSectionSpawnRequest {
  TArray<FNatureFeatureSeed> Features;
  FLevelTerrainData TerrainData;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelLandmarkSectionSpawnRequest {
  TArray<FLandmark> Landmarks;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FLevelWorldRouteRequest {
  TArray<FLevelLocalPoint> Route;
  FLevelTerrainData TerrainData;
};

struct FLevelSystemPayload {
  FString Id;
};

struct FLevelSystemState {
  func::Maybe<FString> ActionId = func::nothing<FString>();
  ForbocAI::Game::Data::FLevelTerrainSourceSettings TerrainSources;
  ForbocAI::Game::Data::FLevelDataSourceSettings DataSources;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
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
         Left.ActionId.hasValue == Right.ActionId.hasValue &&
         (!Left.ActionId.hasValue ||
          Left.ActionId.value == Right.ActionId.value);
}

inline bool operator!=(const FLevelSystemState &Left,
                       const FLevelSystemState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelScaleSeed &Left,
                       const FLevelScaleSeed &Right) {
  return Left.Mode == Right.Mode &&
         FMath::IsNearlyEqual(Left.WidthFeet, Right.WidthFeet) &&
         FMath::IsNearlyEqual(Left.DepthFeet, Right.DepthFeet) &&
         FMath::IsNearlyEqual(Left.HeightFeet, Right.HeightFeet) &&
         FMath::IsNearlyEqual(Left.FrontageFeet, Right.FrontageFeet) &&
         FMath::IsNearlyEqual(Left.Stories, Right.Stories) &&
         FMath::IsNearlyEqual(Left.LengthLots, Right.LengthLots);
}

inline bool operator!=(const FLevelScaleSeed &Left,
                       const FLevelScaleSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelBlockSeed &Left,
                       const FLevelBlockSeed &Right) {
  return Left.Id == Right.Id && Left.Name == Right.Name &&
         Left.Anchor == Right.Anchor &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         Left.WorldLocation == Right.WorldLocation &&
         FMath::IsNearlyEqual(Left.YawDegrees, Right.YawDegrees) &&
         Left.Scale == Right.Scale && Left.Texture == Right.Texture;
}

inline bool operator!=(const FLevelBlockSeed &Left,
                       const FLevelBlockSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelLabelSeed &Left,
                       const FLevelLabelSeed &Right) {
  return Left.Id == Right.Id && Left.Text == Right.Text &&
         Left.Anchor == Right.Anchor && Left.Height == Right.Height &&
         FMath::IsNearlyEqual(Left.EastLots, Right.EastLots) &&
         FMath::IsNearlyEqual(Left.NorthLots, Right.NorthLots) &&
         FMath::IsNearlyEqual(Left.HeightOffset, Right.HeightOffset) &&
         FMath::IsNearlyEqual(Left.WorldSizeScale, Right.WorldSizeScale) &&
         Left.ReferenceScale == Right.ReferenceScale;
}

inline bool operator!=(const FLevelLabelSeed &Left,
                       const FLevelLabelSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelSectionSeed &Left,
                       const FLevelSectionSeed &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FLevelSectionSeed &Left,
                       const FLevelSectionSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelLayoutSeed &Left,
                       const FLevelLayoutSeed &Right) {
  return Left.Terrain == Right.Terrain && Left.Town == Right.Town &&
         Left.Mine == Right.Mine && Left.OverlayLabels == Right.OverlayLabels;
}

inline bool operator!=(const FLevelLayoutSeed &Left,
                       const FLevelLayoutSeed &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelBlockSpawn &Left,
                       const FLevelBlockSpawn &Right) {
  return Left.Name == Right.Name && Left.Location == Right.Location &&
         Left.Rotation.Equals(Right.Rotation) && Left.Scale == Right.Scale &&
         Left.Texture == Right.Texture && Left.Lod == Right.Lod;
}

inline bool operator!=(const FLevelBlockSpawn &Left,
                       const FLevelBlockSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelLabelSpawn &Left,
                       const FLevelLabelSpawn &Right) {
  return Left.Text == Right.Text && Left.Location == Right.Location &&
         FMath::IsNearlyEqual(Left.WorldSize, Right.WorldSize) &&
         Left.Lod == Right.Lod;
}

inline bool operator!=(const FLevelLabelSpawn &Left,
                       const FLevelLabelSpawn &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelSectionSpawn &Left,
                       const FLevelSectionSpawn &Right) {
  return Left.Blocks == Right.Blocks && Left.Labels == Right.Labels;
}

inline bool operator!=(const FLevelSectionSpawn &Left,
                       const FLevelSectionSpawn &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
