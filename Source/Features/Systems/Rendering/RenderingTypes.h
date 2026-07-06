#pragma once

#include "Core/rtk.hpp"

#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Rendering/RenderingTypes.h"

class UMaterialInterface;
class UStaticMeshComponent;
class UWorld;

namespace ForbocAI {
namespace Game {
namespace Level {

struct FLevelRetroTextureApply {
  UStaticMeshComponent *Part;
  UMaterialInterface *BaseMaterial;
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RuntimeSettings;
};

struct FRenderingPayload {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RuntimeSettings;
};

struct FRenderingPayloadRequest {
  FString Id;
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RuntimeSettings;
};

struct FRuntimeMemoryStats {
  int64 UsedPhysicalMegabytes;
  int64 PeakUsedPhysicalMegabytes;
  int64 UsedVirtualMegabytes;
};

struct FRuntimeFrameTimingStats {
  double GameThreadMilliseconds;
  double RenderThreadMilliseconds;
  double RhiThreadMilliseconds;
  double GpuMilliseconds;
  int32 DrawCalls;
  int32 RhiPrimitives;
};

struct FRuntimeFramePacingStats {
  double WallDeltaMilliseconds;
  double InputDeltaMilliseconds;
  double StatsSelectionMilliseconds;
  double PolyCountMilliseconds;
  double EngineIdleMilliseconds;
  double EngineIdleOvershootMilliseconds;
  float MaxFps;
  float FrameRateLimit;
  float EffectiveMaxTickRate;
  int32 FixedFrameRateEnabled;
  float FixedFrameRate;
  int32 FixedTimeStepEnabled;
  double FixedDeltaMilliseconds;
  int32 VsyncEnabled;
  int32 IdleWhenNotForegroundEnabled;
  int32 AppHasFocus;
  int32 CpuThrottleEnabled;
  int32 AllWindowsHidden;
};

struct FRuntimePolyCountStats {
  int64 PolyCount;
  double MeasurementMilliseconds;
};

struct FRuntimeStatsViewModel {
  int32 FramesPerSecond;
  int32 StackDepth;
  int64 PolyCount;
  int64 UsedPhysicalMemoryMegabytes;
  int64 PeakPhysicalMemoryMegabytes;
  int64 UsedVirtualMemoryMegabytes;
  double GameThreadMilliseconds;
  double RenderThreadMilliseconds;
  double RhiThreadMilliseconds;
  double GpuMilliseconds;
  int32 DrawCalls;
  int32 RhiPrimitives;
  double WallDeltaMilliseconds;
  double InputDeltaMilliseconds;
  double StatsSelectionMilliseconds;
  double PolyCountMilliseconds;
  double EngineIdleMilliseconds;
  double EngineIdleOvershootMilliseconds;
  float MaxFps;
  float FrameRateLimit;
  float EffectiveMaxTickRate;
  int32 FixedFrameRateEnabled;
  float FixedFrameRate;
  int32 FixedTimeStepEnabled;
  double FixedDeltaMilliseconds;
  int32 VsyncEnabled;
  int32 IdleWhenNotForegroundEnabled;
  int32 AppHasFocus;
  int32 CpuThrottleEnabled;
  int32 AllWindowsHidden;
};

struct FRenderingPresentationRequest {
  FString Id;
};

struct FRenderingTextureSpecRequest {
  ELevelRetroTexture Texture;
  TArray<FLevelRetroTextureSpec> Catalog;
};

struct FRenderingAssetPaths {
  FString LevelCubeMeshPath;
  FString BlockoutMaterialPath;
};

struct FTownspersonPresentationReduceRequest {
  ForbocAI::Game::Data::FTownspersonPresentationSettings Settings;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FHorsePresentationReduceRequest {
  ForbocAI::Game::Data::FHorsePresentationSettings Settings;
  ForbocAI::Game::Data::FLevelGeometrySettings Geometry;
};

struct FTownspersonPresentationViewModel {
  float WalkSpeed = 0.0f;
  float PauseDuration = 0.0f;
  float PatrolArrivalDistance = 0.0f;
  FVector MannequinOffset = FVector::ZeroVector;
  FRotator MannequinRotation = FRotator::ZeroRotator;
  FVector MannequinScale = FVector::OneVector;
  float InteractionRadius = 0.0f;
  FVector NameTextLocation = FVector::ZeroVector;
  float NameTextWorldSize = 0.0f;
  FVector PromptTextLocation = FVector::ZeroVector;
  float PromptTextWorldSize = 0.0f;
  FString InteractionPrompt;
  FVector DialogueTextLocation = FVector::ZeroVector;
  float DialogueTextWorldSize = 0.0f;
  FString MeshPath;
  FString AnimationBlueprintClassPath;
};

struct FHorsePresentationViewModel {
  FString DefaultName;
  float WalkSpeed = 0.0f;
  float PauseDuration = 0.0f;
  float PatrolArrivalDistance = 0.0f;
  FVector ImportedHorseScale = FVector::OneVector;
  FVector MountedRiderLocation = FVector::ZeroVector;
  FVector MountedRiderScale = FVector::OneVector;
  FVector NameTextLocation = FVector::ZeroVector;
  float NameTextWorldSize = 0.0f;
  FString HorseMeshPath;
  FString HorseWalkAnimationPath;
  FString RiderMeshPath;
  FString RiderWalkAnimationPath;
};

struct FRenderingState {
  func::Maybe<FString> LastActionId = func::nothing<FString>();
  FLevelRetroRenderProfile RuntimeProfile;
  TArray<FLevelRetroTextureSpec> TextureCatalog;
  TArray<FLevelDistanceLodStage> DistanceLodStages;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RuntimeSettings;
  FRenderingAssetPaths AssetPaths;
  FTownspersonPresentationViewModel TownspersonPresentation;
  FHorsePresentationViewModel HorsePresentation;
  bool bReady = false;
};

inline bool operator==(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.RuntimeSettings == Right.RuntimeSettings;
}

inline bool operator!=(const FRenderingPayload &Left,
                       const FRenderingPayload &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingPayloadRequest &Left,
                       const FRenderingPayloadRequest &Right) {
  return Left.Id == Right.Id && Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.RuntimeSettings == Right.RuntimeSettings;
}

inline bool operator!=(const FRenderingPayloadRequest &Left,
                       const FRenderingPayloadRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return Left.Id == Right.Id;
}

inline bool operator!=(const FRenderingPresentationRequest &Left,
                       const FRenderingPresentationRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureSpecRequest &Left,
                       const FRenderingTextureSpecRequest &Right) {
  return Left.Texture == Right.Texture && Left.Catalog == Right.Catalog;
}

inline bool operator!=(const FRenderingTextureSpecRequest &Left,
                       const FRenderingTextureSpecRequest &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return Left.LevelCubeMeshPath == Right.LevelCubeMeshPath &&
         Left.BlockoutMaterialPath == Right.BlockoutMaterialPath;
}

inline bool operator!=(const FRenderingAssetPaths &Left,
                       const FRenderingAssetPaths &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.PatrolArrivalDistance,
                              Right.PatrolArrivalDistance) &&
         Left.MannequinOffset == Right.MannequinOffset &&
         Left.MannequinRotation == Right.MannequinRotation &&
         Left.MannequinScale == Right.MannequinScale &&
         FMath::IsNearlyEqual(Left.InteractionRadius,
                              Right.InteractionRadius) &&
         Left.NameTextLocation == Right.NameTextLocation &&
         FMath::IsNearlyEqual(Left.NameTextWorldSize,
                              Right.NameTextWorldSize) &&
         Left.PromptTextLocation == Right.PromptTextLocation &&
         FMath::IsNearlyEqual(Left.PromptTextWorldSize,
                              Right.PromptTextWorldSize) &&
         Left.InteractionPrompt == Right.InteractionPrompt &&
         Left.DialogueTextLocation == Right.DialogueTextLocation &&
         FMath::IsNearlyEqual(Left.DialogueTextWorldSize,
                              Right.DialogueTextWorldSize) &&
         Left.MeshPath == Right.MeshPath &&
         Left.AnimationBlueprintClassPath ==
             Right.AnimationBlueprintClassPath;
}

inline bool operator!=(const FTownspersonPresentationViewModel &Left,
                       const FTownspersonPresentationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return Left.DefaultName == Right.DefaultName &&
         FMath::IsNearlyEqual(Left.WalkSpeed, Right.WalkSpeed) &&
         FMath::IsNearlyEqual(Left.PauseDuration, Right.PauseDuration) &&
         FMath::IsNearlyEqual(Left.PatrolArrivalDistance,
                              Right.PatrolArrivalDistance) &&
         Left.ImportedHorseScale == Right.ImportedHorseScale &&
         Left.MountedRiderLocation == Right.MountedRiderLocation &&
         Left.MountedRiderScale == Right.MountedRiderScale &&
         Left.NameTextLocation == Right.NameTextLocation &&
         FMath::IsNearlyEqual(Left.NameTextWorldSize,
                              Right.NameTextWorldSize) &&
         Left.HorseMeshPath == Right.HorseMeshPath &&
         Left.HorseWalkAnimationPath == Right.HorseWalkAnimationPath &&
         Left.RiderMeshPath == Right.RiderMeshPath &&
         Left.RiderWalkAnimationPath == Right.RiderWalkAnimationPath;
}

inline bool operator!=(const FHorsePresentationViewModel &Left,
                       const FHorsePresentationViewModel &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return Left.LastActionId.hasValue == Right.LastActionId.hasValue &&
         (!Left.LastActionId.hasValue ||
          Left.LastActionId.value == Right.LastActionId.value) &&
         Left.RuntimeProfile == Right.RuntimeProfile &&
         Left.TextureCatalog == Right.TextureCatalog &&
         Left.DistanceLodStages == Right.DistanceLodStages &&
         Left.RuntimeSettings == Right.RuntimeSettings &&
         Left.AssetPaths == Right.AssetPaths &&
         Left.TownspersonPresentation == Right.TownspersonPresentation &&
         Left.HorsePresentation == Right.HorsePresentation &&
         Left.bReady == Right.bReady;
}

inline bool operator!=(const FRenderingState &Left,
                       const FRenderingState &Right) {
  return !(Left == Right);
}

} // namespace Level
} // namespace Game
} // namespace ForbocAI
