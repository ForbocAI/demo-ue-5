#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsAdapters.h"

#include "Features/Components/Data/RuntimeSettings/BotSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/PlayerSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/RenderingSettingsAdapters.h"
#include "Features/Components/Data/RuntimeSettings/RuntimeSettingsRegistryAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace RuntimeSettingsAdapters {
namespace Json = JsonAdapters;
namespace {

using FRuntimeSettingsSourceCatalog = TMap<FString, TSharedPtr<FJsonObject>>;

struct FRuntimeSettingsSourceGroup {
  const char *Parent;
  TArray<const char *> Children;

  FRuntimeSettingsSourceGroup(const char *InParent,
                              std::initializer_list<const char *> InChildren)
      : Parent(InParent), Children(InChildren) {}
};

FString RuntimeSettingsSourceKey(const char *Atom) {
  return Json::SettingsFieldName(Atom);
}

TSharedPtr<FJsonObject>
RuntimeSettingsSource(const FRuntimeSettingsSourceCatalog &Sources,
                      const char *Atom) {
  const TSharedPtr<FJsonObject> *Source =
      Sources.Find(RuntimeSettingsSourceKey(Atom));
  check(Source != nullptr);
  check(Source->IsValid());
  return *Source;
}

TSharedPtr<FJsonObject>
LoadRuntimeSettingsSource(const TSharedPtr<FJsonObject> &Manifest,
                          const char *FieldAtom) {
  return Json::LoadRequiredObjectFromContent(
      {Json::ReadStringField(Manifest, FieldAtom)});
}

const TArray<FRuntimeSettingsSourceGroup> &RuntimeSettingsSourceGroups() {
  static const TArray<FRuntimeSettingsSourceGroup> Groups = {
      {"Root",
       {"Player", "Interaction", "Level", "Rendering", "Bots", "Dialogue",
        "Speech", "UI", "Runtime"}},
      {"Rendering",
       {"RenderingAssets", "RenderingProfile", "RenderingRuntime",
        "TextureCatalog", "RenderingDistanceLod"}},
      {"RenderingRuntime",
       {"TextureSettings", "ConsoleVariables", "TexturePalettes"}}};
  return Groups;
}

FRuntimeSettingsSourceCatalog
LoadRuntimeSettingsSourceGroup(const FRuntimeSettingsSourceCatalog &Sources,
                               const FRuntimeSettingsSourceGroup &Group) {
  const TSharedPtr<FJsonObject> Parent =
      RuntimeSettingsSource(Sources, Group.Parent);
  return func::fold_indexed(
      Group.Children, static_cast<size_t>(Group.Children.Num()), Sources,
      [Parent](const FRuntimeSettingsSourceCatalog &Current,
               const char *Child) {
        FRuntimeSettingsSourceCatalog Next = Current;
        Next.Add(RuntimeSettingsSourceKey(Child),
                 LoadRuntimeSettingsSource(Parent, Child));
        return Next;
      });
}

FRuntimeSettingsSourceCatalog
LoadRuntimeSettingsSources(const TSharedPtr<FJsonObject> &Object) {
  FRuntimeSettingsSourceCatalog Seed;
  Seed.Add(RuntimeSettingsSourceKey("Root"), Object);
  const TArray<FRuntimeSettingsSourceGroup> &Groups =
      RuntimeSettingsSourceGroups();
  return func::fold_indexed(
      Groups, static_cast<size_t>(Groups.Num()), Seed,
      LoadRuntimeSettingsSourceGroup);
}

} // namespace

FRuntimeSettings
ReadRuntimeSettings(const TSharedPtr<FJsonObject> &Object) {
  const FRuntimeSettingsSourceCatalog Sources =
      LoadRuntimeSettingsSources(Object);
  FRuntimeSettings Settings;
  Settings.PlayerPresentation = PlayerSettingsAdapters::
      ReadPlayerPresentationSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Player"), "PlayerPresentation"));
  Settings.Interaction = PlayerSettingsAdapters::ReadInteractionSettings(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "Interaction"),
                            "Interaction"));
  Settings.TownspersonDefaults = BotSettingsAdapters::
      ReadTownspersonDefaultsSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Bots"), "TownspersonDefaults"));
  Settings.LevelTerrainSources = LevelSettingsAdapters::
      ReadLevelTerrainSourceSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Level"), "LevelTerrainSources"));
  Settings.LevelDataSources = LevelSettingsAdapters::
      ReadLevelDataSourceSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Level"), "LevelDataSources"));
  Settings.RuntimeValidation = LevelSettingsAdapters::
      ReadRuntimeValidationSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Level"), "RuntimeValidation"));
  Settings.LevelGeometry = LevelSettingsAdapters::ReadLevelGeometrySettings(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "Level"),
                            "LevelGeometry"));
  Settings.RenderingAssets = RenderingSettingsAdapters::
      ReadRenderingAssetPathSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "RenderingAssets"),
          "RenderingAssets"));
  Settings.RenderingProfile = RenderingSettingsAdapters::
      ReadRenderingProfileSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "RenderingProfile"),
          "RenderingProfile"));
  Settings.TextureCatalog = RenderingSettingsAdapters::
      ReadTextureCatalogSettings(RuntimeSettingsSource(Sources,
                                                       "TextureCatalog"));
  Settings.RenderingRuntime =
      RenderingSettingsAdapters::ReadRenderingRuntimeSettings({
          Json::ReadObjectField(RuntimeSettingsSource(Sources,
                                                      "TextureSettings"),
                                "TextureSettings"),
          RuntimeSettingsSource(Sources, "ConsoleVariables"),
          RuntimeSettingsSource(Sources, "TexturePalettes")});
  Settings.RenderingDistanceLod =
      RenderingSettingsAdapters::ReadRenderingDistanceLodSettings(
          Json::ReadObjectField(RuntimeSettingsSource(
                                    Sources, "RenderingDistanceLod"),
                                "DistanceLod"));
  Settings.DialogueRuntime =
      Json::ReadSettingsWith<FDialogueRuntimeSettings>(
          JSON_SETTINGS_ATOMS(ReplyPayloadIdFormat))(
          Json::ReadObjectField(RuntimeSettingsSource(Sources, "Dialogue"),
                                "RuntimeDialogue"));
  Settings.BotRuntime = Json::ReadSettingsWith<FBotRuntimeSettings>(
      JSON_SETTINGS_ATOMS(
          InitialName, InitialHealth, InitialMaxHealth, InitialMana,
          InitialMaxMana, InitialStamina, InitialMaxStamina, MinimumHealth,
          InitialPosition, InitialRotation, InitialLastKnownPlayerPosition,
          InitialTimeSinceLastSeenPlayer,
          EnemySpottedTimeSinceLastSeenPlayer, bInitialHasAggro,
          bDefaultHazardOverlapping, bDefaultVisibilityCanSeeEnemy,
          InitialPhase, InitialTickCount, AggroTimeoutSeconds,
          DamageFleeHealthRatio, PhaseFleeHealthRatio,
          MovementArrivalDistanceSquared, AggroPositionToleranceSquared,
          DefaultMovementInterpSpeed, PatrolGoalPriority, PatrolGoalIdFormat,
          bPatrolGoalInitialCompleted, bActiveGoalComponentHasActiveGoal,
          TownspersonStats, HorseStats, ObservationIntervalSeconds,
          InitialObservationTimeSeconds, bOrchestratorCanEverTick,
          PatrolTickIntervalSeconds, InitialPatrolPauseRemainingSeconds,
          bRegisteredBotActive, bPositionPayloadHasLocalLocation,
          bPositionPayloadHasWorldLocation, StartLog, RegisteredLogFormat,
          ProcessFailedLogFormat, ExecuteLogFormat, NullActorLabel,
          MoveActionType, AttackActionType, MoveActionOffset,
          StateObservationFormat, DefaultBehaviorState))(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "Bots"),
                            "RuntimeBots"));
  Settings.TownspersonPresentation = BotSettingsAdapters::
      ReadTownspersonPresentationSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Bots"),
          "TownspersonPresentation"));
  Settings.HorsePresentation = BotSettingsAdapters::
      ReadHorsePresentationSettings(Json::ReadObjectField(
          RuntimeSettingsSource(Sources, "Bots"), "HorsePresentation"));
  Settings.RuntimeObservationIds =
      Json::ReadSettingsWith<FRuntimeObservationIdSettings>(
          JSON_SETTINGS_ATOMS(PlayerPresentationRequested,
                              PlayerMovementInputObserved,
                              TownspersonPresentationRequested,
                              HorsePresentationRequested,
                              TownspersonCandidatesObserved))(
          Json::ReadObjectField(RuntimeSettingsSource(Sources, "Runtime"),
                                "RuntimeObservationIds"));
  Settings.RuntimeDebugMessages =
      Json::ReadSettingsWith<FRuntimeDebugMessageSettings>(
          JSON_SETTINGS_ATOMS(OnScreenKey, DurationSeconds, Color))(
          Json::ReadObjectField(RuntimeSettingsSource(Sources, "Runtime"),
                                "RuntimeDebugMessages"));
  Settings.RuntimeViewNames =
      Json::ReadSettingsWith<FRuntimeViewNameSettings>(
          JSON_SETTINGS_ATOMS(SceneRoot, TownspersonCharacterMesh,
                              TownspersonInteractionSphere,
                              TownspersonNameText, TownspersonPromptText,
                              TownspersonDialogueText,
                              TownspersonOverlapProfile, HorseImportedMesh,
                              HorseMountedRiderMesh, HorseNameText,
                              PlayerCameraBoom, PlayerFollowCamera,
                              SpeechPresentationMesh, SpeechComponent))(
          Json::ReadObjectField(RuntimeSettingsSource(Sources, "Runtime"),
                                "RuntimeViewNames"));
  Settings.RuntimeText = Json::ReadSettingsWith<FRuntimeTextSettings>(
      JSON_SETTINGS_ATOMS(TownspersonNameRoleFormat, NpcReplyLog,
                          TownspersonMissingMesh,
                          TownspersonMissingAnimation, HorseMissingMesh,
                          HorseMissingWalkAnimation,
                          RiderMissingWalkAnimation, RiderMissingMesh,
                          StartupSdkEnabled, StartupSdkDisabled))(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "Runtime"),
                            "RuntimeText"));
  Settings.RuntimeReduxLog =
      Json::ReadSettingsWith<FRuntimeReduxLogSettings>(
          JSON_SETTINGS_ATOMS(SampleInterval, SampledActionTypes))(
          Json::ReadObjectField(RuntimeSettingsSource(Sources, "Runtime"),
                                "RuntimeReduxLog"));
  Settings.UIRuntime = Json::ReadSettingsWith<FUIRuntimeSettings>(
      JSON_SETTINGS_ATOMS(
          PlayerRoleLabel, SystemRoleLabel, NpcRoleLabel, UnknownRoleLabel,
          ChatMessageFormat, HistoryRoleSeparator, HistoryTextStartOffset,
          HistoryMinimumRoleIndex, PlaceholderTitle, PlaceholderPlayerLine,
          PlaceholderNpcReply, ConversationTitleFormat, PlayerLineFormat,
          NpcReplyFormat, ConnectionMessageFormat, UnboundDialogueError,
          PayloadIdFormat, PanelColor, TitleColor, PlayerColor, SystemColor,
          NpcColor, UnknownColor, RuntimeReplyColor, PanelPadding, TitleSize,
          BodySize, StatsOverlay))(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "UI"),
                            "RuntimeUI"));
  Settings.SpeechRuntime = Json::ReadSettingsWith<FSpeechRuntimeSettings>(
      JSON_SETTINGS_ATOMS(
          RestViseme, RestWeight, SpeechRate, Volume, bEnableLipSync,
          bCanEverTick, bStartTickEnabled, InitialPlaybackTime,
          bInitialSpeechActive, EstimatedBasePhonemeSeconds, VowelPhonemes,
          SilenceCharacters, SilencePhoneme, VisemeChangeTolerance,
          TtsEndpoint, TtsVerb, TtsContentTypeHeader, TtsContentType,
          TtsRequestFormat, TtsSuccessResponseCode, MinimumAudioBytes,
          SpeechStartLogFormat, SpeechAudioReceivedLogFormat,
          ResetMorphTargets, VisemeMappings, DurationRules))(
      Json::ReadObjectField(RuntimeSettingsSource(Sources, "Speech"),
                            "RuntimeSpeech"));
  return Settings;
}

FRuntimeSettings LoadRuntimeSettings() {
  return ReadRuntimeSettings(Json::LoadRequiredObjectFromContent(
      {TEXT("Data/runtime_settings.json")}));
}

} // namespace RuntimeSettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
