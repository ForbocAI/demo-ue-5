#pragma once

#include "Core/rtk.hpp"
#include "Features/Components/Data/Settings/Automation/AutomationTypes.h"
#include "Features/Components/Data/Settings/Bot/Identity/IdentityTypes.h"
#include "Features/Components/Data/Settings/Bot/Horse/HorseTypes.h"
#include "Features/Components/Data/Settings/Bot/Configuration/ConfigurationTypes.h"
#include "Features/Components/Data/Settings/Bot/Townsperson/TownspersonTypes.h"
#include "Features/Components/Data/Settings/Dialogue/SettingsDialogueTypes.h"
#include "Features/Components/Data/Settings/Ecs/EcsTypes.h"
#include "Features/Components/Data/Settings/Interaction/SettingsInteractionTypes.h"
#include "Features/Components/Data/Settings/Level/Csv/CsvTypes.h"
#include "Features/Components/Data/Settings/Level/Geometry/GeometryTypes.h"
#include "Features/Components/Data/Settings/Level/Layout/SettingsLevelLayoutTypes.h"
#include "Features/Components/Data/Settings/Level/Terrain/LevelTerrainTypes.h"
#include "Features/Components/Data/Settings/Level/Validation/ValidationTypes.h"
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


struct FSceneViewNameSettings {
  FString Root;
};

struct FTownspersonViewNameSettings {
  FString CharacterMesh;
  FString InteractionSphere;
  FString NameText;
  FString PromptText;
  FString DialogueText;
  FString OverlapProfile;
};

struct FHorseViewNameSettings {
  FString ImportedMesh;
  FString MountedRiderMesh;
  FString NameText;
};

struct FPlayerViewNameSettings {
  FString CameraBoom;
  FString FollowCamera;
};

struct FSpeechViewNameSettings {
  FString PresentationMesh;
  FString Component;
};

struct FViewNameSettings {
  FSceneViewNameSettings Scene;
  FTownspersonViewNameSettings Townsperson;
  FHorseViewNameSettings Horse;
  FPlayerViewNameSettings Player;
  FSpeechViewNameSettings Speech;
};

struct FTownspersonTextSettings {
  FString NameRoleFormat;
  FString NpcReplyLog;
  FString MissingMesh;
  FString MissingAnimation;
};

struct FHorseTextSettings {
  FString MissingMesh;
  FString MissingWalkAnimation;
};

struct FRiderTextSettings {
  FString MissingWalkAnimation;
  FString MissingMesh;
};

struct FStartupTextSettings {
  FString SdkEnabled;
  FString SdkDisabled;
};

struct FTextSettings {
  FTownspersonTextSettings Townsperson;
  FHorseTextSettings Horse;
  FRiderTextSettings Rider;
  FStartupTextSettings Startup;
};

struct FReduxLogSettings {
  int32 SampleInterval;
  TArray<FString> SampledActionTypes;
};

struct FSettings {
  FPresentationSettings PlayerPresentation;
  FInteractionSettings Interaction;
  FTownspersonDefaultsSettings TownspersonDefaults;
  FSourceSettings LevelTerrainSources;
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


/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FObservationIdSettings &Left, const FObservationIdSettings &Right) */
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
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FObservationIdSettings &Left, const FObservationIdSettings &Right) */
inline bool operator!=(const FObservationIdSettings &Left,
                       const FObservationIdSettings &Right) {
  return !(Left == Right);
}
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FDebugMessageSettings &Left, const FDebugMessageSettings &Right) */
inline bool operator==(const FDebugMessageSettings &Left,
                       const FDebugMessageSettings &Right) {
  return Left.OnScreenKey == Right.OnScreenKey &&
         FMath::IsNearlyEqual(Left.DurationSeconds, Right.DurationSeconds) &&
         Left.Color == Right.Color;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FDebugMessageSettings &Left, const FDebugMessageSettings &Right) */
inline bool operator!=(const FDebugMessageSettings &Left,
                       const FDebugMessageSettings &Right) {
  return !(Left == Right);
}
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FSceneViewNameSettings &Left, const FSceneViewNameSettings &Right) */
inline bool operator==(const FSceneViewNameSettings &Left,
                       const FSceneViewNameSettings &Right) {
  return Left.Root == Right.Root;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FSceneViewNameSettings &Left, const FSceneViewNameSettings &Right) */
inline bool operator!=(const FSceneViewNameSettings &Left,
                       const FSceneViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonViewNameSettings &Left, const FTownspersonViewNameSettings &Right) */
inline bool operator==(const FTownspersonViewNameSettings &Left,
                       const FTownspersonViewNameSettings &Right) {
  return Left.CharacterMesh == Right.CharacterMesh &&
         Left.InteractionSphere == Right.InteractionSphere &&
         Left.NameText == Right.NameText &&
         Left.PromptText == Right.PromptText &&
         Left.DialogueText == Right.DialogueText &&
         Left.OverlapProfile == Right.OverlapProfile;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonViewNameSettings &Left, const FTownspersonViewNameSettings &Right) */
inline bool operator!=(const FTownspersonViewNameSettings &Left,
                       const FTownspersonViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FHorseViewNameSettings &Left, const FHorseViewNameSettings &Right) */
inline bool operator==(const FHorseViewNameSettings &Left,
                       const FHorseViewNameSettings &Right) {
  return Left.ImportedMesh == Right.ImportedMesh &&
         Left.MountedRiderMesh == Right.MountedRiderMesh &&
         Left.NameText == Right.NameText;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FHorseViewNameSettings &Left, const FHorseViewNameSettings &Right) */
inline bool operator!=(const FHorseViewNameSettings &Left,
                       const FHorseViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FPlayerViewNameSettings &Left, const FPlayerViewNameSettings &Right) */
inline bool operator==(const FPlayerViewNameSettings &Left,
                       const FPlayerViewNameSettings &Right) {
  return Left.CameraBoom == Right.CameraBoom &&
         Left.FollowCamera == Right.FollowCamera;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FPlayerViewNameSettings &Left, const FPlayerViewNameSettings &Right) */
inline bool operator!=(const FPlayerViewNameSettings &Left,
                       const FPlayerViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FSpeechViewNameSettings &Left, const FSpeechViewNameSettings &Right) */
inline bool operator==(const FSpeechViewNameSettings &Left,
                       const FSpeechViewNameSettings &Right) {
  return Left.PresentationMesh == Right.PresentationMesh &&
         Left.Component == Right.Component;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FSpeechViewNameSettings &Left, const FSpeechViewNameSettings &Right) */
inline bool operator!=(const FSpeechViewNameSettings &Left,
                       const FSpeechViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FViewNameSettings &Left, const FViewNameSettings &Right) */
inline bool operator==(const FViewNameSettings &Left,
                       const FViewNameSettings &Right) {
  return Left.Scene == Right.Scene && Left.Townsperson == Right.Townsperson &&
         Left.Horse == Right.Horse && Left.Player == Right.Player &&
         Left.Speech == Right.Speech;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FViewNameSettings &Left, const FViewNameSettings &Right) */
inline bool operator!=(const FViewNameSettings &Left,
                       const FViewNameSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FTownspersonTextSettings &Left, const FTownspersonTextSettings &Right) */
inline bool operator==(const FTownspersonTextSettings &Left,
                       const FTownspersonTextSettings &Right) {
  return Left.NameRoleFormat == Right.NameRoleFormat &&
         Left.NpcReplyLog == Right.NpcReplyLog &&
         Left.MissingMesh == Right.MissingMesh &&
         Left.MissingAnimation == Right.MissingAnimation;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FTownspersonTextSettings &Left, const FTownspersonTextSettings &Right) */
inline bool operator!=(const FTownspersonTextSettings &Left,
                       const FTownspersonTextSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FHorseTextSettings &Left, const FHorseTextSettings &Right) */
inline bool operator==(const FHorseTextSettings &Left,
                       const FHorseTextSettings &Right) {
  return Left.MissingMesh == Right.MissingMesh &&
         Left.MissingWalkAnimation == Right.MissingWalkAnimation;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FHorseTextSettings &Left, const FHorseTextSettings &Right) */
inline bool operator!=(const FHorseTextSettings &Left,
                       const FHorseTextSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FRiderTextSettings &Left, const FRiderTextSettings &Right) */
inline bool operator==(const FRiderTextSettings &Left,
                       const FRiderTextSettings &Right) {
  return Left.MissingWalkAnimation == Right.MissingWalkAnimation &&
         Left.MissingMesh == Right.MissingMesh;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FRiderTextSettings &Left, const FRiderTextSettings &Right) */
inline bool operator!=(const FRiderTextSettings &Left,
                       const FRiderTextSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FStartupTextSettings &Left, const FStartupTextSettings &Right) */
inline bool operator==(const FStartupTextSettings &Left,
                       const FStartupTextSettings &Right) {
  return Left.SdkEnabled == Right.SdkEnabled &&
         Left.SdkDisabled == Right.SdkDisabled;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FStartupTextSettings &Left, const FStartupTextSettings &Right) */
inline bool operator!=(const FStartupTextSettings &Left,
                       const FStartupTextSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FTextSettings &Left, const FTextSettings &Right) */
inline bool operator==(const FTextSettings &Left,
                       const FTextSettings &Right) {
  return Left.Townsperson == Right.Townsperson && Left.Horse == Right.Horse &&
         Left.Rider == Right.Rider && Left.Startup == Right.Startup;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FTextSettings &Left, const FTextSettings &Right) */
inline bool operator!=(const FTextSettings &Left,
                       const FTextSettings &Right) { return !(Left == Right); }
/** User Story: As a components data settings consumer, I need to compare values for equality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator==(const FReduxLogSettings &Left, const FReduxLogSettings &Right) */
inline bool operator==(const FReduxLogSettings &Left,
                       const FReduxLogSettings &Right) {
  return Left.SampleInterval == Right.SampleInterval &&
         Left.SampledActionTypes == Right.SampledActionTypes;
}
/** User Story: As a components data settings consumer, I need to compare values for inequality through a stable signature so the components data settings workflow remains explicit and composable. @fn inline bool operator!=(const FReduxLogSettings &Left, const FReduxLogSettings &Right) */
inline bool operator!=(const FReduxLogSettings &Left,
                       const FReduxLogSettings &Right) {
  return !(Left == Right);
}
} // namespace Data
} // namespace Game
} // namespace ForbocAI
