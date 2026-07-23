#include "Tests/Store/StoreSpecSupport.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FStoreDataBackedMapRendering,
    "ForbocAI.Runtime.Store.DataBackedMap.Rendering",
    EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/** User Story: As a tests store consumer, I need to invoke run test through a stable signature so the tests store workflow remains explicit and composable. @fn bool FStoreDataBackedMapRendering::RunTest(const FString &Parameters) */
bool FStoreDataBackedMapRendering::RunTest(const FString &Parameters) {
  (void)Parameters;
  const FSettings Settings =
      ForbocAI::Game::Data::SettingsAdapters::LoadSettings();
  const FStoreSettings StoreAutomation = Settings.Automation.Store;
  FLabelCursor Labels = StoreLabels(
      StoreAutomation.Labels.DataBackedMapLabels);
  rtk::EnhancedStore<FRuntimeState> EnhancedStoreValue =
      Store::ConfigureStore();
  const FRuntimeState &State = EnhancedStoreValue.getState();

  const FLevelRetroRenderProfile &RetroProfile =
      RenderingSelectors::SelectRuntimeProfile(
          RuntimeSelectors::SelectRenderingState(State));
  const ForbocAI::Game::Data::FProfileSettings &ExpectedProfile =
      Settings.RenderingProfile;
  TestEqual(Labels.Next(),
            RetroProfile.TimeOfDayHour, ExpectedProfile.TimeOfDayHour);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcessSaturationMultiplier,
            ExpectedProfile.PostProcessSaturationMultiplier);
  TestEqual(Labels.Next(),
            RetroProfile.PostProcessContrastMultiplier,
            ExpectedProfile.PostProcessContrastMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.DirectionalLightColorR >=
               RetroProfile.DirectionalLightColorB);
  TestEqual(Labels.Next(),
            RetroProfile.AntiAliasingMethod,
            ExpectedProfile.AntiAliasingMethod);
  TestEqual(Labels.Next(),
            RetroProfile.ScreenPercentage, ExpectedProfile.ScreenPercentage);
  TestEqual(Labels.Next(),
            RetroProfile.InternalRenderWidth,
            ExpectedProfile.InternalRenderWidth);
  TestEqual(Labels.Next(),
            RetroProfile.InternalRenderHeight,
            ExpectedProfile.InternalRenderHeight);
  TestEqual(Labels.Next(),
            RetroProfile.OutputScaleMultiplier,
            ExpectedProfile.OutputScaleMultiplier);
  TestTrue(Labels.Next(),
           RetroProfile.bFullscreenOutput == ExpectedProfile.bFullscreenOutput);
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(RetroProfile.ViewDistanceScale,
                                ExpectedProfile.ViewDistanceScale));
  TestTrue(Labels.Next(),
           RetroProfile.bSkyDomeEnabled == ExpectedProfile.bSkyDomeEnabled &&
               RetroProfile.SkyDomeMeshPath == ExpectedProfile.SkyDomeMeshPath &&
               RetroProfile.SkyDomeMaterialPath ==
                   ExpectedProfile.SkyDomeMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.SkyDomeHorizonColorB >
                   RetroProfile.SkyDomeZenithColorB &&
               FMath::IsNearlyEqual(RetroProfile.SkyDomeSkyBrightness,
                                    ExpectedProfile.SkyDomeSkyBrightness));
  TestTrue(Labels.Next(),
           FMath::IsNearlyEqual(RetroProfile.SkyDomeStarColorR,
                                ExpectedProfile.SkyDomeStarColorR) &&
               FMath::IsNearlyEqual(RetroProfile.SkyDomeStarColorB,
                                    ExpectedProfile.SkyDomeStarColorB));
  TestTrue(Labels.Next(),
           RetroProfile.bMoonDiscEnabled == ExpectedProfile.bMoonDiscEnabled &&
               RetroProfile.MoonDiscMeshPath ==
                   ExpectedProfile.MoonDiscMeshPath &&
               RetroProfile.MoonDiscMaterialPath ==
                   ExpectedProfile.MoonDiscMaterialPath);
  TestTrue(Labels.Next(),
           RetroProfile.bFogEnabled == ExpectedProfile.bFogEnabled &&
               FMath::IsNearlyEqual(RetroProfile.FogDensity,
                                    ExpectedProfile.FogDensity) &&
               FMath::IsNearlyEqual(RetroProfile.FogMaxOpacity,
                                    ExpectedProfile.FogMaxOpacity) &&
               FMath::IsNearlyEqual(RetroProfile.FogColorB,
                                    ExpectedProfile.FogColorB));
  TestFalse(Labels.Next(),
            RetroProfile.bVolumetricFogEnabled);

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
            StatsOverlay.MemoryBytesPerMegabyte,
            ExpectedStatsOverlay.MemoryBytesPerMegabyte);
  TestEqual(Labels.Next(),
            StatsOverlay.UsedPhysicalMemoryLabel,
            ExpectedStatsOverlay.UsedPhysicalMemoryLabel);
  TestEqual(Labels.Next(),
            StatsOverlay.PeakPhysicalMemoryLabel,
            ExpectedStatsOverlay.PeakPhysicalMemoryLabel);

  const TArray<FLevelRetroTextureSpec> &TextureCatalog =
      RenderingSelectors::SelectTextureCatalog(
          RuntimeSelectors::SelectRenderingState(State));
  TestTrue(Labels.Next(),
           TextureCatalog.Num() >= Settings.TextureCatalog.Num());


  return true;
}
