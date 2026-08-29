#include "Tests/Store/StoreSpecSupport.h"
#include "Features/Components/AuthoredValues/AuthoredValuesTypes.h"
#include "Features/Systems/Rendering/Stats/Sampling/SamplingSelectors.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreDataBackedMapRendering,
    FORBOCAI_DEMOUE5_AUTHORED_STRINGV57C5974FB672,
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/** User Story: As a tests store consumer, I need to invoke run test through a stable signature so the tests store workflow remains explicit and composable. @fn bool FStoreDataBackedMapRendering::RunTest(const FString &Parameters) */
bool FStoreDataBackedMapRendering::RunTest(const FString &Parameters) {
  (void)Parameters;
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels = StoreLabels(
      StoreAutomation.Labels.DataBackedMapLabels);
  rtk::EnhancedStore<ForbocAI::Game::Level::FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const ForbocAI::Game::Level::FRuntimeState &State = EnhancedStoreValue.getState();

  const FLevelRetroRenderProfile &RetroProfile =
      RenderingSelectors::SelectRuntimeProfile(
          RuntimeSelectors::SelectRenderingState(State));
  const ForbocAI::Game::Data::FProfileSettings &ExpectedProfile =
      Settings.RenderingProfile;
  TestEqual(Labels.Next(),
            RetroProfile.Time.TimeOfDayHour,
            ExpectedProfile.Time.TimeOfDayHour);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcess.Grade.PostProcessSaturationMultiplier,
            ExpectedProfile.PostProcess.Grade.PostProcessSaturationMultiplier);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcess.Grade.PostProcessContrastMultiplier,
            ExpectedProfile.PostProcess.Grade.PostProcessContrastMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.Lighting.Directional.Color.DirectionalLightColorR >=
               RetroProfile.Lighting.Directional.Color.DirectionalLightColorB);
  TestEqual(Labels.Next(),
            RetroProfile.Output.AntiAliasing.AntiAliasingMethod,
            ExpectedProfile.Output.AntiAliasing.AntiAliasingMethod);
  TestEqual(Labels.Next(),
            RetroProfile.Output.AntiAliasing.ScreenPercentage,
            ExpectedProfile.Output.AntiAliasing.ScreenPercentage);
  TestEqual(Labels.Next(),
            RetroProfile.Output.Resolution.InternalRenderWidth,
            ExpectedProfile.Output.Resolution.InternalRenderWidth);
  TestEqual(Labels.Next(),
            RetroProfile.Output.Resolution.InternalRenderHeight,
            ExpectedProfile.Output.Resolution.InternalRenderHeight);
  TestEqual(Labels.Next(),
            RetroProfile.Output.Resolution.OutputScaleMultiplier,
            ExpectedProfile.Output.Resolution.OutputScaleMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.Output.Resolution.bFullscreenOutput ==
               ExpectedProfile.Output.Resolution.bFullscreenOutput);
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(RetroProfile.Scalability.ViewDistanceScale,
                                ExpectedProfile.Scalability.ViewDistanceScale));
  TestTrue(Labels.Next(),
           RetroProfile.Sky.Dome.Geometry.bSkyDomeEnabled ==
                   ExpectedProfile.Sky.Dome.Geometry.bSkyDomeEnabled &&
               RetroProfile.Sky.Dome.Geometry.SkyDomeMeshPath ==
                   ExpectedProfile.Sky.Dome.Geometry.SkyDomeMeshPath &&
               RetroProfile.Sky.Dome.Geometry.SkyDomeMaterialPath ==
                   ExpectedProfile.Sky.Dome.Geometry.SkyDomeMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.Sky.Dome.HorizonColor.SkyDomeHorizonColorB >
                   RetroProfile.Sky.Dome.ZenithColor.SkyDomeZenithColorB &&
               FMath::IsNearlyEqual(
                   RetroProfile.Sky.Dome.Illumination.SkyDomeSkyBrightness,
                   ExpectedProfile.Sky.Dome.Illumination
                       .SkyDomeSkyBrightness));
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(
               RetroProfile.Sky.Dome.StarColor.SkyDomeStarColorR,
               ExpectedProfile.Sky.Dome.StarColor.SkyDomeStarColorR) &&
               FMath::IsNearlyEqual(
                   RetroProfile.Sky.Dome.StarColor.SkyDomeStarColorB,
                   ExpectedProfile.Sky.Dome.StarColor.SkyDomeStarColorB));
  TestTrue(Labels.Next(),
           RetroProfile.Sky.Moon.Geometry.bMoonDiscEnabled ==
                   ExpectedProfile.Sky.Moon.Geometry.bMoonDiscEnabled &&
               RetroProfile.Sky.Moon.Geometry.MoonDiscMeshPath ==
                   ExpectedProfile.Sky.Moon.Geometry.MoonDiscMeshPath &&
               RetroProfile.Sky.Moon.Geometry.MoonDiscMaterialPath ==
                   ExpectedProfile.Sky.Moon.Geometry.MoonDiscMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.Fog.State.bFogEnabled ==
                   ExpectedProfile.Fog.State.bFogEnabled &&
               FMath::IsNearlyEqual(RetroProfile.Fog.Shape.FogDensity,
                                    ExpectedProfile.Fog.Shape.FogDensity) &&
               FMath::IsNearlyEqual(RetroProfile.Fog.Shape.FogMaxOpacity,
                                    ExpectedProfile.Fog.Shape.FogMaxOpacity) &&
               FMath::IsNearlyEqual(RetroProfile.Fog.Color.FogColorB,
                                    ExpectedProfile.Fog.Color.FogColorB));
  TestFalse(Labels.Next(),
            RetroProfile.Fog.State.bVolumetricFogEnabled);

  const TArray<FLevelDistanceLodStage> &DistanceLodStages =
      RenderingSelectors::SelectDistanceLodStages(
          RuntimeSelectors::SelectRenderingState(State));
  const TArray<FLevelDistanceLodStage> ExpectedDistanceLodStages =
      RenderingDistanceLodReducers::ReduceDistanceLodStages(
          Settings.RenderingDistanceLod);
  TestEqual(Labels.Next(),
            DistanceLodStages.Num(), ExpectedDistanceLodStages.Num());
  const FString RuntimeLodStageLabel = Labels.Next();
  func::for_each_indexed(
      ExpectedDistanceLodStages,
      static_cast<size_t>(ExpectedDistanceLodStages.Num()),
      [this, &DistanceLodStages,
       &RuntimeLodStageLabel](const FLevelDistanceLodStage &ExpectedStage) {
        const FLevelDistanceLodStage *RuntimeStage =
            DistanceLodStages.FindByPredicate(
                [&ExpectedStage](const FLevelDistanceLodStage &Candidate) {
                  return Candidate.Selection.Id ==
                         ExpectedStage.Selection.Id;
                });
        TestTrue(RuntimeLodStageLabel,
                 RuntimeStage != nullptr && *RuntimeStage == ExpectedStage);
      });

  const ForbocAI::Game::Data::FOverlaySettings &StatsOverlay =
      RuntimeSelectors::SelectUISettings(State).StatsOverlay;
  const ForbocAI::Game::Data::FOverlaySettings
      &ExpectedStatsOverlay = Settings.UI.StatsOverlay;
  TestEqual(Labels.Next(),
            StatsOverlay.Measurement.Memory.MemoryBytesPerMegabyte,
            ExpectedStatsOverlay.Measurement.Memory.MemoryBytesPerMegabyte);
  TestEqual(Labels.Next(),
            StatsOverlay.Labels.Memory.UsedPhysicalMemoryLabel,
            ExpectedStatsOverlay.Labels.Memory.UsedPhysicalMemoryLabel);
  TestEqual(Labels.Next(),
            StatsOverlay.Labels.Memory.PeakPhysicalMemoryLabel,
            ExpectedStatsOverlay.Labels.Memory.PeakPhysicalMemoryLabel);

  const TArray<FLevelRetroTextureSpec> &TextureCatalog =
      RenderingSelectors::SelectTextureCatalog(
          RuntimeSelectors::SelectRenderingState(State));
  TestTrue(Labels.Next(),
           TextureCatalog.Num() >= Settings.TextureCatalog.Num());

  FLabelCursor SamplingLabels = StoreLabels(
      StoreAutomation.Labels.RuntimeStatsSamplingLabels);
  const FRenderingState &RenderingState =
      RuntimeSelectors::SelectRenderingState(State);
  const FRuntimeStatsSamplingPlan RetainedPlan =
      RenderingStatsSelectors::SelectRuntimeStatsSamplingPlan(
          {&RenderingState, &StatsOverlay,
           RenderingState.StatsClock.FrameClockSeconds,
           StatsOverlay.Refresh.BudgetScreenshotIntervalSeconds});
  TestTrue(
      SamplingLabels.Next(),
      RetainedPlan.Refresh.PolyCountMode ==
              ERuntimeStatsSampleMode::Retain &&
          RetainedPlan.Refresh.StatsMode == ERuntimeStatsSampleMode::Retain);

  FRenderingState DueState = RenderingState;
  DueState.StatsClock.PolyCountRefreshElapsedSeconds =
      StatsOverlay.Refresh.PolyCountRefreshIntervalSeconds;
  DueState.StatsClock.StatsRefreshElapsedSeconds =
      StatsOverlay.Refresh.StatsRefreshIntervalSeconds;
  const double DueClockSeconds =
      StatsOverlay.Refresh.BudgetLogIntervalSeconds;
  const float DueScreenshotIntervalSeconds =
      StatsOverlay.Refresh.BudgetLogIntervalSeconds;
  const FRuntimeStatsSamplingPlan DuePlan =
      RenderingStatsSelectors::SelectRuntimeStatsSamplingPlan(
          {&DueState, &StatsOverlay, DueClockSeconds,
           DueScreenshotIntervalSeconds});
  TestTrue(
      SamplingLabels.Next(),
      DuePlan.Refresh.PolyCountMode == ERuntimeStatsSampleMode::Refresh &&
          DuePlan.Refresh.StatsMode == ERuntimeStatsSampleMode::Refresh);
  TestTrue(SamplingLabels.Next(),
           DuePlan.Budget.Log.bTriggered &&
               FMath::IsNearlyEqual(DuePlan.Budget.Log.PreviousSeconds,
                                    DueClockSeconds));
  TestTrue(
      SamplingLabels.Next(),
      DuePlan.Budget.Screenshot.Checkpoint.bTriggered &&
          FMath::IsNearlyEqual(
              DuePlan.Budget.Screenshot.Checkpoint.PreviousSeconds,
              DueClockSeconds) &&
          DuePlan.Budget.Screenshot.Index ==
              DueState.BudgetClock.BudgetScreenshotIndex +
                  StatsOverlay.BudgetCapture.Request
                      .BudgetScreenshotIndexStep);
  TestTrue(
      SamplingLabels.Next(),
      DuePlan.Budget.Effects ==
          TArray<ERuntimeStatsEffect>{
              ERuntimeStatsEffect::Present,
              ERuntimeStatsEffect::BudgetLog,
              ERuntimeStatsEffect::BudgetScreenshot});

  const FRuntimeStatsObservation RetainedObservation = {
      {RenderingState.PolyCache.CachedPolyCount,
       RenderingState.PolyCache.CachedPolyCountMilliseconds},
      func::nothing<FRuntimeStatsViewModel>()};
  const FRuntimeStatsSamplePayload RetainedPayload =
      RenderingStatsSelectors::SelectRuntimeStatsSamplePayload(
          {StatsOverlay.Measurement.Frame.InitialDeltaSeconds, &RetainedPlan,
           &StatsOverlay, RetainedObservation});
  TestTrue(SamplingLabels.Next(),
           func::is_nothing(RetainedPayload.PolyCount) &&
               func::is_nothing(RetainedPayload.Stats));

  return true;
}
