#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Demo {
namespace Data {

struct FRuntimeObservationIdSettings {
  FString PlayerPresentationRequested;
  FString PlayerMovementInputObserved;
  FString TownspersonPresentationRequested;
  FString HorsePresentationRequested;
  FString TownspersonCandidatesObserved;
};


struct FRuntimeDebugMessageSettings {
  int32 OnScreenKey;
  float DurationSeconds;
  FColor Color;
};


struct FRuntimeViewNameSettings {
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


struct FRuntimeTextSettings {
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


inline bool operator==(const FRuntimeObservationIdSettings &Left,
                       const FRuntimeObservationIdSettings &Right) {
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

inline bool operator!=(const FRuntimeObservationIdSettings &Left,
                       const FRuntimeObservationIdSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeDebugMessageSettings &Left,
                       const FRuntimeDebugMessageSettings &Right) {
  return Left.OnScreenKey == Right.OnScreenKey &&
         FMath::IsNearlyEqual(Left.DurationSeconds, Right.DurationSeconds) &&
         Left.Color == Right.Color;
}

inline bool operator!=(const FRuntimeDebugMessageSettings &Left,
                       const FRuntimeDebugMessageSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeViewNameSettings &Left,
                       const FRuntimeViewNameSettings &Right) {
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

inline bool operator!=(const FRuntimeViewNameSettings &Left,
                       const FRuntimeViewNameSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRuntimeTextSettings &Left,
                       const FRuntimeTextSettings &Right) {
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

inline bool operator!=(const FRuntimeTextSettings &Left,
                       const FRuntimeTextSettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Demo
} // namespace ForbocAI
