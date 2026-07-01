#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"

#include "Features/Components/Data/Json/JsonAdapters.h"
#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"

namespace ForbocAI {
namespace Demo {
namespace Data {
namespace RuntimeSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

FColor ReadColorSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  return FColor(static_cast<uint8>(Int(TEXT("r"))),
                static_cast<uint8>(Int(TEXT("g"))),
                static_cast<uint8>(Int(TEXT("b"))),
                static_cast<uint8>(Int(TEXT("a"))));
}

FLinearColor ReadLinearColorSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  return FLinearColor(Float(TEXT("r")), Float(TEXT("g")), Float(TEXT("b")),
                      Float(TEXT("a")));
}

FRuntimeObservationIdSettings
ReadRuntimeObservationIdSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeObservationIdSettings>(
      Object,
      JSON_SETTINGS_FIELDS(FRuntimeObservationIdSettings,
                           PlayerPresentationRequested,
                           PlayerMovementInputObserved,
                           TownspersonPresentationRequested,
                           HorsePresentationRequested,
                           TownspersonCandidatesObserved));
}

FRuntimeDebugMessageSettings
ReadRuntimeDebugMessageSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeDebugMessageSettings>(
      Object,
      {JSON_SETTING_FIELDS(FRuntimeDebugMessageSettings, OnScreenKey,
                           DurationSeconds),
       JSON_OBJECT_SETTING_FIELD(FRuntimeDebugMessageSettings,
                                 ReadColorSettings, Color)});
}

FRuntimeViewNameSettings
ReadRuntimeViewNameSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeViewNameSettings>(
      Object, JSON_SETTINGS_FIELDS(FRuntimeViewNameSettings, SceneRoot,
                                   TownspersonCharacterMesh,
                                   TownspersonInteractionSphere,
                                   TownspersonNameText, TownspersonPromptText,
                                   TownspersonDialogueText,
                                   TownspersonOverlapProfile,
                                   HorseImportedMesh, HorseMountedRiderMesh,
                                   HorseNameText, PlayerCameraBoom,
                                   PlayerFollowCamera, SpeechPresentationMesh,
                                   SpeechComponent));
}

FRuntimeTextSettings
ReadRuntimeTextSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FRuntimeTextSettings>(
      Object, JSON_SETTINGS_FIELDS(FRuntimeTextSettings,
                                   TownspersonNameRoleFormat, NpcReplyLog,
                                   TownspersonMissingMesh,
                                   TownspersonMissingAnimation,
                                   HorseMissingMesh, HorseMissingWalkAnimation,
                                   RiderMissingWalkAnimation, RiderMissingMesh,
                                   StartupSdkEnabled, StartupSdkDisabled));
}

FUIRuntimeSettings ReadUIRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FUIRuntimeSettings>(
      Object,
      {JSON_SETTING_FIELDS(FUIRuntimeSettings, PlayerRoleLabel,
                           SystemRoleLabel, NpcRoleLabel, UnknownRoleLabel,
                           ChatMessageFormat, HistoryRoleSeparator,
                           HistoryTextStartOffset, HistoryMinimumRoleIndex,
                           PlaceholderTitle, PlaceholderPlayerLine,
                           PlaceholderNpcReply, ConversationTitleFormat,
                           PlayerLineFormat, NpcReplyFormat,
                           ConnectionMessageFormat, UnboundDialogueError,
                           PayloadIdFormat, PanelPadding, TitleSize, BodySize),
       JSON_OBJECT_SETTING_FIELDS(FUIRuntimeSettings, ReadLinearColorSettings,
                                  PanelColor, TitleColor, PlayerColor,
                                  SystemColor, NpcColor, UnknownColor,
                                  RuntimeReplyColor)});
}

FDialogueRuntimeSettings
ReadDialogueRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FDialogueRuntimeSettings>(
      Object, JSON_SETTINGS_FIELDS(FDialogueRuntimeSettings,
                                   ReplyPayloadIdFormat));
}

FBotStatPresetSettings
ReadBotStatPresetSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FBotStatPresetSettings>(
      Object, JSON_SETTINGS_FIELDS(FBotStatPresetSettings, MoveSpeed,
                                   AwarenessRange, Resolve, bCanTalk));
}

FBotRuntimeSettings
ReadBotRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FBotRuntimeSettings>(
      Object,
      {JSON_SETTING_FIELDS(FBotRuntimeSettings, InitialName, InitialHealth,
                           InitialMaxHealth, InitialMana, InitialMaxMana,
                           InitialStamina, InitialMaxStamina, MinimumHealth),
       JSON_SETTING_FIELDS(FBotRuntimeSettings, InitialPosition,
                           InitialRotation, InitialLastKnownPlayerPosition,
                           InitialTimeSinceLastSeenPlayer,
                           EnemySpottedTimeSinceLastSeenPlayer,
                           bInitialHasAggro, bDefaultHazardOverlapping,
                           bDefaultVisibilityCanSeeEnemy),
       JSON_SETTING_FIELDS(FBotRuntimeSettings, InitialPhase, InitialTickCount,
                           AggroTimeoutSeconds, DamageFleeHealthRatio,
                           PhaseFleeHealthRatio,
                           MovementArrivalDistanceSquared,
                           AggroPositionToleranceSquared,
                           DefaultMovementInterpSpeed),
       JSON_SETTING_FIELDS(FBotRuntimeSettings, PatrolGoalPriority,
                           PatrolGoalIdFormat, bPatrolGoalInitialCompleted,
                           bActiveGoalComponentHasActiveGoal),
       JSON_SETTING_FIELDS(FBotRuntimeSettings, ObservationIntervalSeconds,
                           InitialObservationTimeSeconds,
                           bOrchestratorCanEverTick, bRegisteredBotActive,
                           bPositionPayloadHasLocalLocation,
                           bPositionPayloadHasWorldLocation),
       JSON_SETTING_FIELDS(FBotRuntimeSettings, StartLog, RegisteredLogFormat,
                           ProcessFailedLogFormat, ExecuteLogFormat,
                           NullActorLabel, MoveActionType, AttackActionType,
                           MoveActionOffset, StateObservationFormat,
                           DefaultBehaviorState),
       JSON_OBJECT_SETTING_FIELDS(FBotRuntimeSettings, ReadBotStatPresetSettings,
                                  TownspersonStats, HorseStats)});
}

FSpeechVisemeMappingSettings
ReadSpeechVisemeMappingSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpeechVisemeMappingSettings>(
      Object, JSON_SETTINGS_FIELDS(FSpeechVisemeMappingSettings, Phoneme,
                                   MorphTargetName, BlendWeight));
}

FSpeechVowelPhonemeSettings
ReadSpeechVowelPhonemeSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpeechVowelPhonemeSettings>(
      Object, JSON_SETTINGS_FIELDS(FSpeechVowelPhonemeSettings, Character,
                                   Phoneme));
}

FSpeechPhonemeDurationRuleSettings
ReadSpeechPhonemeDurationRuleSettings(
    const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpeechPhonemeDurationRuleSettings>(
      Object, JSON_SETTINGS_FIELDS(FSpeechPhonemeDurationRuleSettings, Kind,
                                   Phoneme, Multiplier));
}

FSpeechRuntimeSettings
ReadSpeechRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  return Json::ReadSettingsFields<FSpeechRuntimeSettings>(
      Object,
      {JSON_SETTING_FIELDS(FSpeechRuntimeSettings, RestViseme, RestWeight,
                           SpeechRate, Volume, bEnableLipSync, bCanEverTick,
                           bStartTickEnabled, InitialPlaybackTime,
                           bInitialSpeechActive, EstimatedBasePhonemeSeconds,
                           SilenceCharacters, SilencePhoneme,
                           VisemeChangeTolerance, TtsEndpoint, TtsVerb,
                           TtsContentTypeHeader, TtsContentType,
                           TtsRequestFormat, TtsSuccessResponseCode,
                           MinimumAudioBytes, SpeechStartLogFormat,
                           SpeechAudioReceivedLogFormat, ResetMorphTargets),
       JSON_OBJECT_ARRAY_SETTING_FIELDS(FSpeechRuntimeSettings,
                                        ReadSpeechVowelPhonemeSettings,
                                        VowelPhonemes),
       JSON_OBJECT_ARRAY_SETTING_FIELDS(FSpeechRuntimeSettings,
                                        ReadSpeechVisemeMappingSettings,
                                        VisemeMappings),
       JSON_OBJECT_ARRAY_SETTING_FIELDS(FSpeechRuntimeSettings,
                                        ReadSpeechPhonemeDurationRuleSettings,
                                        DurationRules)});
}

TSharedPtr<FJsonObject>
LoadRuntimeSettingsSubdomain(const TSharedPtr<FJsonObject> &Manifest,
                             const char *FieldAtom) {
  return Json::LoadRequiredObjectFromContent(
      {Json::ReadStringField(Manifest, FieldAtom)});
}

} // namespace

FDemoRuntimeSettings
ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const TSharedPtr<FJsonObject> Player =
      LoadRuntimeSettingsSubdomain(Object, "Player");
  const TSharedPtr<FJsonObject> Interaction =
      LoadRuntimeSettingsSubdomain(Object, "Interaction");
  const TSharedPtr<FJsonObject> Level =
      LoadRuntimeSettingsSubdomain(Object, "Level");
  const TSharedPtr<FJsonObject> Rendering =
      LoadRuntimeSettingsSubdomain(Object, "Rendering");
  const TSharedPtr<FJsonObject> RenderingAssets =
      LoadRuntimeSettingsSubdomain(Rendering, "RenderingAssets");
  const TSharedPtr<FJsonObject> RenderingProfile =
      LoadRuntimeSettingsSubdomain(Rendering, "RenderingProfile");
  const TSharedPtr<FJsonObject> RenderingRuntime =
      LoadRuntimeSettingsSubdomain(Rendering, "RenderingRuntime");
  const TSharedPtr<FJsonObject> RenderingRuntimeTextureSettings =
      LoadRuntimeSettingsSubdomain(RenderingRuntime, "TextureSettings");
  const TSharedPtr<FJsonObject> RenderingRuntimeConsoleVariables =
      LoadRuntimeSettingsSubdomain(RenderingRuntime, "ConsoleVariables");
  const TSharedPtr<FJsonObject> RenderingRuntimeTexturePalettes =
      LoadRuntimeSettingsSubdomain(RenderingRuntime, "TexturePalettes");
  const TSharedPtr<FJsonObject> TextureCatalog =
      LoadRuntimeSettingsSubdomain(Rendering, "TextureCatalog");
  const TSharedPtr<FJsonObject> Bots =
      LoadRuntimeSettingsSubdomain(Object, "Bots");
  const TSharedPtr<FJsonObject> Dialogue =
      LoadRuntimeSettingsSubdomain(Object, "Dialogue");
  const TSharedPtr<FJsonObject> Speech =
      LoadRuntimeSettingsSubdomain(Object, "Speech");
  const TSharedPtr<FJsonObject> UI =
      LoadRuntimeSettingsSubdomain(Object, "UI");
  const TSharedPtr<FJsonObject> Runtime =
      LoadRuntimeSettingsSubdomain(Object, "Runtime");
  FDemoRuntimeSettings Settings;
  Settings.PlayerPresentation =
      PlayerSettingsAdapters::ReadPlayerPresentationSettings(
          Json::ReadObjectField(Player, "PlayerPresentation"));
  Settings.Interaction = PlayerSettingsAdapters::ReadInteractionSettings(
      Json::ReadObjectField(Interaction, "Interaction"));
  Settings.TownspersonDefaults =
      BotSettingsAdapters::ReadTownspersonDefaultsSettings(
          Json::ReadObjectField(Bots, "TownspersonDefaults"));
  Settings.LevelTerrainSources =
      LevelSettingsAdapters::ReadLevelTerrainSourceSettings(
          Json::ReadObjectField(Level, "LevelTerrainSources"));
  Settings.LevelDataSources =
      LevelSettingsAdapters::ReadLevelDataSourceSettings(
          Json::ReadObjectField(Level, "LevelDataSources"));
  Settings.RuntimeValidation =
      LevelSettingsAdapters::ReadRuntimeValidationSettings(
          Json::ReadObjectField(Level, "RuntimeValidation"));
  Settings.LevelGeometry = LevelSettingsAdapters::ReadLevelGeometrySettings(
      Json::ReadObjectField(Level, "LevelGeometry"));
  Settings.RenderingAssets =
      RenderingSettingsAdapters::ReadRenderingAssetPathSettings(
          Json::ReadObjectField(RenderingAssets, "RenderingAssets"));
  Settings.RenderingProfile =
      RenderingSettingsAdapters::ReadRenderingProfileSettings(
          Json::ReadObjectField(RenderingProfile, "RenderingProfile"));
  Settings.TextureCatalog =
      RenderingSettingsAdapters::ReadTextureCatalogSettings(TextureCatalog);
  Settings.RenderingRuntime =
      RenderingSettingsAdapters::ReadRenderingRuntimeSettings(
          Json::ReadObjectField(RenderingRuntimeTextureSettings,
                                "TextureSettings"),
          RenderingRuntimeConsoleVariables,
          RenderingRuntimeTexturePalettes);
  Settings.DialogueRuntime =
      ReadDialogueRuntimeSettings(
          Json::ReadObjectField(Dialogue, "RuntimeDialogue"));
  Settings.BotRuntime =
      ReadBotRuntimeSettings(Json::ReadObjectField(Bots, "RuntimeBots"));
  Settings.TownspersonPresentation =
      BotSettingsAdapters::ReadTownspersonPresentationSettings(
          Json::ReadObjectField(Bots, "TownspersonPresentation"));
  Settings.HorsePresentation =
      BotSettingsAdapters::ReadHorsePresentationSettings(
          Json::ReadObjectField(Bots, "HorsePresentation"));
  Settings.RuntimeObservationIds =
      ReadRuntimeObservationIdSettings(
          Json::ReadObjectField(Runtime, "RuntimeObservationIds"));
  Settings.RuntimeDebugMessages =
      ReadRuntimeDebugMessageSettings(
          Json::ReadObjectField(Runtime, "RuntimeDebugMessages"));
  Settings.RuntimeViewNames =
      ReadRuntimeViewNameSettings(
          Json::ReadObjectField(Runtime, "RuntimeViewNames"));
  Settings.RuntimeText =
      ReadRuntimeTextSettings(Json::ReadObjectField(Runtime, "RuntimeText"));
  Settings.UIRuntime =
      ReadUIRuntimeSettings(Json::ReadObjectField(UI, "RuntimeUI"));
  Settings.SpeechRuntime =
      ReadSpeechRuntimeSettings(Json::ReadObjectField(Speech, "RuntimeSpeech"));
  return Settings;
}

FDemoRuntimeSettings LoadDemoRuntimeSettings() {
  return ReadDemoRuntimeSettings(
      Json::LoadRequiredObjectFromContent(
          {TEXT("Data/runtime_demo_settings.json")}));
}

} // namespace RuntimeSettingsAdapters
} // namespace Data
} // namespace Demo
} // namespace ForbocAI
