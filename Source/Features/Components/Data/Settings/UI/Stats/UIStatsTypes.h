#pragma once

#include "Core/rtk.hpp"

namespace ForbocAI {
namespace Game {
namespace Data {

struct FStatsOverlaySettings {
  FString FramesPerSecondLabel;
  FString StackDepthLabel;
  FString PolyCountLabel;
  FString UsedPhysicalMemoryLabel;
  FString PeakPhysicalMemoryLabel;
  FString UsedVirtualMemoryLabel;
  FString GameThreadMillisecondsLabel;
  FString RenderThreadMillisecondsLabel;
  FString RhiThreadMillisecondsLabel;
  FString GpuMillisecondsLabel;
  FString DrawCallsLabel;
  FString RhiPrimitivesLabel;
  FString WallDeltaMillisecondsLabel;
  FString InputDeltaMillisecondsLabel;
  FString StatsSelectionMillisecondsLabel;
  FString PolyCountMillisecondsLabel;
  FString EngineIdleMillisecondsLabel;
  FString EngineIdleOvershootMillisecondsLabel;
  FString MaxFpsLabel;
  FString FrameRateLimitLabel;
  FString EffectiveMaxTickRateLabel;
  FString FixedFrameRateEnabledLabel;
  FString FixedFrameRateLabel;
  FString FixedTimeStepEnabledLabel;
  FString FixedDeltaMillisecondsLabel;
  FString VsyncEnabledLabel;
  FString IdleWhenNotForegroundEnabledLabel;
  FString AppHasFocusLabel;
  FString CpuThrottleEnabledLabel;
  FString AllWindowsHiddenLabel;
  FString RootReducerMillisecondsLabel;
  FString CombinedReducerMillisecondsLabel;
  FString EcsProjectionMillisecondsLabel;
  FString ProjectedEntityCountLabel;
  FString ProjectedComponentTypeCountLabel;
  FString LabelValueSeparator;
  FString ValueFormat;
  FString DecimalValueFormat;
  FString DebugMessageFormat;
  FString BudgetLogFormat;
  int32 FormatBufferCharacterCount;
  int32 DebugMessageKey;
  float DebugMessageDurationSeconds;
  float ViewportLeft;
  float ViewportTop;
  float ViewportWidth;
  float ViewportHeight;
  float PanelPadding;
  float StatsRefreshIntervalSeconds;
  float PolyCountRefreshIntervalSeconds;
  float BudgetLogIntervalSeconds;
  float IntervalResetElapsedSeconds;
  float BudgetScreenshotIntervalSeconds;
  float BudgetScreenshotDisabledIntervalSeconds;
  FString BudgetScreenshotIntervalCommandLineKey;
  FString BudgetScreenshotDirectory;
  FString BudgetScreenshotFileNameFormat;
  int32 BudgetScreenshotInitialIndex;
  int32 BudgetScreenshotIndexStep;
  float FramesPerSecondNumerator;
  float MinimumDeltaSeconds;
  float InitialDeltaSeconds;
  int32 InitialFramesPerSecond;
  int32 EmptyStackDepth;
  int32 EmptyPolyCount;
  int32 EmptyTriangleCount;
  int32 EmptyMemoryMegabytes;
  int32 MemoryBytesPerMegabyte;
  int32 RhiStatsGpuIndex;
  int32 RhiStatsMinimumGpuIndex;
  int32 RhiStatsMaximumGpuIndex;
  FString IdleWhenNotForegroundCVarName;
  FString MaxFpsCVarName;
  FString VsyncCVarName;
  float SecondsToMilliseconds;
  int32 DiagnosticFalseIntValue;
  int32 DiagnosticTrueIntValue;
  int32 DiagnosticDefaultIntValue;
  float DiagnosticDefaultFloatValue;
  int32 MeshLodIndex;
  int32 ForcedLodAutomaticModel;
  int32 LodModelIndexOffset;
  int32 ProcMeshFirstSectionIndex;
  int32 ProcMeshSectionStep;
  int32 TriangleIndexDivisor;
  int32 ZOrder;
  int32 FontSize;
  int32 FramesPerSecondMediumThreshold;
  int32 FramesPerSecondHighThreshold;
  int32 StackDepthMediumThreshold;
  int32 StackDepthHighThreshold;
  int32 PolyCountMediumThreshold;
  int32 PolyCountHighThreshold;
  int32 MemoryMediumThreshold;
  int32 MemoryHighThreshold;
  bool bRemoveDpIScale;
  bool bAutoWrapText;
  bool bBudgetScreenshotCreateDirectoryTree;
  bool bBudgetScreenshotShowUI;
  bool bBudgetScreenshotAddFilenameSuffix;
  bool bDiagnosticAllowFrameRateSmoothing;
  FLinearColor PanelColor;
  FLinearColor TextColor;
  FLinearColor LowValueColor;
  FLinearColor MediumValueColor;
  FLinearColor HighValueColor;
};

inline bool operator==(const FStatsOverlaySettings &Left,
                       const FStatsOverlaySettings &Right) {
  return Left.FramesPerSecondLabel == Right.FramesPerSecondLabel &&
         Left.StackDepthLabel == Right.StackDepthLabel &&
         Left.PolyCountLabel == Right.PolyCountLabel &&
         Left.UsedPhysicalMemoryLabel == Right.UsedPhysicalMemoryLabel &&
         Left.PeakPhysicalMemoryLabel == Right.PeakPhysicalMemoryLabel &&
         Left.UsedVirtualMemoryLabel == Right.UsedVirtualMemoryLabel &&
         Left.GameThreadMillisecondsLabel ==
             Right.GameThreadMillisecondsLabel &&
         Left.RenderThreadMillisecondsLabel ==
             Right.RenderThreadMillisecondsLabel &&
         Left.RhiThreadMillisecondsLabel ==
             Right.RhiThreadMillisecondsLabel &&
         Left.GpuMillisecondsLabel == Right.GpuMillisecondsLabel &&
         Left.DrawCallsLabel == Right.DrawCallsLabel &&
         Left.RhiPrimitivesLabel == Right.RhiPrimitivesLabel &&
         Left.WallDeltaMillisecondsLabel ==
             Right.WallDeltaMillisecondsLabel &&
         Left.InputDeltaMillisecondsLabel ==
             Right.InputDeltaMillisecondsLabel &&
         Left.StatsSelectionMillisecondsLabel ==
             Right.StatsSelectionMillisecondsLabel &&
         Left.PolyCountMillisecondsLabel ==
             Right.PolyCountMillisecondsLabel &&
         Left.EngineIdleMillisecondsLabel ==
             Right.EngineIdleMillisecondsLabel &&
         Left.EngineIdleOvershootMillisecondsLabel ==
             Right.EngineIdleOvershootMillisecondsLabel &&
         Left.MaxFpsLabel == Right.MaxFpsLabel &&
         Left.FrameRateLimitLabel == Right.FrameRateLimitLabel &&
         Left.EffectiveMaxTickRateLabel == Right.EffectiveMaxTickRateLabel &&
         Left.FixedFrameRateEnabledLabel ==
             Right.FixedFrameRateEnabledLabel &&
         Left.FixedFrameRateLabel == Right.FixedFrameRateLabel &&
         Left.FixedTimeStepEnabledLabel == Right.FixedTimeStepEnabledLabel &&
         Left.FixedDeltaMillisecondsLabel ==
             Right.FixedDeltaMillisecondsLabel &&
         Left.VsyncEnabledLabel == Right.VsyncEnabledLabel &&
         Left.IdleWhenNotForegroundEnabledLabel ==
             Right.IdleWhenNotForegroundEnabledLabel &&
         Left.AppHasFocusLabel == Right.AppHasFocusLabel &&
         Left.CpuThrottleEnabledLabel == Right.CpuThrottleEnabledLabel &&
         Left.AllWindowsHiddenLabel == Right.AllWindowsHiddenLabel &&
         Left.RootReducerMillisecondsLabel ==
             Right.RootReducerMillisecondsLabel &&
         Left.CombinedReducerMillisecondsLabel ==
             Right.CombinedReducerMillisecondsLabel &&
         Left.EcsProjectionMillisecondsLabel ==
             Right.EcsProjectionMillisecondsLabel &&
         Left.ProjectedEntityCountLabel == Right.ProjectedEntityCountLabel &&
         Left.ProjectedComponentTypeCountLabel ==
             Right.ProjectedComponentTypeCountLabel &&
         Left.LabelValueSeparator == Right.LabelValueSeparator &&
         Left.ValueFormat == Right.ValueFormat &&
         Left.DecimalValueFormat == Right.DecimalValueFormat &&
         Left.DebugMessageFormat == Right.DebugMessageFormat &&
         Left.BudgetLogFormat == Right.BudgetLogFormat &&
         Left.FormatBufferCharacterCount == Right.FormatBufferCharacterCount &&
         Left.DebugMessageKey == Right.DebugMessageKey &&
         FMath::IsNearlyEqual(Left.DebugMessageDurationSeconds,
                              Right.DebugMessageDurationSeconds) &&
         FMath::IsNearlyEqual(Left.ViewportLeft, Right.ViewportLeft) &&
         FMath::IsNearlyEqual(Left.ViewportTop, Right.ViewportTop) &&
         FMath::IsNearlyEqual(Left.ViewportWidth, Right.ViewportWidth) &&
         FMath::IsNearlyEqual(Left.ViewportHeight, Right.ViewportHeight) &&
         FMath::IsNearlyEqual(Left.PanelPadding, Right.PanelPadding) &&
         FMath::IsNearlyEqual(Left.StatsRefreshIntervalSeconds,
                              Right.StatsRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.PolyCountRefreshIntervalSeconds,
                              Right.PolyCountRefreshIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetLogIntervalSeconds,
                              Right.BudgetLogIntervalSeconds) &&
         FMath::IsNearlyEqual(Left.IntervalResetElapsedSeconds,
                              Right.IntervalResetElapsedSeconds) &&
         FMath::IsNearlyEqual(Left.BudgetScreenshotIntervalSeconds,
                              Right.BudgetScreenshotIntervalSeconds) &&
         FMath::IsNearlyEqual(
             Left.BudgetScreenshotDisabledIntervalSeconds,
             Right.BudgetScreenshotDisabledIntervalSeconds) &&
         Left.BudgetScreenshotIntervalCommandLineKey ==
             Right.BudgetScreenshotIntervalCommandLineKey &&
         Left.BudgetScreenshotDirectory == Right.BudgetScreenshotDirectory &&
         Left.BudgetScreenshotFileNameFormat ==
             Right.BudgetScreenshotFileNameFormat &&
         Left.BudgetScreenshotInitialIndex ==
             Right.BudgetScreenshotInitialIndex &&
         Left.BudgetScreenshotIndexStep == Right.BudgetScreenshotIndexStep &&
         FMath::IsNearlyEqual(Left.FramesPerSecondNumerator,
                              Right.FramesPerSecondNumerator) &&
         FMath::IsNearlyEqual(Left.MinimumDeltaSeconds,
                              Right.MinimumDeltaSeconds) &&
         FMath::IsNearlyEqual(Left.InitialDeltaSeconds,
                              Right.InitialDeltaSeconds) &&
         Left.InitialFramesPerSecond == Right.InitialFramesPerSecond &&
         Left.EmptyStackDepth == Right.EmptyStackDepth &&
         Left.EmptyPolyCount == Right.EmptyPolyCount &&
         Left.EmptyTriangleCount == Right.EmptyTriangleCount &&
         Left.EmptyMemoryMegabytes == Right.EmptyMemoryMegabytes &&
         Left.MemoryBytesPerMegabyte == Right.MemoryBytesPerMegabyte &&
         Left.RhiStatsGpuIndex == Right.RhiStatsGpuIndex &&
         Left.RhiStatsMinimumGpuIndex == Right.RhiStatsMinimumGpuIndex &&
         Left.RhiStatsMaximumGpuIndex == Right.RhiStatsMaximumGpuIndex &&
         Left.IdleWhenNotForegroundCVarName ==
             Right.IdleWhenNotForegroundCVarName &&
         Left.MaxFpsCVarName == Right.MaxFpsCVarName &&
         Left.VsyncCVarName == Right.VsyncCVarName &&
         FMath::IsNearlyEqual(Left.SecondsToMilliseconds,
                              Right.SecondsToMilliseconds) &&
         Left.DiagnosticFalseIntValue == Right.DiagnosticFalseIntValue &&
         Left.DiagnosticTrueIntValue == Right.DiagnosticTrueIntValue &&
         Left.DiagnosticDefaultIntValue == Right.DiagnosticDefaultIntValue &&
         FMath::IsNearlyEqual(Left.DiagnosticDefaultFloatValue,
                              Right.DiagnosticDefaultFloatValue) &&
         Left.MeshLodIndex == Right.MeshLodIndex &&
         Left.ForcedLodAutomaticModel == Right.ForcedLodAutomaticModel &&
         Left.LodModelIndexOffset == Right.LodModelIndexOffset &&
         Left.ProcMeshFirstSectionIndex == Right.ProcMeshFirstSectionIndex &&
         Left.ProcMeshSectionStep == Right.ProcMeshSectionStep &&
         Left.TriangleIndexDivisor == Right.TriangleIndexDivisor &&
         Left.ZOrder == Right.ZOrder && Left.FontSize == Right.FontSize &&
         Left.FramesPerSecondMediumThreshold ==
             Right.FramesPerSecondMediumThreshold &&
         Left.FramesPerSecondHighThreshold ==
             Right.FramesPerSecondHighThreshold &&
         Left.StackDepthMediumThreshold == Right.StackDepthMediumThreshold &&
         Left.StackDepthHighThreshold == Right.StackDepthHighThreshold &&
         Left.PolyCountMediumThreshold == Right.PolyCountMediumThreshold &&
         Left.PolyCountHighThreshold == Right.PolyCountHighThreshold &&
         Left.MemoryMediumThreshold == Right.MemoryMediumThreshold &&
         Left.MemoryHighThreshold == Right.MemoryHighThreshold &&
         Left.bRemoveDpIScale == Right.bRemoveDpIScale &&
         Left.bAutoWrapText == Right.bAutoWrapText &&
         Left.bBudgetScreenshotCreateDirectoryTree ==
             Right.bBudgetScreenshotCreateDirectoryTree &&
         Left.bBudgetScreenshotShowUI == Right.bBudgetScreenshotShowUI &&
         Left.bBudgetScreenshotAddFilenameSuffix ==
             Right.bBudgetScreenshotAddFilenameSuffix &&
         Left.bDiagnosticAllowFrameRateSmoothing ==
             Right.bDiagnosticAllowFrameRateSmoothing &&
         Left.PanelColor == Right.PanelColor &&
         Left.TextColor == Right.TextColor &&
         Left.LowValueColor == Right.LowValueColor &&
         Left.MediumValueColor == Right.MediumValueColor &&
         Left.HighValueColor == Right.HighValueColor;
}

inline bool operator!=(const FStatsOverlaySettings &Left,
                       const FStatsOverlaySettings &Right) {
  return !(Left == Right);
}

} // namespace Data
} // namespace Game
} // namespace ForbocAI
