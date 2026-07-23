#include "Features/Systems/SystemsSelectors.h"

#include "Features/Systems/Level/SystemsLevelSelectors.h"

namespace ForbocAI::Game::Level::RuntimeSelectors {

/** User Story: As a runtime state consumer, I need to select player state through a stable signature so root-store reads stay explicit and composable. @fn const FPlayerState &SelectPlayerState(const FRuntimeState &State) */
const FPlayerState &SelectPlayerState(const FRuntimeState &State) {
  return State.Player;
}

/** User Story: As a runtime state consumer, I need to select systems state through a stable signature so root-store reads stay explicit and composable. @fn const FSystemsState &SelectSystemsState(const FRuntimeState &State) */
const FSystemsState &SelectSystemsState(const FRuntimeState &State) {
  return State.Systems;
}

/** User Story: As a runtime state consumer, I need to select level state through a stable signature so root-store reads stay explicit and composable. @fn const FSystemState &SelectLevelState(const FRuntimeState &State) */
const FSystemState &SelectLevelState(const FRuntimeState &State) {
  return State.Level;
}

/** User Story: As a runtime state consumer, I need to select terrain source settings through a stable signature so level configuration reads stay explicit and composable. @fn const ForbocAI::Game::Data::FSourceSettings & SelectLevelTerrainSources(const FRuntimeState &State) */
const ForbocAI::Game::Data::FSourceSettings &
SelectLevelTerrainSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectTerrainSources(State.Level);
}

/** User Story: As a runtime state consumer, I need to select CSV settings through a stable signature so level data reads stay explicit and composable. @fn const ForbocAI::Game::Data::FCsvSettings & SelectLevelCsv(const FRuntimeState &State) */
const ForbocAI::Game::Data::FCsvSettings &
SelectLevelCsv(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectCsv(State.Level);
}

/** User Story: As a runtime state consumer, I need to select data source settings through a stable signature so level configuration reads stay explicit and composable. @fn const ForbocAI::Game::Data::FDataSourceSettings & SelectLevelDataSources(const FRuntimeState &State) */
const ForbocAI::Game::Data::FDataSourceSettings &
SelectLevelDataSources(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectDataSources(State.Level);
}

/** User Story: As a runtime state consumer, I need to select geometry settings through a stable signature so level layout reads stay explicit and composable. @fn const ForbocAI::Game::Data::FGeometrySettings & SelectLevelGeometry(const FRuntimeState &State) */
const ForbocAI::Game::Data::FGeometrySettings &
SelectLevelGeometry(const FRuntimeState &State) {
  return LevelSystemSelectors::SelectGeometry(State.Level);
}

/** User Story: As a runtime state consumer, I need to select current geometry settings through a stable signature so callers use the root store consistently. @fn const ForbocAI::Game::Data::FGeometrySettings &SelectLevelGeometry() */
const ForbocAI::Game::Data::FGeometrySettings &SelectLevelGeometry() {
  return SelectLevelGeometry(SelectState());
}

/** User Story: As a runtime state consumer, I need to select rendering state through a stable signature so rendering reads stay explicit and composable. @fn const FRenderingState &SelectRenderingState(const FRuntimeState &State) */
const FRenderingState &SelectRenderingState(const FRuntimeState &State) {
  return State.Rendering;
}

/** User Story: As a runtime state consumer, I need to select dialogue state through a stable signature so conversation reads stay explicit and composable. @fn const FDialogueState &SelectDialogueState(const FRuntimeState &State) */
const FDialogueState &SelectDialogueState(const FRuntimeState &State) {
  return State.Dialogue;
}

/** User Story: As a runtime state consumer, I need to select interaction state through a stable signature so interaction reads stay explicit and composable. @fn const FInteractionState &SelectInteractionState(const FRuntimeState &State) */
const FInteractionState &SelectInteractionState(const FRuntimeState &State) {
  return State.Interaction;
}

/** User Story: As a runtime state consumer, I need to select speech state through a stable signature so speech reads stay explicit and composable. @fn const FSpeechState &SelectSpeechState(const FRuntimeState &State) */
const FSpeechState &SelectSpeechState(const FRuntimeState &State) {
  return State.Speech;
}

/** User Story: As a runtime state consumer, I need to select UI state through a stable signature so presentation reads stay explicit and composable. @fn const FUIState &SelectUIState(const FRuntimeState &State) */
const FUIState &SelectUIState(const FRuntimeState &State) {
  return State.UI;
}

/** User Story: As a runtime state consumer, I need to select terrain state through a stable signature so terrain reads stay explicit and composable. @fn const FTerrainState &SelectTerrainState(const FRuntimeState &State) */
const FTerrainState &SelectTerrainState(const FRuntimeState &State) {
  return State.Terrain;
}

/** User Story: As a runtime state consumer, I need to select spawn state through a stable signature so spawn reads stay explicit and composable. @fn const FSpawnState &SelectSpawnState(const FRuntimeState &State) */
const FSpawnState &SelectSpawnState(const FRuntimeState &State) {
  return State.Spawn;
}

} // namespace ForbocAI::Game::Level::RuntimeSelectors
