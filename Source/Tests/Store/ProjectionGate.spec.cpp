#include "Tests/Store/StoreSpecSupport.h"

#include "Features/Systems/Projection/ProjectionSlice.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreProjectionGate,
    "ForbocAI.Runtime.Store.ProjectionGate",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @fn bool FStoreProjectionGate::RunTest(const FString &Parameters)
 * @brief Verifies high-frequency patrol observation actions do not rebuild ECS.
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when projected ECS input actions are gated separately from view-only patrol calculations.
 *
 * User Story: As a runtime maintainer, I need RTK reducers to update hot
 * observation state without rebuilding ECS unless the dispatched action mutates
 * a slice selected by the ECS projection.
 */
bool FStoreProjectionGate::RunTest(const FString &Parameters) {
  (void)Parameters;

  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  FLabelCursor Labels = StoreLabels(
      Settings.Automation.Store.Labels.ProjectionGateLabels);
  FBotPositionMoved MovePayload;
  FBotPatrolAdvanceRequest PatrolRequest;
  TestTrue(Labels.Next(),
           RuntimeReducers::ShouldProjectRuntimeAction(
               BotPositionActions::BotPositionMoved()(MovePayload)));
  TestFalse(Labels.Next(),
            RuntimeReducers::ShouldProjectRuntimeAction(
                BotPositionActions::PatrolAdvanceObserved()(PatrolRequest)));

  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const int64 ProjectedGeneration =
      RuntimeSelectors::SelectWorld(EnhancedStoreValue.getState()).Generation;
  EnhancedStoreValue.dispatch(
      BotPositionActions::PatrolAdvanceObserved()(PatrolRequest));
  const int64 ObservedGeneration =
      RuntimeSelectors::SelectWorld(EnhancedStoreValue.getState()).Generation;
  TestEqual(Labels.Next(),
            ObservedGeneration, ProjectedGeneration);

  return true;
}
