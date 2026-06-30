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

struct FRenderingRgbSettings {
  int32 R = 0;
  int32 G = 0;
  int32 B = 0;
};

struct FRenderingTextureHashSettings {
  int32 XMultiplier = 0;
  int32 YMultiplier = 0;
  int32 SaltMultiplier = 0;
  int32 XorShift = 0;
};

struct FRenderingConsoleVariableSettings {
  FString Name;
  FString ValueKind;
  FString ProfileField;
  int32 IntValue = 0;
  float FloatValue = 0.0f;
};

struct FRenderingTexturePredicateSettings {
  FString Kind;
  int32 XMultiplier = 0;
  int32 YMultiplier = 0;
  int32 NoiseMultiplier = 0;
  int32 XDivisor = 0;
  int32 YDivisor = 0;
  int32 Modulus = 0;
  int32 Equals = 0;
  int32 LessThan = 0;
};

struct FRenderingTextureColorResultSettings {
  FString Kind;
  FRenderingRgbSettings Color;
  FRenderingRgbSettings ColorA;
  FRenderingRgbSettings ColorB;
  int32 NumeratorBase = 0;
  int32 NumeratorNoiseModulus = 0;
  int32 Denominator = 0;
};

struct FRenderingTextureRuleSettings {
  FRenderingTexturePredicateSettings Predicate;
  FRenderingTextureColorResultSettings Result;
};

struct FRenderingTexturePaletteSettings {
  FString Texture;
  TArray<FRenderingTextureRuleSettings> Rules;
};

struct FRenderingRuntimeSettings {
  int32 TextureChannels = 0;
  int32 TextureAlpha = 0;
  FString TextureCacheKeyFormat;
  FString MaterialTextureParameter;
  FRenderingTextureHashSettings TextureHash;
  TArray<FRenderingConsoleVariableSettings> ConsoleVariables;
  TArray<FRenderingTexturePaletteSettings> TexturePalettes;
};

struct FUIRuntimeSettings {
  FString PlayerRoleLabel;
  FString SystemRoleLabel;
  FString NpcRoleLabel;
  FString UnknownRoleLabel;
  FString ChatMessageFormat;
  FString HistoryRoleSeparator;
  int32 HistoryTextStartOffset = 0;
  int32 HistoryMinimumRoleIndex = 0;
  FString PlaceholderTitle;
  FString PlaceholderPlayerLine;
  FString PlaceholderNpcReply;
  FString ConversationTitleFormat;
  FString PlayerLineFormat;
  FString NpcReplyFormat;
  FString ConnectionMessageFormat;
  FString UnboundDialogueError;
  FString PayloadIdFormat;
  FLinearColor PanelColor;
  FLinearColor TitleColor;
  FLinearColor PlayerColor;
  FLinearColor SystemColor;
  FLinearColor NpcColor;
  FLinearColor UnknownColor;
  FLinearColor RuntimeReplyColor;
  float PanelPadding = 0.0f;
  float TitleSize = 0.0f;
  float BodySize = 0.0f;
};

struct FDialogueRuntimeSettings {
  FString ReplyPayloadIdFormat;
};

struct FBotStatPresetSettings {
  float MoveSpeed;
  float AwarenessRange;
  float Resolve;
  bool bCanTalk;
};

struct FBotRuntimeSettings {
  FString InitialName;
  float InitialHealth;
  float InitialMaxHealth;
  float InitialMana;
  float InitialMaxMana;
  float InitialStamina;
  float InitialMaxStamina;
  float MinimumHealth;
  FVector InitialPosition;
  FRotator InitialRotation;
  FVector InitialLastKnownPlayerPosition;
  float InitialTimeSinceLastSeenPlayer;
  float EnemySpottedTimeSinceLastSeenPlayer;
  bool bInitialHasAggro;
  bool bDefaultHazardOverlapping;
  bool bDefaultVisibilityCanSeeEnemy;
  int32 InitialPhase;
  int32 InitialTickCount;
  float AggroTimeoutSeconds;
  float DamageFleeHealthRatio;
  float PhaseFleeHealthRatio;
  float MovementArrivalDistanceSquared;
  float AggroPositionToleranceSquared;
  float DefaultMovementInterpSpeed;
  int32 PatrolGoalPriority;
  FString PatrolGoalIdFormat;
  bool bPatrolGoalInitialCompleted;
  bool bActiveGoalComponentHasActiveGoal;
  FBotStatPresetSettings TownspersonStats;
  FBotStatPresetSettings HorseStats;
  float ObservationIntervalSeconds;
  float InitialObservationTimeSeconds;
  bool bOrchestratorCanEverTick;
  bool bRegisteredBotActive;
  bool bPositionPayloadHasLocalLocation;
  bool bPositionPayloadHasWorldLocation;
  FString StartLog;
  FString RegisteredLogFormat;
  FString ProcessFailedLogFormat;
  FString ExecuteLogFormat;
  FString NullActorLabel;
  FString MoveActionType;
  FString AttackActionType;
  FVector MoveActionOffset;
  FString StateObservationFormat;
  int32 DefaultBehaviorState;
};

struct FSpeechVisemeMappingSettings {
  FString Phoneme;
  FString MorphTargetName;
  float BlendWeight = 0.0f;
};

struct FSpeechPhonemeDurationRuleSettings {
  FString Kind;
  FString Phoneme;
  float Multiplier = 0.0f;
};

struct FSpeechRuntimeSettings {
  FString RestViseme;
  float RestWeight = 0.0f;
  float SpeechRate = 0.0f;
  float Volume = 0.0f;
  bool bEnableLipSync = false;
  bool bCanEverTick = false;
  bool bStartTickEnabled = false;
  float InitialPlaybackTime = 0.0f;
  bool bInitialSpeechActive = false;
  float EstimatedBasePhonemeSeconds = 0.0f;
  FString VowelCharacters;
  FString VowelPhonemeFormat;
  FString SilenceCharacters;
  FString SilencePhoneme;
  float VisemeChangeTolerance = 0.0f;
  FString TtsEndpoint;
  FString TtsVerb;
  FString TtsContentTypeHeader;
  FString TtsContentType;
  FString TtsRequestFormat;
  int32 TtsSuccessResponseCode = 0;
  int32 MinimumAudioBytes = 0;
  FString SpeechStartLogFormat;
  FString SpeechAudioReceivedLogFormat;
  TArray<FString> ResetMorphTargets;
  TArray<FSpeechVisemeMappingSettings> VisemeMappings;
  TArray<FSpeechPhonemeDurationRuleSettings> DurationRules;
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
  FRenderingRuntimeSettings RenderingRuntime;
  FDialogueRuntimeSettings DialogueRuntime;
  FBotRuntimeSettings BotRuntime;
  FUIRuntimeSettings UIRuntime;
  FSpeechRuntimeSettings SpeechRuntime;
  FTownspersonPresentationSettings TownspersonPresentation;
  FHorsePresentationSettings HorsePresentation;
  FRuntimeObservationIdSettings RuntimeObservationIds;
  FRuntimeDebugMessageSettings RuntimeDebugMessages;
  FRuntimeViewNameSettings RuntimeViewNames;
  FRuntimeTextSettings RuntimeText;
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

inline bool operator==(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return Left.R == Right.R && Left.G == Right.G && Left.B == Right.B;
}

inline bool operator!=(const FRenderingRgbSettings &Left,
                       const FRenderingRgbSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureHashSettings &Left,
                       const FRenderingTextureHashSettings &Right) {
  return Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.SaltMultiplier == Right.SaltMultiplier &&
         Left.XorShift == Right.XorShift;
}

inline bool operator!=(const FRenderingTextureHashSettings &Left,
                       const FRenderingTextureHashSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingConsoleVariableSettings &Left,
                       const FRenderingConsoleVariableSettings &Right) {
  return Left.Name == Right.Name && Left.ValueKind == Right.ValueKind &&
         Left.ProfileField == Right.ProfileField &&
         Left.IntValue == Right.IntValue &&
         FMath::IsNearlyEqual(Left.FloatValue, Right.FloatValue);
}

inline bool operator!=(const FRenderingConsoleVariableSettings &Left,
                       const FRenderingConsoleVariableSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTexturePredicateSettings &Left,
                       const FRenderingTexturePredicateSettings &Right) {
  return Left.Kind == Right.Kind && Left.XMultiplier == Right.XMultiplier &&
         Left.YMultiplier == Right.YMultiplier &&
         Left.NoiseMultiplier == Right.NoiseMultiplier &&
         Left.XDivisor == Right.XDivisor &&
         Left.YDivisor == Right.YDivisor && Left.Modulus == Right.Modulus &&
         Left.Equals == Right.Equals && Left.LessThan == Right.LessThan;
}

inline bool operator!=(const FRenderingTexturePredicateSettings &Left,
                       const FRenderingTexturePredicateSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureColorResultSettings &Left,
                       const FRenderingTextureColorResultSettings &Right) {
  return Left.Kind == Right.Kind && Left.Color == Right.Color &&
         Left.ColorA == Right.ColorA && Left.ColorB == Right.ColorB &&
         Left.NumeratorBase == Right.NumeratorBase &&
         Left.NumeratorNoiseModulus == Right.NumeratorNoiseModulus &&
         Left.Denominator == Right.Denominator;
}

inline bool operator!=(const FRenderingTextureColorResultSettings &Left,
                       const FRenderingTextureColorResultSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTextureRuleSettings &Left,
                       const FRenderingTextureRuleSettings &Right) {
  return Left.Predicate == Right.Predicate && Left.Result == Right.Result;
}

inline bool operator!=(const FRenderingTextureRuleSettings &Left,
                       const FRenderingTextureRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingTexturePaletteSettings &Left,
                       const FRenderingTexturePaletteSettings &Right) {
  return Left.Texture == Right.Texture && Left.Rules == Right.Rules;
}

inline bool operator!=(const FRenderingTexturePaletteSettings &Left,
                       const FRenderingTexturePaletteSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FRenderingRuntimeSettings &Left,
                       const FRenderingRuntimeSettings &Right) {
  return Left.TextureChannels == Right.TextureChannels &&
         Left.TextureAlpha == Right.TextureAlpha &&
         Left.TextureCacheKeyFormat == Right.TextureCacheKeyFormat &&
         Left.MaterialTextureParameter == Right.MaterialTextureParameter &&
         Left.TextureHash == Right.TextureHash &&
         Left.ConsoleVariables == Right.ConsoleVariables &&
         Left.TexturePalettes == Right.TexturePalettes;
}

inline bool operator!=(const FRenderingRuntimeSettings &Left,
                       const FRenderingRuntimeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FUIRuntimeSettings &Left,
                       const FUIRuntimeSettings &Right) {
  return Left.PlayerRoleLabel == Right.PlayerRoleLabel &&
         Left.SystemRoleLabel == Right.SystemRoleLabel &&
         Left.NpcRoleLabel == Right.NpcRoleLabel &&
         Left.UnknownRoleLabel == Right.UnknownRoleLabel &&
         Left.ChatMessageFormat == Right.ChatMessageFormat &&
         Left.HistoryRoleSeparator == Right.HistoryRoleSeparator &&
         Left.HistoryTextStartOffset == Right.HistoryTextStartOffset &&
         Left.HistoryMinimumRoleIndex == Right.HistoryMinimumRoleIndex &&
         Left.PlaceholderTitle == Right.PlaceholderTitle &&
         Left.PlaceholderPlayerLine == Right.PlaceholderPlayerLine &&
         Left.PlaceholderNpcReply == Right.PlaceholderNpcReply &&
         Left.ConversationTitleFormat == Right.ConversationTitleFormat &&
         Left.PlayerLineFormat == Right.PlayerLineFormat &&
         Left.NpcReplyFormat == Right.NpcReplyFormat &&
         Left.ConnectionMessageFormat == Right.ConnectionMessageFormat &&
         Left.UnboundDialogueError == Right.UnboundDialogueError &&
         Left.PayloadIdFormat == Right.PayloadIdFormat &&
         Left.PanelColor == Right.PanelColor &&
         Left.TitleColor == Right.TitleColor &&
         Left.PlayerColor == Right.PlayerColor &&
         Left.SystemColor == Right.SystemColor &&
         Left.NpcColor == Right.NpcColor &&
         Left.UnknownColor == Right.UnknownColor &&
         Left.RuntimeReplyColor == Right.RuntimeReplyColor &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.TitleSize, Right.TitleSize) &&
         FMath::IsNearlyEqual(Left.BodySize, Right.BodySize);
}

inline bool operator!=(const FUIRuntimeSettings &Left,
                       const FUIRuntimeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FDialogueRuntimeSettings &Left,
                       const FDialogueRuntimeSettings &Right) {
  return Left.ReplyPayloadIdFormat == Right.ReplyPayloadIdFormat;
}

inline bool operator!=(const FDialogueRuntimeSettings &Left,
                       const FDialogueRuntimeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotStatPresetSettings &Left,
                       const FBotStatPresetSettings &Right) {
  return FMath::IsNearlyEqual(Left.MoveSpeed, Right.MoveSpeed) &&
         FMath::IsNearlyEqual(Left.AwarenessRange, Right.AwarenessRange) &&
         FMath::IsNearlyEqual(Left.Resolve, Right.Resolve) &&
         Left.bCanTalk == Right.bCanTalk;
}

inline bool operator!=(const FBotStatPresetSettings &Left,
                       const FBotStatPresetSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FBotRuntimeSettings &Left,
                       const FBotRuntimeSettings &Right) {
  return Left.InitialName == Right.InitialName &&
         FMath::IsNearlyEqual(Left.InitialHealth, Right.InitialHealth) &&
         FMath::IsNearlyEqual(Left.InitialMaxHealth,
                              Right.InitialMaxHealth) &&
         FMath::IsNearlyEqual(Left.InitialMana, Right.InitialMana) &&
         FMath::IsNearlyEqual(Left.InitialMaxMana, Right.InitialMaxMana) &&
         FMath::IsNearlyEqual(Left.InitialStamina, Right.InitialStamina) &&
         FMath::IsNearlyEqual(Left.InitialMaxStamina,
                              Right.InitialMaxStamina) &&
         FMath::IsNearlyEqual(Left.MinimumHealth, Right.MinimumHealth) &&
         Left.InitialPosition == Right.InitialPosition &&
         Left.InitialRotation == Right.InitialRotation &&
         Left.InitialLastKnownPlayerPosition ==
             Right.InitialLastKnownPlayerPosition &&
         FMath::IsNearlyEqual(Left.InitialTimeSinceLastSeenPlayer,
                              Right.InitialTimeSinceLastSeenPlayer) &&
         FMath::IsNearlyEqual(Left.EnemySpottedTimeSinceLastSeenPlayer,
                              Right.EnemySpottedTimeSinceLastSeenPlayer) &&
         Left.bInitialHasAggro == Right.bInitialHasAggro &&
         Left.bDefaultHazardOverlapping ==
             Right.bDefaultHazardOverlapping &&
         Left.bDefaultVisibilityCanSeeEnemy ==
             Right.bDefaultVisibilityCanSeeEnemy &&
         Left.InitialPhase == Right.InitialPhase &&
         Left.InitialTickCount == Right.InitialTickCount &&
         FMath::IsNearlyEqual(Left.AggroTimeoutSeconds,
                              Right.AggroTimeoutSeconds) &&
         FMath::IsNearlyEqual(Left.DamageFleeHealthRatio,
                              Right.DamageFleeHealthRatio) &&
         FMath::IsNearlyEqual(Left.PhaseFleeHealthRatio,
                              Right.PhaseFleeHealthRatio) &&
         FMath::IsNearlyEqual(Left.MovementArrivalDistanceSquared,
                              Right.MovementArrivalDistanceSquared) &&
         FMath::IsNearlyEqual(Left.AggroPositionToleranceSquared,
                              Right.AggroPositionToleranceSquared) &&
         FMath::IsNearlyEqual(Left.DefaultMovementInterpSpeed,
                              Right.DefaultMovementInterpSpeed) &&
         Left.PatrolGoalPriority == Right.PatrolGoalPriority &&
         Left.PatrolGoalIdFormat == Right.PatrolGoalIdFormat &&
         Left.bPatrolGoalInitialCompleted ==
             Right.bPatrolGoalInitialCompleted &&
         Left.bActiveGoalComponentHasActiveGoal ==
             Right.bActiveGoalComponentHasActiveGoal &&
         Left.TownspersonStats == Right.TownspersonStats &&
         Left.HorseStats == Right.HorseStats &&
         FMath::IsNearlyEqual(Left.ObservationIntervalSeconds,
                              Right.ObservationIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.InitialObservationTimeSeconds,
                              Right.InitialObservationTimeSeconds) &&
         Left.bOrchestratorCanEverTick ==
             Right.bOrchestratorCanEverTick &&
         Left.bRegisteredBotActive == Right.bRegisteredBotActive &&
         Left.bPositionPayloadHasLocalLocation ==
             Right.bPositionPayloadHasLocalLocation &&
         Left.bPositionPayloadHasWorldLocation ==
             Right.bPositionPayloadHasWorldLocation &&
         Left.StartLog == Right.StartLog &&
         Left.RegisteredLogFormat == Right.RegisteredLogFormat &&
         Left.ProcessFailedLogFormat == Right.ProcessFailedLogFormat &&
         Left.ExecuteLogFormat == Right.ExecuteLogFormat &&
         Left.NullActorLabel == Right.NullActorLabel &&
         Left.MoveActionType == Right.MoveActionType &&
         Left.AttackActionType == Right.AttackActionType &&
         Left.MoveActionOffset == Right.MoveActionOffset &&
         Left.StateObservationFormat == Right.StateObservationFormat &&
         Left.DefaultBehaviorState == Right.DefaultBehaviorState;
}

inline bool operator!=(const FBotRuntimeSettings &Left,
                       const FBotRuntimeSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechVisemeMappingSettings &Left,
                       const FSpeechVisemeMappingSettings &Right) {
  return Left.Phoneme == Right.Phoneme &&
         Left.MorphTargetName == Right.MorphTargetName &&
         FMath::IsNearlyEqual(Left.BlendWeight, Right.BlendWeight);
}

inline bool operator!=(const FSpeechVisemeMappingSettings &Left,
                       const FSpeechVisemeMappingSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechPhonemeDurationRuleSettings &Left,
                       const FSpeechPhonemeDurationRuleSettings &Right) {
  return Left.Kind == Right.Kind && Left.Phoneme == Right.Phoneme &&
         FMath::IsNearlyEqual(Left.Multiplier, Right.Multiplier);
}

inline bool operator!=(const FSpeechPhonemeDurationRuleSettings &Left,
                       const FSpeechPhonemeDurationRuleSettings &Right) {
  return !(Left == Right);
}

inline bool operator==(const FSpeechRuntimeSettings &Left,
                       const FSpeechRuntimeSettings &Right) {
  return Left.RestViseme == Right.RestViseme &&
         FMath::IsNearlyEqual(Left.RestWeight, Right.RestWeight) &&
         FMath::IsNearlyEqual(Left.SpeechRate, Right.SpeechRate) &&
         FMath::IsNearlyEqual(Left.Volume, Right.Volume) &&
         Left.bEnableLipSync == Right.bEnableLipSync &&
         Left.bCanEverTick == Right.bCanEverTick &&
         Left.bStartTickEnabled == Right.bStartTickEnabled &&
         FMath::IsNearlyEqual(Left.InitialPlaybackTime,
                              Right.InitialPlaybackTime) &&
         Left.bInitialSpeechActive == Right.bInitialSpeechActive &&
         FMath::IsNearlyEqual(Left.EstimatedBasePhonemeSeconds,
                              Right.EstimatedBasePhonemeSeconds) &&
         Left.VowelCharacters == Right.VowelCharacters &&
         Left.VowelPhonemeFormat == Right.VowelPhonemeFormat &&
         Left.SilenceCharacters == Right.SilenceCharacters &&
         Left.SilencePhoneme == Right.SilencePhoneme &&
         FMath::IsNearlyEqual(Left.VisemeChangeTolerance,
                              Right.VisemeChangeTolerance) &&
         Left.TtsEndpoint == Right.TtsEndpoint &&
         Left.TtsVerb == Right.TtsVerb &&
         Left.TtsContentTypeHeader == Right.TtsContentTypeHeader &&
         Left.TtsContentType == Right.TtsContentType &&
         Left.TtsRequestFormat == Right.TtsRequestFormat &&
         Left.TtsSuccessResponseCode == Right.TtsSuccessResponseCode &&
         Left.MinimumAudioBytes == Right.MinimumAudioBytes &&
         Left.SpeechStartLogFormat == Right.SpeechStartLogFormat &&
         Left.SpeechAudioReceivedLogFormat ==
             Right.SpeechAudioReceivedLogFormat &&
         Left.ResetMorphTargets == Right.ResetMorphTargets &&
         Left.VisemeMappings == Right.VisemeMappings &&
         Left.DurationRules == Right.DurationRules;
}

inline bool operator!=(const FSpeechRuntimeSettings &Left,
                       const FSpeechRuntimeSettings &Right) {
  return !(Left == Right);
}

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
