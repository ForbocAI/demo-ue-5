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
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FRuntimeObservationIdSettings Settings;
  Settings.PlayerPresentationRequested =
      String(TEXT("player_presentation_requested"));
  Settings.PlayerMovementInputObserved =
      String(TEXT("player_movement_input_observed"));
  Settings.TownspersonPresentationRequested =
      String(TEXT("townsperson_presentation_requested"));
  Settings.HorsePresentationRequested =
      String(TEXT("horse_presentation_requested"));
  Settings.TownspersonCandidatesObserved =
      String(TEXT("townsperson_candidates_observed"));
  return Settings;
}

FRuntimeDebugMessageSettings
ReadRuntimeDebugMessageSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  FRuntimeDebugMessageSettings Settings;
  Settings.OnScreenKey = Int(TEXT("on_screen_key"));
  Settings.DurationSeconds = Float(TEXT("duration_seconds"));
  Settings.Color = ReadColorSettings(ObjectValue(TEXT("color")));
  return Settings;
}

FRuntimeViewNameSettings
ReadRuntimeViewNameSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FRuntimeViewNameSettings Settings;
  Settings.SceneRoot = String(TEXT("scene_root"));
  Settings.TownspersonCharacterMesh =
      String(TEXT("townsperson_character_mesh"));
  Settings.TownspersonInteractionSphere =
      String(TEXT("townsperson_interaction_sphere"));
  Settings.TownspersonNameText = String(TEXT("townsperson_name_text"));
  Settings.TownspersonPromptText = String(TEXT("townsperson_prompt_text"));
  Settings.TownspersonDialogueText =
      String(TEXT("townsperson_dialogue_text"));
  Settings.TownspersonOverlapProfile =
      String(TEXT("townsperson_overlap_profile"));
  Settings.HorseImportedMesh = String(TEXT("horse_imported_mesh"));
  Settings.HorseMountedRiderMesh = String(TEXT("horse_mounted_rider_mesh"));
  Settings.HorseNameText = String(TEXT("horse_name_text"));
  Settings.PlayerCameraBoom = String(TEXT("player_camera_boom"));
  Settings.PlayerFollowCamera = String(TEXT("player_follow_camera"));
  Settings.SpeechPresentationMesh = String(TEXT("speech_presentation_mesh"));
  Settings.SpeechComponent = String(TEXT("speech_component"));
  return Settings;
}

FRuntimeTextSettings
ReadRuntimeTextSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FRuntimeTextSettings Settings;
  Settings.TownspersonNameRoleFormat =
      String(TEXT("townsperson_name_role_format"));
  Settings.NpcReplyLog = String(TEXT("npc_reply_log"));
  Settings.TownspersonMissingMesh = String(TEXT("townsperson_missing_mesh"));
  Settings.TownspersonMissingAnimation =
      String(TEXT("townsperson_missing_animation"));
  Settings.HorseMissingMesh = String(TEXT("horse_missing_mesh"));
  Settings.HorseMissingWalkAnimation =
      String(TEXT("horse_missing_walk_animation"));
  Settings.RiderMissingWalkAnimation =
      String(TEXT("rider_missing_walk_animation"));
  Settings.RiderMissingMesh = String(TEXT("rider_missing_mesh"));
  Settings.StartupSdkEnabled = String(TEXT("startup_sdk_enabled"));
  Settings.StartupSdkDisabled = String(TEXT("startup_sdk_disabled"));
  return Settings;
}

FUIRuntimeSettings ReadUIRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  FUIRuntimeSettings Settings;
  Settings.PlayerRoleLabel = String(TEXT("player_role_label"));
  Settings.SystemRoleLabel = String(TEXT("system_role_label"));
  Settings.NpcRoleLabel = String(TEXT("npc_role_label"));
  Settings.UnknownRoleLabel = String(TEXT("unknown_role_label"));
  Settings.ChatMessageFormat = String(TEXT("chat_message_format"));
  Settings.HistoryRoleSeparator = String(TEXT("history_role_separator"));
  Settings.HistoryTextStartOffset = Int(TEXT("history_text_start_offset"));
  Settings.HistoryMinimumRoleIndex = Int(TEXT("history_minimum_role_index"));
  Settings.PlaceholderTitle = String(TEXT("placeholder_title"));
  Settings.PlaceholderPlayerLine = String(TEXT("placeholder_player_line"));
  Settings.PlaceholderNpcReply = String(TEXT("placeholder_npc_reply"));
  Settings.ConversationTitleFormat = String(TEXT("conversation_title_format"));
  Settings.PlayerLineFormat = String(TEXT("player_line_format"));
  Settings.NpcReplyFormat = String(TEXT("npc_reply_format"));
  Settings.ConnectionMessageFormat = String(TEXT("connection_message_format"));
  Settings.UnboundDialogueError = String(TEXT("unbound_dialogue_error"));
  Settings.PayloadIdFormat = String(TEXT("payload_id_format"));
  Settings.PanelColor =
      ReadLinearColorSettings(ObjectValue(TEXT("panel_color")));
  Settings.TitleColor =
      ReadLinearColorSettings(ObjectValue(TEXT("title_color")));
  Settings.PlayerColor =
      ReadLinearColorSettings(ObjectValue(TEXT("player_color")));
  Settings.SystemColor =
      ReadLinearColorSettings(ObjectValue(TEXT("system_color")));
  Settings.NpcColor = ReadLinearColorSettings(ObjectValue(TEXT("npc_color")));
  Settings.UnknownColor =
      ReadLinearColorSettings(ObjectValue(TEXT("unknown_color")));
  Settings.RuntimeReplyColor =
      ReadLinearColorSettings(ObjectValue(TEXT("runtime_reply_color")));
  Settings.PanelPadding = Float(TEXT("panel_padding"));
  Settings.TitleSize = Float(TEXT("title_size"));
  Settings.BodySize = Float(TEXT("body_size"));
  return Settings;
}

FDialogueRuntimeSettings
ReadDialogueRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  FDialogueRuntimeSettings Settings;
  Settings.ReplyPayloadIdFormat = String(TEXT("reply_payload_id_format"));
  return Settings;
}

FBotStatPresetSettings
ReadBotStatPresetSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonBoolReader Bool = Json::BoolIn(Object);
  FBotStatPresetSettings Settings;
  Settings.MoveSpeed = Float(TEXT("move_speed"));
  Settings.AwarenessRange = Float(TEXT("awareness_range"));
  Settings.Resolve = Float(TEXT("resolve"));
  Settings.bCanTalk = Bool(TEXT("can_talk"));
  return Settings;
}

FBotRuntimeSettings
ReadBotRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonObjectValueReader ObjectValue = Json::ObjectValueIn(Object);
  const Json::FJsonVectorReader Vector = Json::VectorIn(Object);
  FBotRuntimeSettings Settings;
  Settings.InitialName = String(TEXT("initial_name"));
  Settings.InitialHealth = Float(TEXT("initial_health"));
  Settings.InitialMaxHealth = Float(TEXT("initial_max_health"));
  Settings.InitialMana = Float(TEXT("initial_mana"));
  Settings.InitialMaxMana = Float(TEXT("initial_max_mana"));
  Settings.InitialStamina = Float(TEXT("initial_stamina"));
  Settings.InitialMaxStamina = Float(TEXT("initial_max_stamina"));
  Settings.MinimumHealth = Float(TEXT("minimum_health"));
  Settings.InitialPosition = Vector(TEXT("initial_position"));
  Settings.InitialRotation =
      Json::RotatorIn(Object)(TEXT("initial_rotation"));
  Settings.InitialLastKnownPlayerPosition =
      Vector(TEXT("initial_last_known_player_position"));
  Settings.InitialTimeSinceLastSeenPlayer =
      Float(TEXT("initial_time_since_last_seen_player"));
  Settings.EnemySpottedTimeSinceLastSeenPlayer =
      Float(TEXT("enemy_spotted_time_since_last_seen_player"));
  Settings.bInitialHasAggro = Json::BoolIn(Object)(TEXT("initial_has_aggro"));
  Settings.bDefaultHazardOverlapping =
      Bool(TEXT("default_hazard_overlapping"));
  Settings.bDefaultVisibilityCanSeeEnemy =
      Bool(TEXT("default_visibility_can_see_enemy"));
  Settings.InitialPhase = Int(TEXT("initial_phase"));
  Settings.InitialTickCount = Int(TEXT("initial_tick_count"));
  Settings.AggroTimeoutSeconds = Float(TEXT("aggro_timeout_seconds"));
  Settings.DamageFleeHealthRatio = Float(TEXT("damage_flee_health_ratio"));
  Settings.PhaseFleeHealthRatio = Float(TEXT("phase_flee_health_ratio"));
  Settings.MovementArrivalDistanceSquared =
      Float(TEXT("movement_arrival_distance_squared"));
  Settings.AggroPositionToleranceSquared =
      Float(TEXT("aggro_position_tolerance_squared"));
  Settings.DefaultMovementInterpSpeed =
      Float(TEXT("default_movement_interp_speed"));
  Settings.PatrolGoalPriority = Int(TEXT("patrol_goal_priority"));
  Settings.PatrolGoalIdFormat = String(TEXT("patrol_goal_id_format"));
  Settings.bPatrolGoalInitialCompleted =
      Bool(TEXT("patrol_goal_initial_completed"));
  Settings.bActiveGoalComponentHasActiveGoal =
      Bool(TEXT("active_goal_component_has_active_goal"));
  Settings.TownspersonStats =
      ReadBotStatPresetSettings(ObjectValue(TEXT("townsperson_stats")));
  Settings.HorseStats =
      ReadBotStatPresetSettings(ObjectValue(TEXT("horse_stats")));
  Settings.ObservationIntervalSeconds =
      Float(TEXT("observation_interval_seconds"));
  Settings.InitialObservationTimeSeconds =
      Float(TEXT("initial_observation_time_seconds"));
  Settings.bOrchestratorCanEverTick =
      Bool(TEXT("orchestrator_can_ever_tick"));
  Settings.bRegisteredBotActive = Bool(TEXT("registered_bot_active"));
  Settings.bPositionPayloadHasLocalLocation =
      Bool(TEXT("position_payload_has_local_location"));
  Settings.bPositionPayloadHasWorldLocation =
      Bool(TEXT("position_payload_has_world_location"));
  Settings.StartLog = String(TEXT("start_log"));
  Settings.RegisteredLogFormat = String(TEXT("registered_log_format"));
  Settings.ProcessFailedLogFormat = String(TEXT("process_failed_log_format"));
  Settings.ExecuteLogFormat = String(TEXT("execute_log_format"));
  Settings.NullActorLabel = String(TEXT("null_actor_label"));
  Settings.MoveActionType = String(TEXT("move_action_type"));
  Settings.AttackActionType = String(TEXT("attack_action_type"));
  Settings.MoveActionOffset = Vector(TEXT("move_action_offset"));
  Settings.StateObservationFormat = String(TEXT("state_observation_format"));
  Settings.DefaultBehaviorState = Int(TEXT("default_behavior_state"));
  return Settings;
}

TArray<FString>
ReadStringArraySettings(const TArray<TSharedPtr<FJsonValue>> &Values) {
  return func::fold_indexed(
      Values, static_cast<size_t>(Values.Num()), TArray<FString>(),
      [](const TArray<FString> &Acc, const TSharedPtr<FJsonValue> &Value) {
        check(Value.IsValid());
        TArray<FString> Next = Acc;
        Next.Add(Value->AsString());
        return Next;
      });
}

FSpeechVisemeMappingSettings
ReadSpeechVisemeMappingSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  FSpeechVisemeMappingSettings Settings;
  Settings.Phoneme = String(TEXT("phoneme"));
  Settings.MorphTargetName = String(TEXT("morph_target_name"));
  Settings.BlendWeight = Float(TEXT("blend_weight"));
  return Settings;
}

FSpeechPhonemeDurationRuleSettings
ReadSpeechPhonemeDurationRuleSettings(
    const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  FSpeechPhonemeDurationRuleSettings Settings;
  Settings.Kind = String(TEXT("kind"));
  Settings.Phoneme = String(TEXT("phoneme"));
  Settings.Multiplier = Float(TEXT("multiplier"));
  return Settings;
}

FSpeechRuntimeSettings
ReadSpeechRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const Json::FJsonStringReader String = Json::StringIn(Object);
  const Json::FJsonFloatReader Float = Json::FloatIn(Object);
  const Json::FJsonIntReader Int = Json::IntIn(Object);
  const Json::FJsonBoolReader Bool = Json::BoolIn(Object);
  const Json::FJsonArrayReader Array = Json::ArrayIn(Object);
  FSpeechRuntimeSettings Settings;
  Settings.RestViseme = String(TEXT("rest_viseme"));
  Settings.RestWeight = Float(TEXT("rest_weight"));
  Settings.SpeechRate = Float(TEXT("speech_rate"));
  Settings.Volume = Float(TEXT("volume"));
  Settings.bEnableLipSync = Bool(TEXT("enable_lip_sync"));
  Settings.bCanEverTick = Bool(TEXT("can_ever_tick"));
  Settings.bStartTickEnabled = Bool(TEXT("start_tick_enabled"));
  Settings.InitialPlaybackTime = Float(TEXT("initial_playback_time"));
  Settings.bInitialSpeechActive = Bool(TEXT("initial_speech_active"));
  Settings.EstimatedBasePhonemeSeconds =
      Float(TEXT("estimated_base_phoneme_seconds"));
  Settings.VowelCharacters = String(TEXT("vowel_characters"));
  Settings.VowelPhonemeFormat = String(TEXT("vowel_phoneme_format"));
  Settings.SilenceCharacters = String(TEXT("silence_characters"));
  Settings.SilencePhoneme = String(TEXT("silence_phoneme"));
  Settings.VisemeChangeTolerance = Float(TEXT("viseme_change_tolerance"));
  Settings.TtsEndpoint = String(TEXT("tts_endpoint"));
  Settings.TtsVerb = String(TEXT("tts_verb"));
  Settings.TtsContentTypeHeader = String(TEXT("tts_content_type_header"));
  Settings.TtsContentType = String(TEXT("tts_content_type"));
  Settings.TtsRequestFormat = String(TEXT("tts_request_format"));
  Settings.TtsSuccessResponseCode = Int(TEXT("tts_success_response_code"));
  Settings.MinimumAudioBytes = Int(TEXT("minimum_audio_bytes"));
  Settings.SpeechStartLogFormat = String(TEXT("speech_start_log_format"));
  Settings.SpeechAudioReceivedLogFormat =
      String(TEXT("speech_audio_received_log_format"));
  Settings.ResetMorphTargets =
      ReadStringArraySettings(Array(TEXT("reset_morph_targets")));
  Settings.VisemeMappings = Json::MapJsonValues<FSpeechVisemeMappingSettings>(
      Array(TEXT("viseme_mappings")), ReadSpeechVisemeMappingSettings);
  Settings.DurationRules =
      Json::MapJsonValues<FSpeechPhonemeDurationRuleSettings>(
          Array(TEXT("duration_rules")),
          ReadSpeechPhonemeDurationRuleSettings);
  return Settings;
}

TSharedPtr<FJsonObject>
LoadRuntimeSettingsSubdomain(const TSharedPtr<FJsonObject> &Manifest,
                             const TCHAR *FieldName) {
  return Json::LoadRequiredObjectFromContent(
      {Json::StringIn(Manifest)(FieldName)});
}

} // namespace

FDemoRuntimeSettings
ReadDemoRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const TSharedPtr<FJsonObject> Player =
      LoadRuntimeSettingsSubdomain(Object, TEXT("player"));
  const TSharedPtr<FJsonObject> Interaction =
      LoadRuntimeSettingsSubdomain(Object, TEXT("interaction"));
  const TSharedPtr<FJsonObject> Level =
      LoadRuntimeSettingsSubdomain(Object, TEXT("level"));
  const TSharedPtr<FJsonObject> Rendering =
      LoadRuntimeSettingsSubdomain(Object, TEXT("rendering"));
  const TSharedPtr<FJsonObject> Bots =
      LoadRuntimeSettingsSubdomain(Object, TEXT("bots"));
  const TSharedPtr<FJsonObject> Dialogue =
      LoadRuntimeSettingsSubdomain(Object, TEXT("dialogue"));
  const TSharedPtr<FJsonObject> Speech =
      LoadRuntimeSettingsSubdomain(Object, TEXT("speech"));
  const TSharedPtr<FJsonObject> UI =
      LoadRuntimeSettingsSubdomain(Object, TEXT("ui"));
  const TSharedPtr<FJsonObject> Runtime =
      LoadRuntimeSettingsSubdomain(Object, TEXT("runtime"));
  const Json::FJsonObjectValueReader PlayerObject =
      Json::ObjectValueIn(Player);
  const Json::FJsonObjectValueReader InteractionObject =
      Json::ObjectValueIn(Interaction);
  const Json::FJsonObjectValueReader LevelObject = Json::ObjectValueIn(Level);
  const Json::FJsonObjectValueReader RenderingObject =
      Json::ObjectValueIn(Rendering);
  const Json::FJsonObjectValueReader BotsObject = Json::ObjectValueIn(Bots);
  const Json::FJsonObjectValueReader DialogueObject =
      Json::ObjectValueIn(Dialogue);
  const Json::FJsonObjectValueReader SpeechObject =
      Json::ObjectValueIn(Speech);
  const Json::FJsonObjectValueReader UIObject = Json::ObjectValueIn(UI);
  const Json::FJsonObjectValueReader RuntimeObject =
      Json::ObjectValueIn(Runtime);
  FDemoRuntimeSettings Settings;
  Settings.PlayerPresentation =
      PlayerSettingsAdapters::ReadPlayerPresentationSettings(
          PlayerObject(TEXT("player_presentation")));
  Settings.Interaction = PlayerSettingsAdapters::ReadInteractionSettings(
      InteractionObject(TEXT("interaction")));
  Settings.TownspersonDefaults =
      BotSettingsAdapters::ReadTownspersonDefaultsSettings(
          BotsObject(TEXT("townsperson_defaults")));
  Settings.LevelTerrainSources =
      LevelSettingsAdapters::ReadLevelTerrainSourceSettings(
          LevelObject(TEXT("level_terrain_sources")));
  Settings.LevelDataSources =
      LevelSettingsAdapters::ReadLevelDataSourceSettings(
          LevelObject(TEXT("level_data_sources")));
  Settings.RuntimeValidation =
      LevelSettingsAdapters::ReadRuntimeValidationSettings(
          LevelObject(TEXT("runtime_validation")));
  Settings.LevelGeometry = LevelSettingsAdapters::ReadLevelGeometrySettings(
      LevelObject(TEXT("level_geometry")));
  Settings.RenderingAssets =
      RenderingSettingsAdapters::ReadRenderingAssetPathSettings(
          RenderingObject(TEXT("rendering_assets")));
  Settings.RenderingProfile =
      RenderingSettingsAdapters::ReadRenderingProfileSettings(
          RenderingObject(TEXT("rendering_profile")));
  Settings.TextureCatalog =
      RenderingSettingsAdapters::ReadTextureCatalogSettings(Rendering);
  Settings.RenderingRuntime =
      RenderingSettingsAdapters::ReadRenderingRuntimeSettings(
          RenderingObject(TEXT("rendering_runtime")));
  Settings.DialogueRuntime =
      ReadDialogueRuntimeSettings(DialogueObject(TEXT("runtime_dialogue")));
  Settings.BotRuntime =
      ReadBotRuntimeSettings(BotsObject(TEXT("runtime_bots")));
  Settings.TownspersonPresentation =
      BotSettingsAdapters::ReadTownspersonPresentationSettings(
          BotsObject(TEXT("townsperson_presentation")));
  Settings.HorsePresentation =
      BotSettingsAdapters::ReadHorsePresentationSettings(
          BotsObject(TEXT("horse_presentation")));
  Settings.RuntimeObservationIds =
      ReadRuntimeObservationIdSettings(
          RuntimeObject(TEXT("runtime_observation_ids")));
  Settings.RuntimeDebugMessages =
      ReadRuntimeDebugMessageSettings(
          RuntimeObject(TEXT("runtime_debug_messages")));
  Settings.RuntimeViewNames =
      ReadRuntimeViewNameSettings(RuntimeObject(TEXT("runtime_view_names")));
  Settings.RuntimeText =
      ReadRuntimeTextSettings(RuntimeObject(TEXT("runtime_text")));
  Settings.UIRuntime =
      ReadUIRuntimeSettings(UIObject(TEXT("runtime_ui")));
  Settings.SpeechRuntime =
      ReadSpeechRuntimeSettings(SpeechObject(TEXT("runtime_speech")));
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
