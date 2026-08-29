// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Stats/StatsView.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/Engine.h"
#include "Features/Systems/SystemsActions.h"
#include "Features/Systems/SystemsSelectors.h"

namespace FG = ForbocAI::Game::Level;

namespace {

using FOverlaySettings =
    ForbocAI::Game::Data::FOverlaySettings;

struct FTextElementRequest {
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  const FOverlaySettings *Settings;
};

struct FMetricRowRequest {
  UWidgetTree *Tree;
  FString Label;
  UTextBlock **ValueTextElement;
  const FOverlaySettings *Settings;
};

// --- View helpers (UMG widget construction) ---

/** User Story: As a views stats consumer, I need to invoke build stats text element through a stable signature so the views stats workflow remains explicit and composable. @fn UTextBlock *BuildStatsTextElement(const FTextElementRequest &Request) */
UTextBlock *BuildStatsTextElement(const FTextElementRequest &Request) {
  check(Request.Tree);
  check(Request.Settings);
  UTextBlock *Element =
      Request.Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
  check(Element);
  FSlateFontInfo Font = Element->GetFont();
  Font.Size = Request.Settings->Presentation.Typography.FontSize;
  Element->SetFont(Font);
  Element->SetText(FText::FromString(Request.Text));
  Element->SetColorAndOpacity(FSlateColor(Request.Color));
  Element->SetAutoWrapText(
      Request.Settings->Presentation.Typography.bAutoWrapText);
  return Element;
}

/** User Story: As a views stats consumer, I need to invoke build stats metric row through a stable signature so the views stats workflow remains explicit and composable. @fn UHorizontalBox *BuildStatsMetricRow(const FMetricRowRequest &Request) */
UHorizontalBox *BuildStatsMetricRow(const FMetricRowRequest &Request) {
  check(Request.Tree);
  check(Request.ValueTextElement);
  check(Request.Settings);
  UHorizontalBox *RowElement =
      Request.Tree->ConstructWidget<UHorizontalBox>(
          UHorizontalBox::StaticClass());
  check(RowElement);
  RowElement->AddChildToHorizontalBox(BuildStatsTextElement(
      {Request.Tree,
       Request.Label + Request.Settings->Presentation.Format.LabelValueSeparator,
       Request.Settings->Presentation.Colors.TextColor, Request.Settings}));
  *Request.ValueTextElement = BuildStatsTextElement(
      {Request.Tree, FString(),
       Request.Settings->Presentation.Colors.LowValueColor,
       Request.Settings});
  RowElement->AddChildToHorizontalBox(*Request.ValueTextElement);
  return RowElement;
}

// --- View apply helpers (set widget text/color from selector output) ---

/** User Story: As a views stats consumer, I need to invoke apply stats text through a stable signature so the views stats workflow remains explicit and composable. @fn void ApplyStatsText(UTextBlock *TextElement, const FG::FStatsTextModel &Model) */
void ApplyStatsText(UTextBlock *TextElement, const FG::FStatsTextModel &Model) {
  TextElement ? (TextElement->SetText(FText::FromString(Model.Text)),
                 TextElement->SetColorAndOpacity(FSlateColor(Model.Color)),
                 void())
              : void();
}

} // namespace

/** User Story: As a views stats consumer, I need to invoke native construct through a stable signature so the views stats workflow remains explicit and composable. @fn void URuntimeStatsWidget::NativeConstruct() */
void URuntimeStatsWidget::NativeConstruct() {
  Super::NativeConstruct();

  const auto &Settings =
      FG::RuntimeSelectors::SelectUISettings().StatsOverlay;
  SetPositionInViewport(FVector2D(Settings.Presentation.Viewport.ViewportLeft, Settings.Presentation.Viewport.ViewportTop),
                        Settings.Presentation.Typography.bRemoveDpIScale);
  SetDesiredSizeInViewport(
      FVector2D(Settings.Presentation.Viewport.ViewportWidth, Settings.Presentation.Viewport.ViewportHeight));
  SetVisibility(ESlateVisibility::Visible);

  check(WidgetTree);
  WidgetTree->RootWidget
      ? void()
      : ([this, &Settings]() {
          UBorder *PanelElement =
              WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass());
          UVerticalBox *StackElement =
              WidgetTree->ConstructWidget<UVerticalBox>(
                  UVerticalBox::StaticClass());

          check(PanelElement);
          check(StackElement);

          PanelElement->SetPadding(FMargin(Settings.Presentation.Viewport.PanelPadding));
          PanelElement->SetBrushColor(Settings.Presentation.Colors.PanelColor);
          PanelElement->AddChild(StackElement);

          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Performance.FramesPerSecondLabel,
               &FramesPerSecondValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Performance.StackDepthLabel,
               &StackDepthValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Performance.PolyCountLabel, &PolyCountValueTextElement,
               &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Memory.UsedPhysicalMemoryLabel,
               &UsedPhysicalMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Memory.PeakPhysicalMemoryLabel,
               &PeakPhysicalMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Memory.UsedVirtualMemoryLabel,
               &UsedVirtualMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.GameThreadMillisecondsLabel,
               &GameThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.RenderThreadMillisecondsLabel,
               &RenderThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.RhiThreadMillisecondsLabel,
               &RhiThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.GpuMillisecondsLabel,
               &GpuMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.DrawCallsLabel, &DrawCallsValueTextElement,
               &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.RhiPrimitivesLabel,
               &RhiPrimitivesValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.WallDeltaMillisecondsLabel,
               &WallDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Timing.InputDeltaMillisecondsLabel,
               &InputDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.StatsSelectionMillisecondsLabel,
               &StatsSelectionMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.PolyCountMillisecondsLabel,
               &PolyCountMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.EngineIdleMillisecondsLabel,
               &EngineIdleMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rendering.EngineIdleOvershootMillisecondsLabel,
               &EngineIdleOvershootMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.Labels.Rate.MaxFpsLabel,
                                   &MaxFpsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rate.FrameRateLimitLabel,
               &FrameRateLimitValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rate.EffectiveMaxTickRateLabel,
               &EffectiveMaxTickRateValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rate.FixedFrameRateEnabledLabel,
               &FixedFrameRateEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Rate.FixedFrameRateLabel,
               &FixedFrameRateValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.State.FixedTimeStepEnabledLabel,
               &FixedTimeStepEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.State.FixedDeltaMillisecondsLabel,
               &FixedDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.Labels.Rate.VsyncEnabledLabel,
                                   &VsyncEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.State.IdleWhenNotForegroundEnabledLabel,
               &IdleWhenNotForegroundEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.Labels.State.AppHasFocusLabel,
                                   &AppHasFocusValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.State.CpuThrottleEnabledLabel,
               &CpuThrottleEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.State.AllWindowsHiddenLabel,
               &AllWindowsHiddenValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Projection.RootReducerMillisecondsLabel,
               &RootReducerMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Projection.CombinedReducerMillisecondsLabel,
               &CombinedReducerMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Projection.EcsProjectionMillisecondsLabel,
               &EcsProjectionMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Projection.ProjectedEntityCountLabel,
               &ProjectedEntityCountValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.Labels.Projection.ProjectedComponentTypeCountLabel,
               &ProjectedComponentTypeCountValueTextElement, &Settings}));
          WidgetTree->RootWidget = PanelElement;
        }(), void());
}

/** User Story: As a views stats consumer, I need to invoke native tick through a stable signature so the views stats workflow remains explicit and composable. @fn void URuntimeStatsWidget::NativeTick(const FGeometry &MyGeometry, float InDeltaTime) */
void URuntimeStatsWidget::NativeTick(const FGeometry &MyGeometry,
                                     float InDeltaTime) {
  Super::NativeTick(MyGeometry, InDeltaTime);
  RefreshStats(InDeltaTime);
}

/** User Story: As a views stats consumer, I need to invoke refresh stats through a stable signature so the views stats workflow remains explicit and composable. @fn void URuntimeStatsWidget::RefreshStats(float DeltaSeconds) */
void URuntimeStatsWidget::RefreshStats(float DeltaSeconds) {
  FG::RuntimeActions::DispatchObserveRuntimeStatsTick(GetWorld(), DeltaSeconds);
  const FG::FRuntimeStatsPresentationModel &Presentation =
      FG::RuntimeSelectors::SelectRuntimeStatsPresentation();

  ApplyStatsText(FramesPerSecondValueTextElement, Presentation.FramesPerSecond);
  ApplyStatsText(StackDepthValueTextElement, Presentation.StackDepth);
  ApplyStatsText(PolyCountValueTextElement, Presentation.PolyCount);
  ApplyStatsText(UsedPhysicalMemoryValueTextElement, Presentation.UsedPhysicalMemoryMegabytes);
  ApplyStatsText(PeakPhysicalMemoryValueTextElement, Presentation.PeakPhysicalMemoryMegabytes);
  ApplyStatsText(UsedVirtualMemoryValueTextElement, Presentation.UsedVirtualMemoryMegabytes);
  ApplyStatsText(GameThreadMillisecondsValueTextElement, Presentation.GameThreadMilliseconds);
  ApplyStatsText(RenderThreadMillisecondsValueTextElement, Presentation.RenderThreadMilliseconds);
  ApplyStatsText(RhiThreadMillisecondsValueTextElement, Presentation.RhiThreadMilliseconds);
  ApplyStatsText(GpuMillisecondsValueTextElement, Presentation.GpuMilliseconds);
  ApplyStatsText(DrawCallsValueTextElement, Presentation.DrawCalls);
  ApplyStatsText(RhiPrimitivesValueTextElement, Presentation.RhiPrimitives);
  ApplyStatsText(WallDeltaMillisecondsValueTextElement, Presentation.WallDeltaMilliseconds);
  ApplyStatsText(InputDeltaMillisecondsValueTextElement, Presentation.InputDeltaMilliseconds);
  ApplyStatsText(StatsSelectionMillisecondsValueTextElement, Presentation.StatsSelectionMilliseconds);
  ApplyStatsText(PolyCountMillisecondsValueTextElement, Presentation.PolyCountMilliseconds);
  ApplyStatsText(EngineIdleMillisecondsValueTextElement, Presentation.EngineIdleMilliseconds);
  ApplyStatsText(EngineIdleOvershootMillisecondsValueTextElement, Presentation.EngineIdleOvershootMilliseconds);
  ApplyStatsText(MaxFpsValueTextElement, Presentation.MaxFps);
  ApplyStatsText(FrameRateLimitValueTextElement, Presentation.FrameRateLimit);
  ApplyStatsText(EffectiveMaxTickRateValueTextElement, Presentation.EffectiveMaxTickRate);
  ApplyStatsText(FixedFrameRateEnabledValueTextElement, Presentation.FixedFrameRateEnabled);
  ApplyStatsText(FixedFrameRateValueTextElement, Presentation.FixedFrameRate);
  ApplyStatsText(FixedTimeStepEnabledValueTextElement, Presentation.FixedTimeStepEnabled);
  ApplyStatsText(FixedDeltaMillisecondsValueTextElement, Presentation.FixedDeltaMilliseconds);
  ApplyStatsText(VsyncEnabledValueTextElement, Presentation.VsyncEnabled);
  ApplyStatsText(IdleWhenNotForegroundEnabledValueTextElement, Presentation.IdleWhenNotForegroundEnabled);
  ApplyStatsText(AppHasFocusValueTextElement, Presentation.AppHasFocus);
  ApplyStatsText(CpuThrottleEnabledValueTextElement, Presentation.CpuThrottleEnabled);
  ApplyStatsText(AllWindowsHiddenValueTextElement, Presentation.AllWindowsHidden);
  ApplyStatsText(RootReducerMillisecondsValueTextElement, Presentation.RootReducerMilliseconds);
  ApplyStatsText(CombinedReducerMillisecondsValueTextElement, Presentation.CombinedReducerMilliseconds);
  ApplyStatsText(EcsProjectionMillisecondsValueTextElement, Presentation.EcsProjectionMilliseconds);
  ApplyStatsText(ProjectedEntityCountValueTextElement, Presentation.ProjectedEntityCount);
  ApplyStatsText(ProjectedComponentTypeCountValueTextElement, Presentation.ProjectedComponentTypeCount);
}
