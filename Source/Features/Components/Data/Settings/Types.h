#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Bot/Types.h"
#include "Features/Components/Data/Settings/Dialogue/Types.h"
#include "Features/Components/Data/Settings/Interaction/Types.h"
#include "Features/Components/Data/Settings/Level/Types.h"
#include "Features/Components/Data/Settings/Player/Types.h"
#include "Features/Components/Data/Settings/Rendering/Distance/Lod/Types.h"
#include "Features/Components/Data/Settings/Rendering/Profile/Types.h"
#include "Features/Components/Data/Settings/Rendering/Texture/Types.h"
#include "Features/Components/Data/Settings/Speech/Types.h"
#include "Features/Components/Data/Settings/UI/Types.h"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FObservationIdSettings {
  FString PlayerPresentationRequested;
  FString PlayerMovementInputObserved;
  FString TownspersonPresentationRequested;
  FString HorsePresentationRequested;
  FString TownspersonCandidatesObserved;
};


struct FDebugMessageSettings {
  int32 OnScreenKey;
  float DurationSeconds;
  FColor Color;
};


struct FViewNameSettings {
  FString SceneRoot;
  FString TownspersonCharacterMesh;
  FString TownspersonInteractionSphere;
  FString TownspersonNameText;
  FString TownspersonPromptText;
  FString TownspersonDialogueText;
  FString TownspersonOverlapProfile;
  FString HorseImportedMesh;
  FString HorseMountedRiderMesh;
  FString HorseNameText;
  FString PlayerCameraBoom;
  FString PlayerFollowCamera;
  FString SpeechPresentationMesh;
  FString SpeechComponent;
};


struct FTextSettings {
  FString TownspersonNameRoleFormat;
  FString NpcReplyLog;
  FString TownspersonMissingMesh;
  FString TownspersonMissingAnimation;
  FString HorseMissingMesh;
  FString HorseMissingWalkAnimation;
  FString RiderMissingWalkAnimation;
  FString RiderMissingMesh;
  FString StartupSdkEnabled;
  FString StartupSdkDisabled;
};

struct FReduxLogSettings {
  int32 SampleInterval;
  TArray<FString> SampledActionTypes;
};

struct FEcsDomainRegistrationSettings {
  FString Path;
  int32 Kind;
};

struct FEcsSettings {
  TArray<FEcsDomainRegistrationSettings> DomainRegistry;
};

struct FStoreAutomationSettings {
  TArray<FString> DataBackedMapLabels;
  TArray<FString> ReduxLoggerMiddlewareLabels;
  TArray<FString> ProjectionGateLabels;
  FString ReduxLoggerCategory;
  FString ReduxLoggerActionTitlePrefix;
  FString TerrainEntity;
  FString TerrainProjectionDomain;
};

struct FContentAssetExpectationSettings {
  FString Label;
  FString Path;
};

struct FContentConfigExpectationSettings {
  FString Label;
  FString Section;
  FString Key;
  FString Expected;
};

struct FContentAssetsAutomationSettings {
  TArray<FContentAssetExpectationSettings> Packages;
  TArray<FContentAssetExpectationSettings> Classes;
  TArray<FContentAssetExpectationSettings> SpeechComponentClasses;
  TArray<FContentConfigExpectationSettings> ConfigValues;
  TArray<FContentAssetExpectationSettings> Assets;
  TArray<FContentAssetExpectationSettings> SkeletalMeshLods;
  TArray<FContentAssetExpectationSettings> MissingPackages;
  FString SkeletalMeshLoadsLabelFormat;
  FString SkeletalMeshLodDataLabelFormat;
  FString NativeLodAuditCountFormat;
  FString NativeLodAuditEntryFormat;
};

struct FAutomationSettings {
  FStoreAutomationSettings Store;
  FContentAssetsAutomationSettings ContentAssets;
};

struct FSettings {
  FPlayerPresentationSettings PlayerPresentation;
  FInteractionSettings Interaction;
  FTownspersonDefaultsSettings TownspersonDefaults;
  FLevelTerrainSourceSettings LevelTerrainSources;
  FLevelDataSourceSettings LevelDataSources;
  FLevelValidationSettings Validation;
  FLevelGeometrySettings LevelGeometry;
  FRenderingAssetPathSettings RenderingAssets;
  FRenderingProfileSettings RenderingProfile;
  TArray<FRenderingTextureSpecSettings> TextureCatalog;
  FRenderingSettings Rendering;
  FRenderingDistanceLodSettings RenderingDistanceLod;
  FDialogueSettings Dialogue;
  FBotSettings Bot;
  FUISettings UI;
  FSpeechSettings Speech;
  FTownspersonPresentationSettings TownspersonPresentation;
  FHorsePresentationSettings HorsePresentation;
  FObservationIdSettings ObservationIds;
  FDebugMessageSettings DebugMessages;
  FViewNameSettings ViewNames;
  FTextSettings Text;
  FReduxLogSettings ReduxLog;
  FEcsSettings Ecs;
  FAutomationSettings Automation;
};


inline bool operator==(const FObservationIdSettings &Left,
                       const FObservationIdSettings &Right) {
  return Left.PlayerPresentationRequested ==
             Right.PlayerPresentationRequested &&
         Left.PlayerMovementInputObserved ==
             Right.PlayerMovementInputObserved &&
         Left.TownspersonPresentationRequested ==
             Right.TownspersonPresentationRequested &&
         Left.HorsePresentationRequested == Right.HorsePresentationRequested &&
         Left.TownspersonCandidatesObserved ==
             Right.TownspersonCandidatesObserved;
}

inline bool operator!=(const FObservationIdSettings &Left,
                       const FObservationIdSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDebugMessageSettings &Left,
                       const FDebugMessageSettings &Right) {
  return Left.OnScreenKey == Right.OnScreenKey &&
         FMath::IsNearlyEqual(Left.DurationSeconds, Right.DurationSeconds) &&
         Left.Color == Right.Color;
}

inline bool operator!=(const FDebugMessageSettings &Left,
                       const FDebugMessageSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FViewNameSettings &Left,
                       const FViewNameSettings &Right) {
  return Left.SceneRoot == Right.SceneRoot &&
         Left.TownspersonCharacterMesh == Right.TownspersonCharacterMesh &&
         Left.TownspersonInteractionSphere ==
             Right.TownspersonInteractionSphere &&
         Left.TownspersonNameText == Right.TownspersonNameText &&
         Left.TownspersonPromptText == Right.TownspersonPromptText &&
         Left.TownspersonDialogueText == Right.TownspersonDialogueText &&
         Left.TownspersonOverlapProfile == Right.TownspersonOverlapProfile &&
         Left.HorseImportedMesh == Right.HorseImportedMesh &&
         Left.HorseMountedRiderMesh == Right.HorseMountedRiderMesh &&
         Left.HorseNameText == Right.HorseNameText &&
         Left.PlayerCameraBoom == Right.PlayerCameraBoom &&
         Left.PlayerFollowCamera == Right.PlayerFollowCamera &&
         Left.SpeechPresentationMesh == Right.SpeechPresentationMesh &&
         Left.SpeechComponent == Right.SpeechComponent;
}

inline bool operator!=(const FViewNameSettings &Left,
                       const FViewNameSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FTextSettings &Left,
                       const FTextSettings &Right) {
  return Left.TownspersonNameRoleFormat ==
             Right.TownspersonNameRoleFormat &&
         Left.NpcReplyLog == Right.NpcReplyLog &&
         Left.TownspersonMissingMesh == Right.TownspersonMissingMesh &&
         Left.TownspersonMissingAnimation ==
             Right.TownspersonMissingAnimation &&
         Left.HorseMissingMesh == Right.HorseMissingMesh &&
         Left.HorseMissingWalkAnimation == Right.HorseMissingWalkAnimation &&
         Left.RiderMissingWalkAnimation == Right.RiderMissingWalkAnimation &&
         Left.RiderMissingMesh == Right.RiderMissingMesh &&
         Left.StartupSdkEnabled == Right.StartupSdkEnabled &&
         Left.StartupSdkDisabled == Right.StartupSdkDisabled;
}

inline bool operator!=(const FTextSettings &Left,
                       const FTextSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FReduxLogSettings &Left,
                       const FReduxLogSettings &Right) {
  return Left.SampleInterval == Right.SampleInterval &&
         Left.SampledActionTypes == Right.SampledActionTypes;
}

inline bool operator!=(const FReduxLogSettings &Left,
                       const FReduxLogSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FEcsDomainRegistrationSettings &Left,
                       const FEcsDomainRegistrationSettings &Right) {
  return Left.Path == Right.Path && Left.Kind == Right.Kind;
}

inline bool operator!=(const FEcsDomainRegistrationSettings &Left,
                       const FEcsDomainRegistrationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FEcsSettings &Left,
                       const FEcsSettings &Right) {
  return Left.DomainRegistry == Right.DomainRegistry;
}

inline bool operator!=(const FEcsSettings &Left,
                       const FEcsSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FStoreAutomationSettings &Left,
                       const FStoreAutomationSettings &Right) {
  return Left.DataBackedMapLabels == Right.DataBackedMapLabels &&
         Left.ReduxLoggerMiddlewareLabels ==
             Right.ReduxLoggerMiddlewareLabels &&
         Left.ProjectionGateLabels == Right.ProjectionGateLabels &&
         Left.ReduxLoggerCategory == Right.ReduxLoggerCategory &&
         Left.ReduxLoggerActionTitlePrefix ==
             Right.ReduxLoggerActionTitlePrefix &&
         Left.TerrainEntity == Right.TerrainEntity &&
         Left.TerrainProjectionDomain == Right.TerrainProjectionDomain;
}

inline bool operator!=(const FStoreAutomationSettings &Left,
                       const FStoreAutomationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FContentAssetExpectationSettings &Left,
                       const FContentAssetExpectationSettings &Right) {
  return Left.Label == Right.Label && Left.Path == Right.Path;
}

inline bool operator!=(const FContentAssetExpectationSettings &Left,
                       const FContentAssetExpectationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FContentConfigExpectationSettings &Left,
                       const FContentConfigExpectationSettings &Right) {
  return Left.Label == Right.Label && Left.Section == Right.Section &&
         Left.Key == Right.Key && Left.Expected == Right.Expected;
}

inline bool operator!=(const FContentConfigExpectationSettings &Left,
                       const FContentConfigExpectationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FContentAssetsAutomationSettings &Left,
                       const FContentAssetsAutomationSettings &Right) {
  return Left.Packages == Right.Packages &&
         Left.Classes == Right.Classes &&
         Left.SpeechComponentClasses == Right.SpeechComponentClasses &&
         Left.ConfigValues == Right.ConfigValues &&
         Left.Assets == Right.Assets &&
         Left.SkeletalMeshLods == Right.SkeletalMeshLods &&
         Left.MissingPackages == Right.MissingPackages &&
         Left.SkeletalMeshLoadsLabelFormat ==
             Right.SkeletalMeshLoadsLabelFormat &&
         Left.SkeletalMeshLodDataLabelFormat ==
             Right.SkeletalMeshLodDataLabelFormat &&
         Left.NativeLodAuditCountFormat == Right.NativeLodAuditCountFormat &&
         Left.NativeLodAuditEntryFormat == Right.NativeLodAuditEntryFormat;
}

inline bool operator!=(const FContentAssetsAutomationSettings &Left,
                       const FContentAssetsAutomationSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return Left.Store == Right.Store &&
         Left.ContentAssets == Right.ContentAssets;
}

inline bool operator!=(const FAutomationSettings &Left,
                       const FAutomationSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
