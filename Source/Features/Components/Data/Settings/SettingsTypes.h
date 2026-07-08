#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Automation/AutomationTypes.h"
#include "Features/Components/Data/Settings/Bot/SettingsBotTypes.h"
#include "Features/Components/Data/Settings/Dialogue/SettingsDialogueTypes.h"
#include "Features/Components/Data/Settings/Interaction/SettingsInteractionTypes.h"
#include "Features/Components/Data/Settings/Level/SettingsLevelTypes.h"
#include "Features/Components/Data/Settings/Player/SettingsPlayerTypes.h"
#include "Features/Components/Data/Settings/Rendering/Distance/Lod/SettingsRenderingDistanceLodTypes.h"
#include "Features/Components/Data/Settings/Rendering/Profile/SettingsRenderingProfileTypes.h"
#include "Features/Components/Data/Settings/Rendering/Texture/SettingsRenderingTextureTypes.h"
#include "Features/Components/Data/Settings/Speech/SettingsSpeechTypes.h"
#include "Features/Components/Data/Settings/UI/SettingsUITypes.h"

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

struct FSettings {
  FPresentationSettings PlayerPresentation;
  FInteractionSettings Interaction;
  FTownspersonDefaultsSettings TownspersonDefaults;
  FTerrainSourceSettings LevelTerrainSources;
  FCsvSettings LevelCsv;
  FDataSourceSettings LevelDataSources;
  FValidationSettings Validation;
  FGeometrySettings LevelGeometry;
  FRenderingAssetPathSettings RenderingAssets;
  FProfileSettings RenderingProfile;
  TArray<FSpecSettings> TextureCatalog;
  FRenderingSettings Rendering;
  FLodSettings RenderingDistanceLod;
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
  Automation::FSettings Automation;
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

} // namespace Data
} // namespace Game
} // namespace ForbocAI
