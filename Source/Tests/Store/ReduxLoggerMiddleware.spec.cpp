#include "Tests/Store/StoreSpecSupport.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreReduxLoggerMiddleware,
    "ForbocAI.Runtime.Store.ReduxLoggerMiddleware",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * @fn bool FStoreReduxLoggerMiddleware::RunTest(const FString &Parameters)
 * @brief Verifies the SDK redux-logger middleware writes RTK dispatch logs through UE output.
 * @param Parameters Automation test parameters supplied by Unreal.
 * @return true when a dispatched action is captured from the UE log output path.
 *
 * User Story: As a developer debugging tests or runtime play, I need RTK
 * actions to be visible in automation logs and the UE console from one store.
 */
bool FStoreReduxLoggerMiddleware::RunTest(const FString &Parameters) {
  (void)Parameters;

  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels = StoreLabels(
      StoreAutomation.Labels.ReduxLoggerMiddlewareLabels);
  TArray<FString> CapturedLines;
  FReduxLoggerCaptureDevice CaptureDevice(CapturedLines,
                                          StoreAutomation.Logger.ReduxLoggerCategory);

  GLog->AddOutputDevice(&CaptureDevice);
  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const rtk::AnyAction HydratedAction = RuntimeActions::RuntimeHydrated()(
      RuntimeFactories::CreateInitialState());
  EnhancedStoreValue.dispatch(HydratedAction);
  GLog->FlushThreadedLogs();
  GLog->RemoveOutputDevice(&CaptureDevice);

  TestTrue(Labels.Next(),
           ContainsReduxLoggerLine(
               {&CapturedLines,
                ReduxLoggerActionTitleNeedle(
                    StoreAutomation.Logger.ReduxLoggerActionTitlePrefix,
                    HydratedAction),
                int32{}}));
  TestTrue(Labels.Next(),
           ContainsReduxLoggerLine({&CapturedLines, HydratedAction.Type,
                                    int32{}}));

  return true;
}
