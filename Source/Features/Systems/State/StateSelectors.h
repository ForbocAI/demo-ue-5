#pragma once

namespace ForbocAI {
namespace Game {
namespace Level {

namespace {

using FTownspersonViewDefaultsInputSelector =
    std::function<FTownspersonViewDefaults(const FRuntimeState &)>;

const std::function<FTownspersonViewDefaults(const FRuntimeState &)> &
TownspersonViewDefaultsSelector() {
  static const auto Selector =
      rtk::createSelector<FRuntimeState, FTownspersonViewDefaults>(
          std::make_tuple(FTownspersonViewDefaultsInputSelector(
              [](const FRuntimeState &State) -> FTownspersonViewDefaults {
                return TownspersonReducers::ReduceViewDefaults(
                    State.TownspersonDefaults);
              })),
          std::function<FTownspersonViewDefaults(FTownspersonViewDefaults)>(
              [](FTownspersonViewDefaults Defaults)
                  -> FTownspersonViewDefaults {
                return Defaults;
              }));
  return Selector;
}

} // namespace

namespace RuntimeSelectors {

const FPlayerState &SelectPlayerState(const FRuntimeState &State) {
  return State.Player;
}

const FSystemsState &SelectSystemsState(const FRuntimeState &State) {
  return State.Systems;
}

const FSystemState &SelectLevelState(const FRuntimeState &State) {
  return State.Level;
}

const ForbocAI::Game::Data::FTerrainSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectTerrainSources(State.Level);
}

const ForbocAI::Game::Data::FCsvSettings &
SelectLevelCsv(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectCsv(State.Level);
}

const ForbocAI::Game::Data::FDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectDataSources(State.Level);
}

const ForbocAI::Game::Data::FGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectGeometry(State.Level);
}

const ForbocAI::Game::Data::FGeometrySettings &SelectLevelGeometry() {
  return SelectLevelGeometry(SelectState());
}

const FRenderingState &SelectRenderingState(const FRuntimeState &State) {
  return State.Rendering;
}

const FDialogueState &SelectDialogueState(const FRuntimeState &State) {
  return State.Dialogue;
}

const FInteractionState &SelectInteractionState(const FRuntimeState &State) {
  return State.Interaction;
}

const FSpeechState &SelectSpeechState(const FRuntimeState &State) {
  return State.Speech;
}

const FUIState &SelectUIState(const FRuntimeState &State) {
  return State.UI;
}

const FTerrainState &SelectTerrainState(const FRuntimeState &State) {
  return State.Terrain;
}

const FSpawnState &SelectSpawnState(const FRuntimeState &State) {
  return State.Spawn;
}

bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
}

TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) {
  return LandmarkSelectors::SelectAll(State.Landmarks);
}

func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id) {
  return LandmarkSelectors::SelectById(State.Landmarks, Id);
}

FPointPayload SelectPlayerSpawn(const FRuntimeState &State) {
  return SpawnSelectors::SelectPlayerSpawn(State.Spawn);
}

TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectAll(State.Townspeople);
}

func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id) {
  return TownspersonSelectors::SelectById(State.Townspeople, Id);
}

const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectDialogueTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectMemoryTownspeople(State.Townspeople);
}

const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectCombatValidationTownspeople(
      State.Townspeople);
}

TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

TArray<FFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State) {
  return State.TownspersonInteractionRequest;
}

const FSelection &SelectInteractionSelection(
    const FRuntimeState &State) {
  return InteractionSelectors::SelectSelectedCandidate(State.Interaction);
}

const FSelection &SelectInteractionSelection() {
  return SelectInteractionSelection(SelectState());
}

float SelectTownspersonInteractionDistance(const FRuntimeState &State) {
  return InteractionSelectors::SelectTownspersonInteractionDistance(
      State.Interaction);
}

float SelectTownspersonInteractionDistance() {
  return SelectTownspersonInteractionDistance(SelectState());
}

const FPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State) {
  return PlayerSelectors::SelectPresentation(State.Player);
}

const FPresentationViewModel &SelectPlayerPresentation() {
  return SelectPlayerPresentation(SelectState());
}

const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeProfile(State.Rendering);
}

const FLevelRetroRenderProfile &SelectRuntimeProfile() {
  return SelectRuntimeProfile(SelectState());
}

const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State) {
  return RenderingSelectors::SelectTextureCatalog(State.Rendering);
}

const TArray<FLevelRetroTextureSpec> &SelectTextureCatalog() {
  return SelectTextureCatalog(SelectState());
}

const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRuntimeState &State) {
  return RenderingSelectors::SelectDistanceLodStages(State.Rendering);
}

const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingSettings(State.Rendering);
}

const ForbocAI::Game::Data::FRenderingSettings &SelectRenderingSettings() {
  return SelectRenderingSettings(SelectState());
}

const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingAssetPaths(State.Rendering);
}

const FRenderingAssetPaths &SelectRenderingAssetPaths() {
  return SelectRenderingAssetPaths(SelectState());
}

int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return BotPositionReducers::ReduceInitialPatrolIndex(PatrolRoute);
}

FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  return BotPositionReducers::ReduceInitialPatrolLocation(Request);
}

FBotPatrolAdvancePayload
SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  return BotPositionReducers::ReducePatrolAdvance(Request);
}

FMovementInputViewModel
SelectPlayerMovementInput(const FMovementInputRequest &Request) {
  return PlayerReducers::ReduceMovementInput(Request);
}

FVector SelectPostOfficeWorldCenter(const FRuntimeState &State) {
  const ForbocAI::Game::Data::FGeometrySettings &Geometry =
      RuntimeSelectors::SelectLevelGeometry(State);
  const FLevelLocalPoint Point =
      LevelLayoutAdapters::FromPostOfficeLots({Geometry, float(), float(),
                                            float()});
  return FVector(Point.EastWest, Point.NorthSouth, float());
}

FVector SelectPostOfficeWorldCenter() {
  return SelectPostOfficeWorldCenter(SelectState());
}

FTownspersonViewDefaults
SelectTownspersonViewDefaults(const FRuntimeState &State) {
  return TownspersonViewDefaultsSelector()(State);
}

FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FTownspersonViewDefaultsRequest &Request) {
  return SelectTownspersonViewDefaults(SelectState(), Request);
}
