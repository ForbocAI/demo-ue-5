// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/RuntimeStatsWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Engine/Engine.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"

namespace FG = ForbocAI::Game::Level;

namespace {

using FRuntimeStatsOverlaySettings =
    ForbocAI::Game::Data::FRuntimeStatsOverlaySettings;

struct FStatsTextElementRequest {
  UWidgetTree *Tree;
  FString Text;
  FLinearColor Color;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsMetricRowRequest {
  UWidgetTree *Tree;
  FString Label;
  UTextBlock **ValueTextElement;
  const FRuntimeStatsOverlaySettings *Settings;
};

// --- View helpers (UMG widget construction) ---

UTextBlock *BuildStatsTextElement(const FStatsTextElementRequest &Request) {
  check(Request.Tree);
  check(Request.Settings);
  UTextBlock *Element =
      Request.Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
  check(Element);
  FSlateFontInfo Font = Element->GetFont();
  Font.Size = Request.Settings->FontSize;
  Element->SetFont(Font);
  Element->SetText(FText::FromString(Request.Text));
  Element->SetColorAndOpacity(FSlateColor(Request.Color));
  Element->SetAutoWrapText(Request.Settings->bAutoWrapText);
  return Element;
}

UHorizontalBox *BuildStatsMetricRow(const FStatsMetricRowRequest &Request) {
  check(Request.Tree);
  check(Request.ValueTextElement);
  check(Request.Settings);
  UHorizontalBox *RowElement =
      Request.Tree->ConstructWidget<UHorizontalBox>(
          UHorizontalBox::StaticClass());
  check(RowElement);
  RowElement->AddChildToHorizontalBox(BuildStatsTextElement(
      {Request.Tree, Request.Label + Request.Settings->LabelValueSeparator,
       Request.Settings->TextColor, Request.Settings}));
  *Request.ValueTextElement = BuildStatsTextElement(
      {Request.Tree, FString(), Request.Settings->LowValueColor,
       Request.Settings});
  RowElement->AddChildToHorizontalBox(*Request.ValueTextElement);
  return RowElement;
}

// --- View apply helpers (set widget text/color from selector output) ---

void ApplyStatsText(UTextBlock *TextElement, const FG::FStatsTextModel &Model) {
  TextElement ? (TextElement->SetText(FText::FromString(Model.Text)),
                 TextElement->SetColorAndOpacity(FSlateColor(Model.Color)),
                 void())
              : void();
}

} // namespace

void URuntimeStatsWidget::NativeConstruct() {
  Super::NativeConstruct();

  const auto &Settings =
      FG::RuntimeSelectors::SelectUIRuntimeSettings(
          FG::RuntimeSelectors::SelectState())
          .StatsOverlay;
  SetPositionInViewport(FVector2D(Settings.ViewportLeft, Settings.ViewportTop),
                        Settings.bRemoveDpIScale);
  SetDesiredSizeInViewport(
      FVector2D(Settings.ViewportWidth, Settings.ViewportHeight));
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

          PanelElement->SetPadding(FMargin(Settings.PanelPadding));
          PanelElement->SetBrushColor(Settings.PanelColor);
          PanelElement->AddChild(StackElement);

          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FramesPerSecondLabel,
               &FramesPerSecondValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.StackDepthLabel,
               &StackDepthValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.PolyCountLabel, &PolyCountValueTextElement,
               &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.UsedPhysicalMemoryLabel,
               &UsedPhysicalMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.PeakPhysicalMemoryLabel,
               &PeakPhysicalMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.UsedVirtualMemoryLabel,
               &UsedVirtualMemoryValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.GameThreadMillisecondsLabel,
               &GameThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.RenderThreadMillisecondsLabel,
               &RenderThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.RhiThreadMillisecondsLabel,
               &RhiThreadMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.GpuMillisecondsLabel,
               &GpuMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.DrawCallsLabel, &DrawCallsValueTextElement,
               &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.RhiPrimitivesLabel,
               &RhiPrimitivesValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.WallDeltaMillisecondsLabel,
               &WallDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.InputDeltaMillisecondsLabel,
               &InputDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.StatsSelectionMillisecondsLabel,
               &StatsSelectionMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.PolyCountMillisecondsLabel,
               &PolyCountMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.EngineIdleMillisecondsLabel,
               &EngineIdleMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.EngineIdleOvershootMillisecondsLabel,
               &EngineIdleOvershootMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.MaxFpsLabel,
                                   &MaxFpsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FrameRateLimitLabel,
               &FrameRateLimitValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.EffectiveMaxTickRateLabel,
               &EffectiveMaxTickRateValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FixedFrameRateEnabledLabel,
               &FixedFrameRateEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FixedFrameRateLabel,
               &FixedFrameRateValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FixedTimeStepEnabledLabel,
               &FixedTimeStepEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FixedDeltaMillisecondsLabel,
               &FixedDeltaMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.VsyncEnabledLabel,
                                   &VsyncEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.IdleWhenNotForegroundEnabledLabel,
               &IdleWhenNotForegroundEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(
              BuildStatsMetricRow({WidgetTree, Settings.AppHasFocusLabel,
                                   &AppHasFocusValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.CpuThrottleEnabledLabel,
               &CpuThrottleEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.AllWindowsHiddenLabel,
               &AllWindowsHiddenValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.RootReducerMillisecondsLabel,
               &RootReducerMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.CombinedReducerMillisecondsLabel,
               &CombinedReducerMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.EcsProjectionMillisecondsLabel,
               &EcsProjectionMillisecondsValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.ProjectedEntityCountLabel,
               &ProjectedEntityCountValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.ProjectedComponentTypeCountLabel,
               &ProjectedComponentTypeCountValueTextElement, &Settings}));
          WidgetTree->RootWidget = PanelElement;
        }(), void());
}

void URuntimeStatsWidget::NativeTick(const FGeometry &MyGeometry,
                                     float InDeltaTime) {
  Super::NativeTick(MyGeometry, InDeltaTime);
  RefreshStats(InDeltaTime);
}

void URuntimeStatsWidget::RefreshStats(float DeltaSeconds) {
  FG::RuntimeActions::DispatchObserveRuntimeStatsTick(GetWorld(), DeltaSeconds);

  const auto &State = FG::RuntimeSelectors::SelectState();
  const FG::FRuntimeStatsPresentationModel &Presentation =
      FG::RuntimeSelectors::SelectRuntimeStatsPresentation(State);

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
