#include "Features/Systems/SystemsSelectors.h"

#include "Features/Components/Spatial/Level/Layout/SpatialLevelLayoutAdapters.h"
#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "Features/Entities/Characters/Player/PlayerSlice.h"
#include "Features/Systems/Bots/Horses/HorsesSelectors.h"
#include "Features/Systems/Bots/Position/PositionSlice.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSelectors.h"
#include "Features/Systems/Bots/Townspeople/TownspeopleSlice.h"
#include "Features/Systems/Interaction/InteractionSelectors.h"
#include "Features/Systems/Landmarks/SystemsLandmarksSelectors.h"
#include "Features/Systems/Level/SystemsLevelSelectors.h"
#include "Features/Systems/Nature/SystemsNatureSelectors.h"
#include "Features/Systems/Rendering/SystemsRenderingSelectors.h"
#include "Features/Systems/Spawn/SpawnSelectors.h"
#include "Features/Systems/Terrain/TerrainSelectors.h"

namespace ForbocAI {
namespace Game {
namespace Level {

namespace {

using FTownspersonViewDefaultsInputSelector =
    std::function<FTownspersonViewDefaults(const FRuntimeState &)>;

/** User Story: As a features systems state consumer, I need to invoke townsperson view defaults selector through a stable signature so the features systems state workflow remains explicit and composable. @fn const std::function<FTownspersonViewDefaults(const FRuntimeState &)> & TownspersonViewDefaultsSelector() */
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

/** User Story: As a features systems state consumer, I need to invoke select terrain loaded through a stable signature so the features systems state workflow remains explicit and composable. @fn bool SelectTerrainLoaded(const FRuntimeState &State) */
bool SelectTerrainLoaded(const FRuntimeState &State) {
  return TerrainSelectors::SelectLoaded(State.Terrain);
}

/** User Story: As a features systems state consumer, I need to invoke select landmarks through a stable signature so the features systems state workflow remains explicit and composable. @fn TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) */
TArray<FLandmark> SelectLandmarks(const FRuntimeState &State) {
  return LandmarkSelectors::SelectAll(State.Landmarks);
}

/** User Story: As a features systems state consumer, I need to invoke select landmark by id through a stable signature so the features systems state workflow remains explicit and composable. @fn func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State, const FString &Id) */
func::Maybe<FLandmark> SelectLandmarkById(const FRuntimeState &State,
                                          const FString &Id) {
  return LandmarkSelectors::SelectById(State.Landmarks, Id);
}

/** User Story: As a features systems state consumer, I need to invoke select player spawn through a stable signature so the features systems state workflow remains explicit and composable. @fn FPointPayload SelectPlayerSpawn(const FRuntimeState &State) */
FPointPayload SelectPlayerSpawn(const FRuntimeState &State) {
  return SpawnSelectors::SelectPlayerSpawn(State.Spawn);
}

/** User Story: As a features systems state consumer, I need to invoke select townspeople through a stable signature so the features systems state workflow remains explicit and composable. @fn TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) */
TArray<FTownspersonSeed> SelectTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectAll(State.Townspeople);
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson by id through a stable signature so the features systems state workflow remains explicit and composable. @fn func::Maybe<FTownspersonSeed> SelectTownspersonById( const FRuntimeState &State, const FString &Id) */
func::Maybe<FTownspersonSeed> SelectTownspersonById(
    const FRuntimeState &State, const FString &Id) {
  return TownspersonSelectors::SelectById(State.Townspeople, Id);
}

/** User Story: As a features systems state consumer, I need to invoke select dialogue townspeople through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectDialogueTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectDialogueTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectDialogueTownspeople(State.Townspeople);
}

/** User Story: As a features systems state consumer, I need to invoke select memory townspeople through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectMemoryTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectMemoryTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectMemoryTownspeople(State.Townspeople);
}

/** User Story: As a features systems state consumer, I need to invoke select combat validation townspeople through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FTownspersonSeed> & SelectCombatValidationTownspeople(const FRuntimeState &State) */
const TArray<FTownspersonSeed> &
SelectCombatValidationTownspeople(const FRuntimeState &State) {
  return TownspersonSelectors::SelectCombatValidationTownspeople(
      State.Townspeople);
}

/** User Story: As a features systems state consumer, I need to invoke select horses through a stable signature so the features systems state workflow remains explicit and composable. @fn TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) */
TArray<FHorseRouteSeed> SelectHorses(const FRuntimeState &State) {
  return HorseSelectors::SelectAll(State.Horses);
}

/** User Story: As a features systems state consumer, I need to invoke select nature features through a stable signature so the features systems state workflow remains explicit and composable. @fn TArray<FFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) */
TArray<FFeatureSeed> SelectNatureFeatures(const FRuntimeState &State) {
  return NatureSelectors::SelectAll(State.Nature);
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson interaction request through a stable signature so the features systems state workflow remains explicit and composable. @fn const FRuntimeTownspersonInteractionRequest & SelectTownspersonInteractionRequest(const FRuntimeState &State) */
const FRuntimeTownspersonInteractionRequest &
SelectTownspersonInteractionRequest(const FRuntimeState &State) {
  return State.TownspersonInteractionRequest;
}

/** User Story: As a features systems state consumer, I need to invoke select interaction selection through a stable signature so the features systems state workflow remains explicit and composable. @fn const FSelection &SelectInteractionSelection( const FRuntimeState &State) */
const FSelection &SelectInteractionSelection(
    const FRuntimeState &State) {
  return InteractionSelectors::SelectSelectedCandidate(State.Interaction);
}

/** User Story: As a features systems state consumer, I need to invoke select interaction selection through a stable signature so the features systems state workflow remains explicit and composable. @fn const FSelection &SelectInteractionSelection() */
const FSelection &SelectInteractionSelection() {
  return SelectInteractionSelection(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson interaction distance through a stable signature so the features systems state workflow remains explicit and composable. @fn float SelectTownspersonInteractionDistance(const FRuntimeState &State) */
float SelectTownspersonInteractionDistance(const FRuntimeState &State) {
  return InteractionSelectors::SelectTownspersonInteractionDistance(
      State.Interaction);
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson interaction distance through a stable signature so the features systems state workflow remains explicit and composable. @fn float SelectTownspersonInteractionDistance() */
float SelectTownspersonInteractionDistance() {
  return SelectTownspersonInteractionDistance(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select player presentation through a stable signature so the features systems state workflow remains explicit and composable. @fn const FPresentationViewModel & SelectPlayerPresentation(const FRuntimeState &State) */
const FPresentationViewModel &
SelectPlayerPresentation(const FRuntimeState &State) {
  return PlayerSelectors::SelectPresentation(State.Player);
}

/** User Story: As a features systems state consumer, I need to invoke select player presentation through a stable signature so the features systems state workflow remains explicit and composable. @fn const FPresentationViewModel &SelectPlayerPresentation() */
const FPresentationViewModel &SelectPlayerPresentation() {
  return SelectPlayerPresentation(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select runtime profile through a stable signature so the features systems state workflow remains explicit and composable. @fn const FLevelRetroRenderProfile & SelectRuntimeProfile(const FRuntimeState &State) */
const FLevelRetroRenderProfile &
SelectRuntimeProfile(const FRuntimeState &State) {
  return RenderingSelectors::SelectRuntimeProfile(State.Rendering);
}

/** User Story: As a features systems state consumer, I need to invoke select runtime profile through a stable signature so the features systems state workflow remains explicit and composable. @fn const FLevelRetroRenderProfile &SelectRuntimeProfile() */
const FLevelRetroRenderProfile &SelectRuntimeProfile() {
  return SelectRuntimeProfile(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select texture catalog through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> & SelectTextureCatalog(const FRuntimeState &State) */
const TArray<FLevelRetroTextureSpec> &
SelectTextureCatalog(const FRuntimeState &State) {
  return RenderingSelectors::SelectTextureCatalog(State.Rendering);
}

/** User Story: As a features systems state consumer, I need to invoke select texture catalog through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FLevelRetroTextureSpec> &SelectTextureCatalog() */
const TArray<FLevelRetroTextureSpec> &SelectTextureCatalog() {
  return SelectTextureCatalog(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select distance lod stages through a stable signature so the features systems state workflow remains explicit and composable. @fn const TArray<FLevelDistanceLodStage> & SelectDistanceLodStages(const FRuntimeState &State) */
const TArray<FLevelDistanceLodStage> &
SelectDistanceLodStages(const FRuntimeState &State) {
  return RenderingSelectors::SelectDistanceLodStages(State.Rendering);
}

/** User Story: As a features systems state consumer, I need to invoke select rendering settings through a stable signature so the features systems state workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings & SelectRenderingSettings(const FRuntimeState &State) */
const ForbocAI::Game::Data::FRenderingSettings &
SelectRenderingSettings(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingSettings(State.Rendering);
}

/** User Story: As a features systems state consumer, I need to invoke select rendering settings through a stable signature so the features systems state workflow remains explicit and composable. @fn const ForbocAI::Game::Data::FRenderingSettings &SelectRenderingSettings() */
const ForbocAI::Game::Data::FRenderingSettings &SelectRenderingSettings() {
  return SelectRenderingSettings(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select rendering asset paths through a stable signature so the features systems state workflow remains explicit and composable. @fn const FRenderingAssetPaths & SelectRenderingAssetPaths(const FRuntimeState &State) */
const FRenderingAssetPaths &
SelectRenderingAssetPaths(const FRuntimeState &State) {
  return RenderingSelectors::SelectRenderingAssetPaths(State.Rendering);
}

/** User Story: As a features systems state consumer, I need to invoke select rendering asset paths through a stable signature so the features systems state workflow remains explicit and composable. @fn const FRenderingAssetPaths &SelectRenderingAssetPaths() */
const FRenderingAssetPaths &SelectRenderingAssetPaths() {
  return SelectRenderingAssetPaths(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select bot initial patrol index through a stable signature so the features systems state workflow remains explicit and composable. @fn int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute) */
int32 SelectBotInitialPatrolIndex(const TArray<FVector> &PatrolRoute) {
  return BotPositionReducers::ReduceInitialPatrolIndex(PatrolRoute);
}

/** User Story: As a features systems state consumer, I need to invoke select bot initial patrol location through a stable signature so the features systems state workflow remains explicit and composable. @fn FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation( const FBotInitialPatrolLocationRequest &Request) */
FBotInitialPatrolLocationPayload SelectBotInitialPatrolLocation(
    const FBotInitialPatrolLocationRequest &Request) {
  return BotPositionReducers::ReduceInitialPatrolLocation(Request);
}

/** User Story: As a features systems state consumer, I need to invoke select bot patrol advance through a stable signature so the features systems state workflow remains explicit and composable. @fn FBotPatrolAdvancePayload SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request) */
FBotPatrolAdvancePayload
SelectBotPatrolAdvance(const FBotPatrolAdvanceRequest &Request) {
  return BotPositionReducers::ReducePatrolAdvance(Request);
}

/** User Story: As a features systems state consumer, I need to invoke select player movement input through a stable signature so the features systems state workflow remains explicit and composable. @fn FMovementInputViewModel SelectPlayerMovementInput(const FMovementInputRequest &Request) */
FMovementInputViewModel
SelectPlayerMovementInput(const FMovementInputRequest &Request) {
  return PlayerReducers::ReduceMovementInput(Request);
}

/** User Story: As a features systems state consumer, I need to invoke select post office world center through a stable signature so the features systems state workflow remains explicit and composable. @fn FVector SelectPostOfficeWorldCenter(const FRuntimeState &State) */
FVector SelectPostOfficeWorldCenter(const FRuntimeState &State) {
  const ForbocAI::Game::Data::FGeometrySettings &Geometry =
      RuntimeSelectors::SelectLevelGeometry(State);
  const FLevelLocalPoint Point =
      LevelLayoutAdapters::FromPostOfficeLots({Geometry, float(), float(),
                                            float()});
  return FVector(Point.EastWest, Point.NorthSouth, float());
}

/** User Story: As a features systems state consumer, I need to invoke select post office world center through a stable signature so the features systems state workflow remains explicit and composable. @fn FVector SelectPostOfficeWorldCenter() */
FVector SelectPostOfficeWorldCenter() {
  return SelectPostOfficeWorldCenter(SelectState());
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems state workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults(const FRuntimeState &State) */
FTownspersonViewDefaults
SelectTownspersonViewDefaults(const FRuntimeState &State) {
  return TownspersonViewDefaultsSelector()(State);
}

/** User Story: As a features systems state consumer, I need to invoke select townsperson view defaults through a stable signature so the features systems state workflow remains explicit and composable. @fn FTownspersonViewDefaults SelectTownspersonViewDefaults( const FTownspersonViewDefaultsRequest &Request) */
FTownspersonViewDefaults SelectTownspersonViewDefaults(
    const FTownspersonViewDefaultsRequest &Request) {
  return SelectTownspersonViewDefaults(SelectState(), Request);
}

} // namespace RuntimeSelectors
} // namespace Level
} // namespace Game
} // namespace ForbocAI
