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
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

#include <cstdarg>

namespace FG = ForbocAI::Game::Level;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeBudget, Log, All);

namespace {

using FRuntimeStatsOverlaySettings =
    ForbocAI::Game::Data::FRuntimeStatsOverlaySettings;
using FRuntimeStatsViewModel = FG::FRuntimeStatsViewModel;

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

struct FStatsValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  int32 MediumThreshold;
  int32 HighThreshold;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsDecimalValueUpdateRequest {
  UTextBlock *TextElement;
  double Value;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FStatsPlainValueUpdateRequest {
  UTextBlock *TextElement;
  int64 Value;
  const FRuntimeStatsOverlaySettings *Settings;
};

const FRuntimeStatsOverlaySettings &SelectStatsOverlaySettings() {
  return FG::RuntimeSelectors::SelectUIRuntimeSettings(
             FG::Store::GetStore().getState())
      .StatsOverlay;
}

FString FormatRuntimeStatsText(const FString &Format, int32 BufferCharacterCount,
                               ...) {
  TArray<TCHAR> Buffer;
  Buffer.SetNumZeroed(BufferCharacterCount);
  const TCHAR *FormatPtr = *Format;
  va_list Args;
  va_start(Args, BufferCharacterCount);
  FCString::GetVarArgs(Buffer.GetData(), Buffer.Num(), FormatPtr, Args);
  va_end(Args);
  return FString(Buffer.GetData());
}

FString FormatRuntimeStatsValue(int64 Value,
                                const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(Settings.ValueFormat,
                                Settings.FormatBufferCharacterCount,
                                static_cast<long long>(Value));
}

FString
FormatRuntimeStatsDecimalValue(double Value,
                               const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(Settings.DecimalValueFormat,
                                Settings.FormatBufferCharacterCount, Value);
}

FLinearColor SelectRuntimeStatsValueColor(
    const FStatsValueUpdateRequest &Request) {
  return Request.Value >= Request.HighThreshold
             ? Request.Settings->HighValueColor
             : (Request.Value >= Request.MediumThreshold
                    ? Request.Settings->MediumValueColor
                    : Request.Settings->LowValueColor);
}

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

void ApplyStatsValue(const FStatsValueUpdateRequest &Request) {
  check(Request.Settings);
  func::match(
      func::from_nullable_value(Request.TextElement,
                                Request.TextElement != nullptr),
      [&Request](UTextBlock *TextElement) {
        TextElement->SetText(FText::FromString(
            FormatRuntimeStatsValue(Request.Value, *Request.Settings)));
        TextElement->SetColorAndOpacity(
            FSlateColor(SelectRuntimeStatsValueColor(Request)));
      },
      []() {});
}

void ApplyStatsDecimalValue(const FStatsDecimalValueUpdateRequest &Request) {
  check(Request.Settings);
  func::match(
      func::from_nullable_value(Request.TextElement,
                                Request.TextElement != nullptr),
      [&Request](UTextBlock *TextElement) {
        TextElement->SetText(FText::FromString(
            FormatRuntimeStatsDecimalValue(Request.Value, *Request.Settings)));
        TextElement->SetColorAndOpacity(FSlateColor(Request.Settings->TextColor));
      },
      []() {});
}

void ApplyStatsPlainValue(const FStatsPlainValueUpdateRequest &Request) {
  check(Request.Settings);
  func::match(
      func::from_nullable_value(Request.TextElement,
                                Request.TextElement != nullptr),
      [&Request](UTextBlock *TextElement) {
        TextElement->SetText(FText::FromString(
            FormatRuntimeStatsValue(Request.Value, *Request.Settings)));
        TextElement->SetColorAndOpacity(FSlateColor(Request.Settings->TextColor));
      },
      []() {});
}

void LogRuntimeBudgetSample(const FRuntimeStatsViewModel &Stats) {
  UE_LOG(LogForbocRuntimeBudget, Display,
         TEXT("runtime-budget sample fps=%d stack_depth=%d poly_count=%lld "
              "memory_mib=%lld peak_memory_mib=%lld virtual_memory_mib=%lld "
              "game_ms=%.2f render_ms=%.2f rhi_ms=%.2f gpu_ms=%.2f "
              "draw_calls=%d rhi_primitives=%d wall_ms=%.2f "
              "input_delta_ms=%.2f stats_select_ms=%.2f poly_count_ms=%.2f "
              "engine_idle_ms=%.2f engine_idle_overshoot_ms=%.2f "
              "max_fps=%.2f frame_rate_limit=%.2f effective_max_tick_rate=%.2f "
              "fixed_frame_rate_enabled=%d fixed_frame_rate=%.2f "
              "fixed_time_step_enabled=%d fixed_delta_ms=%.2f "
              "vsync=%d idle_when_not_foreground=%d app_has_focus=%d "
              "cpu_throttle=%d all_windows_hidden=%d "
              "root_reducer_ms=%.2f combined_reducer_ms=%.2f "
              "ecs_projection_ms=%.2f ecs_entities=%d "
              "ecs_component_types=%d"),
         Stats.FramesPerSecond, Stats.StackDepth, Stats.PolyCount,
         Stats.UsedPhysicalMemoryMegabytes,
         Stats.PeakPhysicalMemoryMegabytes, Stats.UsedVirtualMemoryMegabytes,
         Stats.GameThreadMilliseconds, Stats.RenderThreadMilliseconds,
         Stats.RhiThreadMilliseconds, Stats.GpuMilliseconds, Stats.DrawCalls,
         Stats.RhiPrimitives, Stats.WallDeltaMilliseconds,
         Stats.InputDeltaMilliseconds, Stats.StatsSelectionMilliseconds,
         Stats.PolyCountMilliseconds, Stats.EngineIdleMilliseconds,
         Stats.EngineIdleOvershootMilliseconds, Stats.MaxFps,
         Stats.FrameRateLimit, Stats.EffectiveMaxTickRate,
         Stats.FixedFrameRateEnabled, Stats.FixedFrameRate,
         Stats.FixedTimeStepEnabled, Stats.FixedDeltaMilliseconds,
         Stats.VsyncEnabled,
         Stats.IdleWhenNotForegroundEnabled, Stats.AppHasFocus,
         Stats.CpuThrottleEnabled, Stats.AllWindowsHidden,
         Stats.RootReducerMilliseconds, Stats.CombinedReducerMilliseconds,
         Stats.EcsProjectionMilliseconds, Stats.ProjectedEntityCount,
         Stats.ProjectedComponentTypeCount);
}

FString FormatRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const FRuntimeStatsOverlaySettings &Settings) {
  return FormatRuntimeStatsText(
      Settings.DebugMessageFormat, Settings.FormatBufferCharacterCount,
      Stats.FramesPerSecond, Stats.StackDepth,
      static_cast<long long>(Stats.PolyCount),
      static_cast<long long>(Stats.UsedPhysicalMemoryMegabytes),
      static_cast<long long>(Stats.PeakPhysicalMemoryMegabytes),
      static_cast<long long>(Stats.UsedVirtualMemoryMegabytes),
      Stats.GameThreadMilliseconds, Stats.RenderThreadMilliseconds,
      Stats.RhiThreadMilliseconds, Stats.GpuMilliseconds, Stats.DrawCalls,
      Stats.RhiPrimitives, Stats.WallDeltaMilliseconds,
      Stats.InputDeltaMilliseconds, Stats.StatsSelectionMilliseconds,
      Stats.PolyCountMilliseconds, Stats.EngineIdleMilliseconds,
      Stats.EngineIdleOvershootMilliseconds, Stats.MaxFps,
      Stats.FrameRateLimit, Stats.EffectiveMaxTickRate,
      Stats.FixedFrameRateEnabled, Stats.FixedFrameRate,
      Stats.FixedTimeStepEnabled, Stats.FixedDeltaMilliseconds,
      Stats.VsyncEnabled, Stats.IdleWhenNotForegroundEnabled,
      Stats.AppHasFocus, Stats.CpuThrottleEnabled, Stats.AllWindowsHidden,
      Stats.RootReducerMilliseconds, Stats.CombinedReducerMilliseconds,
      Stats.EcsProjectionMilliseconds, Stats.ProjectedEntityCount,
      Stats.ProjectedComponentTypeCount);
}

void PresentRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const FRuntimeStatsOverlaySettings &Settings) {
  GEngine ? (GEngine->AddOnScreenDebugMessage(
                 Settings.DebugMessageKey,
                 Settings.DebugMessageDurationSeconds,
                 Settings.TextColor.ToFColor(true),
                 FormatRuntimeStatsDebugMessage(Stats, Settings)),
             void())
          : void();
}

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

} // namespace

void URuntimeStatsWidget::NativeConstruct() {
  Super::NativeConstruct();

  const FRuntimeStatsOverlaySettings &Settings = SelectStatsOverlaySettings();
  StatsRefreshElapsedSeconds = Settings.StatsRefreshIntervalSeconds;
  PolyCountRefreshElapsedSeconds = Settings.PolyCountRefreshIntervalSeconds;
  const double BudgetClockSeconds =
      FG::RenderingSlice::SelectRuntimeBudgetClockSeconds();
  LastFrameClockSeconds = BudgetClockSeconds;
  BudgetLogLastSeconds = BudgetClockSeconds;
  BudgetScreenshotIntervalSeconds =
      FG::RenderingSlice::SelectRuntimeBudgetScreenshotIntervalSeconds(
          Settings);
  BudgetScreenshotLastSeconds = BudgetClockSeconds;
  BudgetScreenshotIndex = Settings.BudgetScreenshotInitialIndex;
  CachedPolyCount = Settings.EmptyPolyCount;
  CachedPolyCountMilliseconds = Settings.DiagnosticDefaultFloatValue;
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
          PanelElement->SetContent(StackElement);
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.FramesPerSecondLabel,
               &FramesPerSecondValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.StackDepthLabel, &StackDepthValueTextElement,
               &Settings}));
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
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.MaxFpsLabel, &MaxFpsValueTextElement,
               &Settings}));
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
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.VsyncEnabledLabel,
               &VsyncEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.IdleWhenNotForegroundEnabledLabel,
               &IdleWhenNotForegroundEnabledValueTextElement, &Settings}));
          StackElement->AddChildToVerticalBox(BuildStatsMetricRow(
              {WidgetTree, Settings.AppHasFocusLabel,
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
  const FRuntimeStatsOverlaySettings &Settings = SelectStatsOverlaySettings();
  const double BudgetClockSeconds =
      FG::RenderingSlice::SelectRuntimeBudgetClockSeconds();
  const float WallDeltaSeconds =
      static_cast<float>(BudgetClockSeconds - LastFrameClockSeconds);
  LastFrameClockSeconds = BudgetClockSeconds;
  StatsRefreshElapsedSeconds += WallDeltaSeconds;
  PolyCountRefreshElapsedSeconds += WallDeltaSeconds;

  const bool bRefreshPolyCount =
      ShouldRunInterval(PolyCountRefreshElapsedSeconds,
                        Settings.PolyCountRefreshIntervalSeconds);
  const FG::FRuntimePolyCountStats PolyCountStats =
      bRefreshPolyCount
          ? FG::RenderingSlice::SelectRuntimePolyCountStats(GetWorld(),
                                                            Settings)
          : FG::FRuntimePolyCountStats{CachedPolyCount,
                                       CachedPolyCountMilliseconds};
  CachedPolyCount = PolyCountStats.PolyCount;
  CachedPolyCountMilliseconds = PolyCountStats.MeasurementMilliseconds;
  PolyCountRefreshElapsedSeconds =
      bRefreshPolyCount ? Settings.IntervalResetElapsedSeconds
                        : PolyCountRefreshElapsedSeconds;

  const bool bRefreshStats =
      ShouldRunInterval(StatsRefreshElapsedSeconds,
                        Settings.StatsRefreshIntervalSeconds);
  const FRuntimeStatsViewModel Stats =
      FG::RenderingSlice::SelectRuntimeStats(
          GetWorld(), DeltaSeconds, WallDeltaSeconds, CachedPolyCount,
          CachedPolyCountMilliseconds, Settings);
  bRefreshStats
      ? (ApplyStatsValue({FramesPerSecondValueTextElement,
                          Stats.FramesPerSecond,
                          Settings.FramesPerSecondMediumThreshold,
                          Settings.FramesPerSecondHighThreshold, &Settings}),
         ApplyStatsValue({StackDepthValueTextElement, Stats.StackDepth,
                          Settings.StackDepthMediumThreshold,
                          Settings.StackDepthHighThreshold, &Settings}),
         ApplyStatsValue({PolyCountValueTextElement, Stats.PolyCount,
                          Settings.PolyCountMediumThreshold,
                          Settings.PolyCountHighThreshold, &Settings}),
         ApplyStatsValue({UsedPhysicalMemoryValueTextElement,
                          Stats.UsedPhysicalMemoryMegabytes,
                          Settings.MemoryMediumThreshold,
                          Settings.MemoryHighThreshold, &Settings}),
         ApplyStatsValue({PeakPhysicalMemoryValueTextElement,
                          Stats.PeakPhysicalMemoryMegabytes,
                          Settings.MemoryMediumThreshold,
                          Settings.MemoryHighThreshold, &Settings}),
         ApplyStatsValue({UsedVirtualMemoryValueTextElement,
                          Stats.UsedVirtualMemoryMegabytes,
                          Settings.MemoryMediumThreshold,
                          Settings.MemoryHighThreshold, &Settings}),
         ApplyStatsDecimalValue({GameThreadMillisecondsValueTextElement,
                                 Stats.GameThreadMilliseconds, &Settings}),
         ApplyStatsDecimalValue({RenderThreadMillisecondsValueTextElement,
                                 Stats.RenderThreadMilliseconds, &Settings}),
         ApplyStatsDecimalValue({RhiThreadMillisecondsValueTextElement,
                                 Stats.RhiThreadMilliseconds, &Settings}),
         ApplyStatsDecimalValue({GpuMillisecondsValueTextElement,
                                 Stats.GpuMilliseconds, &Settings}),
         ApplyStatsPlainValue(
             {DrawCallsValueTextElement, Stats.DrawCalls, &Settings}),
         ApplyStatsPlainValue(
             {RhiPrimitivesValueTextElement, Stats.RhiPrimitives, &Settings}),
         ApplyStatsDecimalValue({WallDeltaMillisecondsValueTextElement,
                                 Stats.WallDeltaMilliseconds, &Settings}),
         ApplyStatsDecimalValue({InputDeltaMillisecondsValueTextElement,
                                 Stats.InputDeltaMilliseconds, &Settings}),
         ApplyStatsDecimalValue({StatsSelectionMillisecondsValueTextElement,
                                 Stats.StatsSelectionMilliseconds, &Settings}),
         ApplyStatsDecimalValue({PolyCountMillisecondsValueTextElement,
                                 Stats.PolyCountMilliseconds, &Settings}),
         ApplyStatsDecimalValue({EngineIdleMillisecondsValueTextElement,
                                 Stats.EngineIdleMilliseconds, &Settings}),
         ApplyStatsDecimalValue(
             {EngineIdleOvershootMillisecondsValueTextElement,
              Stats.EngineIdleOvershootMilliseconds, &Settings}),
         ApplyStatsDecimalValue(
             {MaxFpsValueTextElement, Stats.MaxFps, &Settings}),
         ApplyStatsDecimalValue({FrameRateLimitValueTextElement,
                                 Stats.FrameRateLimit, &Settings}),
         ApplyStatsDecimalValue({EffectiveMaxTickRateValueTextElement,
                                 Stats.EffectiveMaxTickRate, &Settings}),
         ApplyStatsPlainValue({FixedFrameRateEnabledValueTextElement,
                               Stats.FixedFrameRateEnabled, &Settings}),
         ApplyStatsDecimalValue({FixedFrameRateValueTextElement,
                                 Stats.FixedFrameRate, &Settings}),
         ApplyStatsPlainValue({FixedTimeStepEnabledValueTextElement,
                               Stats.FixedTimeStepEnabled, &Settings}),
         ApplyStatsDecimalValue({FixedDeltaMillisecondsValueTextElement,
                                 Stats.FixedDeltaMilliseconds, &Settings}),
         ApplyStatsPlainValue(
             {VsyncEnabledValueTextElement, Stats.VsyncEnabled, &Settings}),
         ApplyStatsPlainValue({IdleWhenNotForegroundEnabledValueTextElement,
                               Stats.IdleWhenNotForegroundEnabled, &Settings}),
         ApplyStatsPlainValue(
             {AppHasFocusValueTextElement, Stats.AppHasFocus, &Settings}),
         ApplyStatsPlainValue({CpuThrottleEnabledValueTextElement,
                               Stats.CpuThrottleEnabled, &Settings}),
         ApplyStatsPlainValue({AllWindowsHiddenValueTextElement,
                               Stats.AllWindowsHidden, &Settings}),
         ApplyStatsDecimalValue({RootReducerMillisecondsValueTextElement,
                                 Stats.RootReducerMilliseconds, &Settings}),
         ApplyStatsDecimalValue({CombinedReducerMillisecondsValueTextElement,
                                 Stats.CombinedReducerMilliseconds, &Settings}),
         ApplyStatsDecimalValue({EcsProjectionMillisecondsValueTextElement,
                                 Stats.EcsProjectionMilliseconds, &Settings}),
         ApplyStatsPlainValue({ProjectedEntityCountValueTextElement,
                               Stats.ProjectedEntityCount, &Settings}),
         ApplyStatsPlainValue({ProjectedComponentTypeCountValueTextElement,
                               Stats.ProjectedComponentTypeCount, &Settings}),
         PresentRuntimeStatsDebugMessage(Stats, Settings),
         StatsRefreshElapsedSeconds = Settings.IntervalResetElapsedSeconds,
         void())
      : void();

  const bool bLogBudgetSample =
      FG::RenderingSlice::ShouldRunRuntimeBudgetWallInterval(
          {BudgetClockSeconds, BudgetLogLastSeconds,
           Settings.BudgetLogIntervalSeconds});
  bLogBudgetSample ? (LogRuntimeBudgetSample(Stats),
                      BudgetLogLastSeconds = BudgetClockSeconds,
                      void())
                   : void();

  const bool bRequestBudgetScreenshot =
      FG::RenderingSlice::ShouldRunRuntimeBudgetScreenshot(
          {BudgetClockSeconds, BudgetScreenshotLastSeconds,
           BudgetScreenshotIntervalSeconds}, Settings);
  bRequestBudgetScreenshot
      ? (BudgetScreenshotIndex += Settings.BudgetScreenshotIndexStep,
         FG::RenderingSlice::RequestRuntimeBudgetScreenshot(
             Settings, BudgetScreenshotIndex),
         BudgetScreenshotLastSeconds = BudgetClockSeconds,
         void())
      : void();
}
