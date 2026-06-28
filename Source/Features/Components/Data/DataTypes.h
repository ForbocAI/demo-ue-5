#pragma once

#include "Core/rtk.hpp"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FJsonContentObjectRequest {
  FString RelativePath;
};

struct FJsonFieldRequest {
  TSharedPtr<FJsonObject> Object;
  FString FieldName;
};

/**
 * @brief Identifies one JSON array value while preserving field context.
 *
 * @signature struct FJsonArrayValueObjectRequest
 *
 * User story: As a data adapter author, I can validate authored array entries
 * without a feature system owning neutral JSON error-shaping logic.
 */
struct FJsonArrayValueObjectRequest {
  TSharedPtr<FJsonValue> Value;
  FString FieldName;
  int32 Index = 0;
};

struct FPlayerPresentationSettings {
  float CapsuleRadius = 0.0f;
  float CapsuleHalfHeight = 0.0f;
  float FollowCameraArmLength = 0.0f;
  float RotationRateYaw = 0.0f;
  float JumpZVelocity = 0.0f;
  float AirControl = 0.0f;
  float MaxWalkSpeed = 0.0f;
  float MinAnalogWalkSpeed = 0.0f;
  float BrakingDecelerationWalking = 0.0f;
  float BrakingDecelerationFalling = 0.0f;
  FVector MeshRelativeLocation = FVector::ZeroVector;
  FRotator MeshRelativeRotation = FRotator::ZeroRotator;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
  FString MoveActionPath;
  FString LookActionPath;
  FString MouseLookActionPath;
  FString JumpActionPath;
  FString DefaultMappingContextPath;
  FString MouseMappingContextPath;
};

struct FInteractionSettings {
  float TownspersonMaxDistanceLots = 0.0f;
  FString NoTownspersonMessage;
};

struct FTownspersonDefaultsSettings {
  FString Id;
  FString Name;
  FString Role;
  FString Persona;
  FString InteractionPrompt;
  FString DefaultPlayerLine;
};

struct FLevelTerrainSourceSettings {
  FString TerrainCsvPath;
  FString OrthoCsvPath;
};

struct FLevelDataSourceSettings {
  FString RuntimeLayoutJsonPath;
  FString LandmarksJsonPath;
  FString TownspeopleJsonPath;
  FString HorsesJsonPath;
  FString NatureJsonPath;
};

struct FRuntimeValidationSettings {
  int32 TerrainGridSize = 0;
  int32 OrthoGridSize = 0;
  float TerrainMinReliefMeters = 0.0f;
};

struct FLevelGeometrySettings {
  float TerrainWorldSize = 0.0f;
  float TerrainElevationScale = 0.0f;
  float TerrainLotsAcross = 0.0f;
  float PostOfficeEastLots = 0.0f;
  float PostOfficeNorthLots = 0.0f;
  float CubeMeshSize = 0.0f;
  float BlockScalePerFoot = 0.0f;
  float HeightScalePerStory = 0.0f;
  float FoundationHeightRatio = 0.0f;
  float RoadClearanceRatio = 0.0f;
  float CharacterHeightRatio = 0.0f;
  float LabelClearanceRatio = 0.0f;
  float ActorFootToTerrainRatio = 0.0f;
  float ActorReferenceFeetAcross = 0.0f;
  float PlayerSpawnNorthLots = 0.0f;
  float PlayerSpawnExtraHeightRatio = 0.0f;
  float MainStreetFacingYawDegrees = 0.0f;
  FString PlayerSpawnAnchorLabel;
  float LandmarkLabelWorldSizeScale = 0.0f;
  float NatureLabelWorldSizeScale = 0.0f;
};

struct FRenderingAssetPathSettings {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
};

struct FRenderingProfileSettings {
  float TimeOfDayHour = 0.0f;
  int32 AntiAliasingMethod = 0;
  int32 PostProcessAAQuality = 0;
  float ScreenPercentage = 0.0f;
  float ViewDistanceScale = 0.0f;
  float FoliageDensityScale = 0.0f;
  float GrassDensityScale = 0.0f;
  float SunPitchDegrees = 0.0f;
  float SunYawDegrees = 0.0f;
  float DirectionalLightIntensity = 0.0f;
  float DirectionalLightSourceAngle = 0.0f;
  int32 ShadowCascades = 0;
  int32 ShadowMaxResolution = 0;
};

struct FRenderingTextureSpecSettings {
  FString Texture;
  FString Name;
  FString Use;
  int32 Size = 0;
};

struct FTownspersonPresentationSettings {
  float CharacterHeightFeet = 0.0f;
  float CharacterShoulderWidthFeet = 0.0f;
  float PatrolPauseSeconds = 0.0f;
  float PromptAboveHeadFeet = 0.0f;
  float NameAbovePromptFeet = 0.0f;
  float DialogueAboveNameHeightRatio = 0.0f;
  float InteractionRadiusLots = 0.0f;
  float WalkSpeedHeightRatio = 0.0f;
  float MannequinScale = 0.0f;
  FVector MannequinOffsetFeet = FVector::ZeroVector;
  FRotator MannequinRotation = FRotator::ZeroRotator;
  float PromptTextScale = 0.0f;
  float DialogueTextScale = 0.0f;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FHorsePresentationSettings {
  FString DefaultName;
  float HorseLengthFeet = 0.0f;
  float BodyHeightFeet = 0.0f;
  float LegHeightFeet = 0.0f;
  float NeckHeightFeet = 0.0f;
  float HeadHeightFeet = 0.0f;
  float SaddleHeightFeet = 0.0f;
  float PatrolPauseSeconds = 0.0f;
  float WalkSpeedHorseLengthRatio = 0.0f;
  float RouteArrivalLegRatio = 0.0f;
  float ImportedHorseScale = 0.0f;
  float MountedRiderScale = 0.0f;
  float NameTextWorldSizeFeet = 0.0f;
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};

struct FDemoRuntimeSettings {
  FPlayerPresentationSettings PlayerPresentation;
  FInteractionSettings Interaction;
  FTownspersonDefaultsSettings TownspersonDefaults;
  FLevelTerrainSourceSettings LevelTerrainSources;
  FLevelDataSourceSettings LevelDataSources;
  FRuntimeValidationSettings RuntimeValidation;
  FLevelGeometrySettings LevelGeometry;
  FRenderingAssetPathSettings RenderingAssets;
  FRenderingProfileSettings RenderingProfile;
  TArray<FRenderingTextureSpecSettings> TextureCatalog;
  FTownspersonPresentationSettings TownspersonPresentation;
  FHorsePresentationSettings HorsePresentation;
};

struct FDataLoadedPayload {
  FString RelativePath;
  bool bLoaded = false;
};

struct FDataState {
  func::Maybe<FString> LastPath = func::nothing<FString>();
  bool bLastLoadSucceeded = false;
  int32 LoadedObjectCount = 0;
};

inline bool operator==(const FJsonContentObjectRequest &Left,
                       const FJsonContentObjectRequest &Right) {
  return Left.RelativePath == Right.RelativePath;
}

inline bool operator!=(const FJsonContentObjectRequest &Left,
                       const FJsonContentObjectRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FJsonFieldRequest &Left,
                       const FJsonFieldRequest &Right) {
  return Left.Object == Right.Object && Left.FieldName == Right.FieldName;
}

inline bool operator!=(const FJsonFieldRequest &Left,
                       const FJsonFieldRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return Left.RelativePath == Right.RelativePath &&
         Left.bLoaded == Right.bLoaded;
}

inline bool operator!=(const FDataLoadedPayload &Left,
                       const FDataLoadedPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDataState &Left, const FDataState &Right) {
  return Left.LastPath.hasValue == Right.LastPath.hasValue &&
         (!Left.LastPath.hasValue ||
          Left.LastPath.value == Right.LastPath.value) &&
         Left.bLastLoadSucceeded == Right.bLastLoadSucceeded &&
         Left.LoadedObjectCount == Right.LoadedObjectCount;
}

inline bool operator!=(const FDataState &Left, const FDataState &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelTerrainSourceSettings &Left,
                       const FLevelTerrainSourceSettings &Right) {
  return Left.TerrainCsvPath == Right.TerrainCsvPath &&
         Left.OrthoCsvPath == Right.OrthoCsvPath;
}

inline bool operator!=(const FLevelTerrainSourceSettings &Left,
                       const FLevelTerrainSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelDataSourceSettings &Left,
                       const FLevelDataSourceSettings &Right) {
  return Left.RuntimeLayoutJsonPath == Right.RuntimeLayoutJsonPath &&
         Left.LandmarksJsonPath == Right.LandmarksJsonPath &&
         Left.TownspeopleJsonPath == Right.TownspeopleJsonPath &&
         Left.HorsesJsonPath == Right.HorsesJsonPath &&
         Left.NatureJsonPath == Right.NatureJsonPath;
}

inline bool operator!=(const FLevelDataSourceSettings &Left,
                       const FLevelDataSourceSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FLevelGeometrySettings &Left,
                       const FLevelGeometrySettings &Right) {
  return FMath::IsNearlyEqual(Left.TerrainWorldSize,
                              Right.TerrainWorldSize) &&
         FMath::IsNearlyEqual(Left.TerrainElevationScale,
                              Right.TerrainElevationScale) &&
         FMath::IsNearlyEqual(Left.TerrainLotsAcross,
                              Right.TerrainLotsAcross) &&
         FMath::IsNearlyEqual(Left.PostOfficeEastLots,
                              Right.PostOfficeEastLots) &&
         FMath::IsNearlyEqual(Left.PostOfficeNorthLots,
                              Right.PostOfficeNorthLots) &&
         FMath::IsNearlyEqual(Left.CubeMeshSize, Right.CubeMeshSize) &&
         FMath::IsNearlyEqual(Left.BlockScalePerFoot,
                              Right.BlockScalePerFoot) &&
         FMath::IsNearlyEqual(Left.HeightScalePerStory,
                              Right.HeightScalePerStory) &&
         FMath::IsNearlyEqual(Left.FoundationHeightRatio,
                              Right.FoundationHeightRatio) &&
         FMath::IsNearlyEqual(Left.RoadClearanceRatio,
                              Right.RoadClearanceRatio) &&
         FMath::IsNearlyEqual(Left.CharacterHeightRatio,
                              Right.CharacterHeightRatio) &&
         FMath::IsNearlyEqual(Left.LabelClearanceRatio,
                              Right.LabelClearanceRatio) &&
         FMath::IsNearlyEqual(Left.ActorFootToTerrainRatio,
                              Right.ActorFootToTerrainRatio) &&
         FMath::IsNearlyEqual(Left.ActorReferenceFeetAcross,
                              Right.ActorReferenceFeetAcross) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnNorthLots,
                              Right.PlayerSpawnNorthLots) &&
         FMath::IsNearlyEqual(Left.PlayerSpawnExtraHeightRatio,
                              Right.PlayerSpawnExtraHeightRatio) &&
         FMath::IsNearlyEqual(Left.MainStreetFacingYawDegrees,
                              Right.MainStreetFacingYawDegrees) &&
         Left.PlayerSpawnAnchorLabel == Right.PlayerSpawnAnchorLabel &&
         FMath::IsNearlyEqual(Left.LandmarkLabelWorldSizeScale,
                              Right.LandmarkLabelWorldSizeScale) &&
         FMath::IsNearlyEqual(Left.NatureLabelWorldSizeScale,
                              Right.NatureLabelWorldSizeScale);
}

inline bool operator!=(const FLevelGeometrySettings &Left,
                       const FLevelGeometrySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
