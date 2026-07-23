#include "Features/Components/Data/Settings/DataSettingsAdapters.h"

#include "Features/Components/Data/Settings/Bot/SettingsBotAdapters.h"
#include "Features/Components/Data/Settings/Level/SettingsLevelAdapters.h"
#include "Features/Components/Data/Settings/Player/PlayerAdapters.h"
#include "Features/Components/Data/Settings/Rendering/RenderingAdapters.h"
#include "Features/Components/Data/Settings/Registry/Catalog/RegistryCatalogAdapters.h"

namespace ForbocAI {
namespace Game {
namespace Data {
namespace SettingsAdapters {
namespace Json = JsonAdapters;
namespace {

using FSettingsSourceCatalog = TMap<FString, TSharedPtr<FJsonObject>>;

struct FSourceGroup {
  const char *Parent;
  TArray<const char *> Children;

  /** User Story: As a components data settings consumer, I need to invoke fsource group through a stable signature so the components data settings workflow remains explicit and composable. @fn FSourceGroup(const char *InParent, std::initializer_list<const char *> InChildren) */
  FSourceGroup(const char *InParent,
                              std::initializer_list<const char *> InChildren)
      : Parent(InParent), Children(InChildren) {}
};

/** User Story: As a components data settings consumer, I need to invoke settings source key through a stable signature so the components data settings workflow remains explicit and composable. @fn FString SettingsSourceKey(const char *Atom) */
FString SettingsSourceKey(const char *Atom) {
  return Json::SettingsFieldName(Atom);
}

/** User Story: As a components data settings consumer, I need to invoke settings source through a stable signature so the components data settings workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> SettingsSource(const FSettingsSourceCatalog &Sources, const char *Atom) */
TSharedPtr<FJsonObject>
SettingsSource(const FSettingsSourceCatalog &Sources,
                      const char *Atom) {
  const TSharedPtr<FJsonObject> *Source =
      Sources.Find(SettingsSourceKey(Atom));
  check(Source != nullptr);
  check(Source->IsValid());
  return *Source;
}

/** User Story: As a components data settings consumer, I need to invoke load settings source through a stable signature so the components data settings workflow remains explicit and composable. @fn TSharedPtr<FJsonObject> LoadSettingsSource(const TSharedPtr<FJsonObject> &Manifest, const char *FieldAtom) */
TSharedPtr<FJsonObject>
LoadSettingsSource(const TSharedPtr<FJsonObject> &Manifest,
                          const char *FieldAtom) {
  return Json::LoadRequiredObjectFromContent(
      {Json::ReadStringField(Manifest, FieldAtom)});
}

/** User Story: As a components data settings consumer, I need to invoke settings source groups through a stable signature so the components data settings workflow remains explicit and composable. @fn const TArray<FSourceGroup> &SettingsSourceGroups() */
const TArray<FSourceGroup> &SettingsSourceGroups() {
  static const TArray<FSourceGroup> Groups = {
      {"Root",
       {"Player", "Interaction", "Level", "Rendering", "Bots", "Dialogue",
        "Speech", "UI", "Core"}},
      {"Core",
       {"ObservationIds", "DebugMessages", "ViewNames", "Text", "ReduxLog",
        "Ecs", "Automation"}},
      {"Rendering",
       {"RenderingAssets", "RenderingProfile", "RenderingPipeline",
        "TextureCatalog", "RenderingDistanceLod"}},
      {"RenderingPipeline",
       {"TextureSettings", "ConsoleSettings", "ConsoleVariables",
        "TexturePalettes"}}};
  return Groups;
}

/** User Story: As a components data settings consumer, I need to invoke load settings source group through a stable signature so the components data settings workflow remains explicit and composable. @fn FSettingsSourceCatalog LoadSettingsSourceGroup(const FSettingsSourceCatalog &Sources, const FSourceGroup &Group) */
FSettingsSourceCatalog
LoadSettingsSourceGroup(const FSettingsSourceCatalog &Sources,
                               const FSourceGroup &Group) {
  const TSharedPtr<FJsonObject> Parent =
      SettingsSource(Sources, Group.Parent);
  return func::fold_indexed(
      Group.Children, static_cast<size_t>(Group.Children.Num()), Sources,
      [Parent](const FSettingsSourceCatalog &Current,
               const char *Child) {
        FSettingsSourceCatalog Next = Current;
        Next.Add(SettingsSourceKey(Child),
                 LoadSettingsSource(Parent, Child));
        return Next;
      });
}

/** User Story: As a components data settings consumer, I need to invoke load settings sources through a stable signature so the components data settings workflow remains explicit and composable. @fn FSettingsSourceCatalog LoadSettingsSources(const TSharedPtr<FJsonObject> &Object) */
FSettingsSourceCatalog
LoadSettingsSources(const TSharedPtr<FJsonObject> &Object) {
  FSettingsSourceCatalog Seed;
  Seed.Add(SettingsSourceKey("Root"), Object);
  const TArray<FSourceGroup> &Groups =
      SettingsSourceGroups();
  return func::fold_indexed(
      Groups, static_cast<size_t>(Groups.Num()), Seed,
      LoadSettingsSourceGroup);
}

} // namespace

/** User Story: As a components data settings consumer, I need to invoke ecs domain segments through a stable signature so the components data settings workflow remains explicit and composable. @fn TArray<FString> EcsDomainSegments(const FString &Path) */
TArray<FString> EcsDomainSegments(const FString &Path) {
  TArray<FString> Segments;
  const FString Separator = FString::Chr(TCHAR('/'));
  Path.ParseIntoArray(Segments, *Separator, true);
  return Segments;
}

/** User Story: As a components data settings consumer, I need to invoke ecs domain path registration through a stable signature so the components data settings workflow remains explicit and composable. @fn ecs::FPathRegistration EcsDomainPathRegistration(const FDomainRegistrationSettings &Settings) */
ecs::FPathRegistration
EcsDomainPathRegistration(const FDomainRegistrationSettings &Settings) {
  ecs::FPathRegistration Registration;
  Registration.Segments = EcsDomainSegments(Settings.Path);
  Registration.Kind = static_cast<ecs::EKind>(Settings.Kind);
  return Registration;
}

/** User Story: As a components data settings consumer, I need to invoke ecs domain registry through a stable signature so the components data settings workflow remains explicit and composable. @fn ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) */
ecs::FGraph EcsDomainRegistry(const FEcsSettings &Settings) {
  return ecs::createDomainRegistry(
      func::map_array<FDomainRegistrationSettings,
                      ecs::FPathRegistration>(
          Settings.DomainRegistry, EcsDomainPathRegistration));
}

/** User Story: As a components data settings consumer, I need to invoke read settings through a stable signature so the components data settings workflow remains explicit and composable. @fn FSettings ReadSettings(const TSharedPtr<FJsonObject> &Object) */
FSettings
ReadSettings(const TSharedPtr<FJsonObject> &Object) {
  const FSettingsSourceCatalog Sources =
      LoadSettingsSources(Object);
  FSettings Settings;
  Settings.PlayerPresentation = PlayerSettingsAdapters::
      ReadPlayerPresentationSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Player"), "PlayerPresentation"));
  Settings.Interaction = PlayerSettingsAdapters::ReadInteractionSettings(
      Json::ReadObjectField(SettingsSource(Sources, "Interaction"),
                            "Interaction"));
  Settings.TownspersonDefaults = BotAdapters::
      ReadTownspersonDefaultsSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Bots"), "TownspersonDefaults"));
  Settings.LevelTerrainSources = LevelSettingsAdapters::
      ReadLevelTerrainSourceSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Level"), "LevelTerrainSources"));
  Settings.LevelCsv = LevelSettingsAdapters::ReadLevelCsvSettings(
      Json::ReadObjectField(SettingsSource(Sources, "Level"),
                            "LevelCsv"));
  Settings.LevelDataSources = LevelSettingsAdapters::
      ReadLevelDataSourceSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Level"), "LevelDataSources"));
  Settings.Validation = LevelSettingsAdapters::
      ReadLevelValidationSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Level"), "Validation"));
  Settings.LevelGeometry = LevelSettingsAdapters::ReadLevelGeometrySettings(
      Json::ReadObjectField(SettingsSource(Sources, "Level"),
                            "LevelGeometry"));
  Settings.RenderingAssets = RenderingSettingsAdapters::
      ReadRenderingAssetPathSettings(Json::ReadObjectField(
          SettingsSource(Sources, "RenderingAssets"),
          "RenderingAssets"));
  Settings.RenderingProfile = RenderingSettingsAdapters::
      ReadRenderingProfileSettings(Json::ReadObjectField(
          SettingsSource(Sources, "RenderingProfile"),
          "RenderingProfile"));
  Settings.TextureCatalog = RenderingSettingsAdapters::
      ReadTextureCatalogSettings(SettingsSource(Sources,
                                                       "TextureCatalog"));
  Settings.Rendering =
      RenderingSettingsAdapters::ReadRenderingSettings({
          Json::ReadObjectField(SettingsSource(Sources,
                                                      "TextureSettings"),
                                "TextureSettings"),
          Json::ReadObjectField(SettingsSource(Sources,
                                                      "ConsoleSettings"),
                                "ConsoleSettings"),
          SettingsSource(Sources, "ConsoleVariables"),
          SettingsSource(Sources, "TexturePalettes")});
  Settings.RenderingDistanceLod =
      RenderingSettingsAdapters::ReadRenderingDistanceLodSettings(
          Json::ReadObjectField(SettingsSource(
                                    Sources, "RenderingDistanceLod"),
                                "DistanceLod"));
  Settings.Dialogue =
      Json::ReadSettingsWith<FDialogueSettings>(
          JSON_SETTINGS_ATOMS(ReplyPayloadIdFormat))(
          Json::ReadObjectField(SettingsSource(Sources, "Dialogue"),
                                "Dialogue"));
  Settings.Bot = Json::ReadSettingsWith<FBotSettings>(
      JSON_SETTINGS_ATOMS(
          InitialName, InitialHealth, InitialMaxHealth, InitialMana,
          InitialMaxMana, InitialStamina, InitialMaxStamina, MinimumHealth,
          InitialPosition, InitialRotation, InitialKnownPlayerPosition,
          InitialTimeSinceSeenPlayer,
          EnemySpottedTimeSinceSeenPlayer, bInitialHasAggro,
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
      Json::ReadObjectField(SettingsSource(Sources, "Bots"),
                            "Bot"));
  Settings.TownspersonPresentation = BotAdapters::
      ReadTownspersonPresentationSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Bots"),
          "TownspersonPresentation"));
  Settings.HorsePresentation = BotAdapters::
      ReadHorsePresentationSettings(Json::ReadObjectField(
          SettingsSource(Sources, "Bots"), "HorsePresentation"));
  Settings.ObservationIds =
      Json::ReadSettingsWith<FObservationIdSettings>(
          JSON_SETTINGS_ATOMS(PlayerPresentationRequested,
                              PlayerMovementInputObserved,
                              TownspersonPresentationRequested,
                              HorsePresentationRequested,
                              TownspersonCandidatesObserved))(
          SettingsSource(Sources, "ObservationIds"));
  Settings.DebugMessages =
      Json::ReadSettingsWith<FDebugMessageSettings>(
          JSON_SETTINGS_ATOMS(OnScreenKey, DurationSeconds, Color))(
          SettingsSource(Sources, "DebugMessages"));
  Settings.ViewNames =
      Json::ReadSettingsWith<FViewNameSettings>(
          JSON_SETTINGS_ATOMS(SceneRoot, TownspersonCharacterMesh,
                              TownspersonInteractionSphere,
                              TownspersonNameText, TownspersonPromptText,
                              TownspersonDialogueText,
                              TownspersonOverlapProfile, HorseImportedMesh,
                              HorseMountedRiderMesh, HorseNameText,
                              PlayerCameraBoom, PlayerFollowCamera,
                              SpeechPresentationMesh, SpeechComponent))(
          SettingsSource(Sources, "ViewNames"));
  Settings.Text = Json::ReadSettingsWith<FTextSettings>(
      JSON_SETTINGS_ATOMS(TownspersonNameRoleFormat, NpcReplyLog,
                          TownspersonMissingMesh,
                          TownspersonMissingAnimation, HorseMissingMesh,
                          HorseMissingWalkAnimation,
                          RiderMissingWalkAnimation, RiderMissingMesh,
                          StartupSdkEnabled, StartupSdkDisabled))(
      SettingsSource(Sources, "Text"));
  Settings.ReduxLog =
      Json::ReadSettingsWith<FReduxLogSettings>(
          JSON_SETTINGS_ATOMS(SampleInterval, SampledActionTypes))(
          SettingsSource(Sources, "ReduxLog"));
  Settings.Ecs = Json::ReadSettingsWith<FEcsSettings>(
      JSON_SETTINGS_ATOMS(DomainRegistry))(
      SettingsSource(Sources, "Ecs"));
  Settings.Automation =
      Json::ReadSettingsWith<Automation::FSettings>(
          JSON_SETTINGS_ATOMS(Store, Tests, ContentAssets, RtkCompliance, Bot,
                              Pipeline,
                              ConversationUI, ProtocolLoop))(
          SettingsSource(Sources, "Automation"));
  Settings.UI = Json::ReadSettingsWith<FUISettings>(
      JSON_SETTINGS_ATOMS(
          PlayerRoleLabel, SystemRoleLabel, NpcRoleLabel, UnknownRoleLabel,
          ChatMessageFormat, HistoryRoleSeparator, HistoryTextStartOffset,
          HistoryMinimumRoleIndex, PlaceholderTitle, PlaceholderPlayerLine,
          PlaceholderNpcReply, ConversationTitleFormat, PlayerLineFormat,
          NpcReplyFormat, ConnectionMessageFormat, UnboundDialogueError,
          PayloadIdFormat, PanelColor, TitleColor, PlayerColor, SystemColor,
          NpcColor, UnknownColor, ReplyColor, PanelPadding, TitleSize,
          BodySize, StatsOverlay, MarketingCapture, FlyMode,
          ScaleAuditCapture))(
      SettingsSource(Sources, "UI"));
  Settings.Speech = Json::ReadSettingsWith<FSpeechSettings>(
      JSON_SETTINGS_ATOMS(
          RestViseme, RestWeight, SpeechRate, Volume, bEnableLipSync,
          bCanEverTick, bStartTickEnabled, InitialPlaybackTime,
          bInitialSpeechActive, EstimatedBasePhonemeSeconds, VowelPhonemes,
          SilenceCharacters, SilencePhoneme, VisemeChangeTolerance,
          TtsEndpoint, TtsVerb, TtsContentTypeHeader, TtsContentType,
          TtsRequestFormat, TtsSuccessResponseCode, MinimumAudioBytes,
          SpeechStartLogFormat, SpeechAudioReceivedLogFormat,
          ResetMorphTargets, VisemeMappings, DurationRules))(
      SettingsSource(Sources, "Speech"));
  return Settings;
}

/** User Story: As a components data settings consumer, I need to invoke load settings through a stable signature so the components data settings workflow remains explicit and composable. @fn FSettings LoadSettings() */
FSettings LoadSettings() {
  return ReadSettings(Json::LoadRequiredObjectFromContent(
      {TEXT("Data/settings.json")}));
}

} // namespace SettingsAdapters
} // namespace Data
} // namespace Game
} // namespace ForbocAI
