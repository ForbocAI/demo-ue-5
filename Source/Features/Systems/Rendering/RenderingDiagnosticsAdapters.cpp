#include "Features/Systems/Rendering/RenderingDiagnosticsAdapters.h"

#include "Features/Systems/Rendering/RenderingDiagnosticsSelectors.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/GameUserSettings.h"
#include "HAL/FileManager.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformMemory.h"
#include "HAL/PlatformTime.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/App.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"
#include "Misc/Paths.h"
#include "RenderTimer.h"
#include "RHIStats.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRenderingDiagnostics, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingAdapters {
namespace {

float SelectCommandLineFloat(const FString &Key, float DefaultValue) {
  float Value = DefaultValue;
  const bool bHasValue = FParse::Value(FCommandLine::Get(), *Key, Value);
  return bHasValue ? Value : DefaultValue;
}


func::Maybe<IConsoleVariable *>
FindRenderingDiagnosticConsoleVariable(const FString &Name) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

int32 SelectConsoleVariableInt(const FString &Name, int32 DefaultValue) {
  return func::match(
      FindRenderingDiagnosticConsoleVariable(Name),
      [](IConsoleVariable *Found) { return Found->GetInt(); },
      [DefaultValue]() { return DefaultValue; });
}

float SelectConsoleVariableFloat(const FString &Name, float DefaultValue) {
  return func::match(
      FindRenderingDiagnosticConsoleVariable(Name),
      [](IConsoleVariable *Found) { return Found->GetFloat(); },
      [DefaultValue]() { return DefaultValue; });
}

FRuntimeFramePacingStats SelectRuntimeFramePacingStats(
    const FFramePacingQuery &Query,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  UGameUserSettings *GameUserSettings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  return {
      RenderingSelectors::SelectRuntimeMilliseconds(Query.WallDeltaSeconds, Settings),
      RenderingSelectors::SelectRuntimeMilliseconds(Query.DeltaSeconds, Settings),
      Query.StatsSelectionMilliseconds,
      Query.PolyCountMilliseconds,
      RenderingSelectors::SelectRuntimeMilliseconds(FApp::GetIdleTime(), Settings),
      RenderingSelectors::SelectRuntimeMilliseconds(FApp::GetIdleTimeOvershoot(), Settings),
      SelectConsoleVariableFloat(Settings.MaxFpsCVarName,
                                 Settings.DiagnosticDefaultFloatValue),
      GameUserSettings != nullptr
          ? GameUserSettings->GetFrameRateLimit()
          : Settings.DiagnosticDefaultFloatValue,
      GEngine != nullptr
          ? GEngine->GetMaxTickRate(
                Query.DeltaSeconds, Settings.bDiagnosticAllowFrameRateSmoothing)
          : Settings.DiagnosticDefaultFloatValue,
      RenderingSelectors::SelectIntFromBool(GEngine != nullptr && GEngine->bUseFixedFrameRate,
                        Settings),
      GEngine != nullptr ? GEngine->FixedFrameRate
                         : Settings.DiagnosticDefaultFloatValue,
      RenderingSelectors::SelectIntFromBool(FApp::UseFixedTimeStep(), Settings),
      RenderingSelectors::SelectRuntimeMilliseconds(FApp::GetFixedDeltaTime(), Settings),
      SelectConsoleVariableInt(Settings.VsyncCVarName,
                               Settings.DiagnosticDefaultIntValue),
      SelectConsoleVariableInt(Settings.IdleWhenNotForegroundCVarName,
                               Settings.DiagnosticDefaultIntValue),
      RenderingSelectors::SelectIntFromBool(FApp::HasFocus(), Settings),
      RenderingSelectors::SelectIntFromBool(GEngine != nullptr && GEngine->ShouldThrottleCPUUsage(),
                        Settings),
      RenderingSelectors::SelectIntFromBool(GEngine != nullptr && GEngine->AreAllWindowsHidden(),
                        Settings)};
}

} // namespace

float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return SelectCommandLineFloat(
      Settings.BudgetScreenshotIntervalCommandLineKey,
      Settings.BudgetScreenshotIntervalSeconds);
}

double SelectRuntimeBudgetClockSeconds() { return FPlatformTime::Seconds(); }

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
  return {RenderingSelectors::SelectMemoryMegabytes(Stats.UsedPhysical, Settings),
          RenderingSelectors::SelectMemoryMegabytes(Stats.PeakUsedPhysical, Settings),
          RenderingSelectors::SelectMemoryMegabytes(Stats.UsedVirtual, Settings)};
}

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const int32 GpuIndex = RenderingSelectors::SelectRuntimeGpuIndex(Settings);
  return {RenderingSelectors::SelectThreadMilliseconds(GGameThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(GRenderThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(GRHIThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(RHIGetGPUFrameCycles(GpuIndex)),
          GNumDrawCallsRHI[GpuIndex], GNumPrimitivesDrawnRHI[GpuIndex]};
}


FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount,
    double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const double StartedSeconds = SelectRuntimeBudgetClockSeconds();
  const FRuntimeState &State = RuntimeSelectors::SelectState();
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectWorld(State);
  const FRuntimeReducerDiagnosticsState &ReducerDiagnostics =
      RuntimeSelectors::SelectRuntimeReducerDiagnostics(State);
  const FRuntimeMemoryStats MemoryStats = SelectRuntimeMemoryStats(Settings);
  const FRuntimeFrameTimingStats TimingStats =
      SelectRuntimeFrameTimingStats(Settings);
  const double SelectedSeconds = SelectRuntimeBudgetClockSeconds();
  const double StatsSelectionMilliseconds =
      RenderingSelectors::SelectRuntimeElapsedMilliseconds(
          {StartedSeconds, SelectedSeconds}, Settings);
  const FRuntimeFramePacingStats PacingStats = SelectRuntimeFramePacingStats(
      {DeltaSeconds, WallDeltaSeconds, StatsSelectionMilliseconds,
       PolyCountMilliseconds}, Settings);
  return {RenderingSelectors::SelectRuntimeStatsFramesPerSecond(WallDeltaSeconds, Settings),
          RenderingSelectors::SelectRuntimeStatsStackDepth(EcsWorld, Settings),
          PolyCount,
          MemoryStats.UsedPhysicalMegabytes,
          MemoryStats.PeakUsedPhysicalMegabytes,
          MemoryStats.UsedVirtualMegabytes,
          TimingStats.GameThreadMilliseconds,
          TimingStats.RenderThreadMilliseconds,
          TimingStats.RhiThreadMilliseconds,
          TimingStats.GpuMilliseconds,
          TimingStats.DrawCalls,
          TimingStats.RhiPrimitives,
          PacingStats.WallDeltaMilliseconds,
          PacingStats.InputDeltaMilliseconds,
          PacingStats.StatsSelectionMilliseconds,
          PacingStats.PolyCountMilliseconds,
          PacingStats.EngineIdleMilliseconds,
          PacingStats.EngineIdleOvershootMilliseconds,
          PacingStats.MaxFps,
          PacingStats.FrameRateLimit,
          PacingStats.EffectiveMaxTickRate,
          PacingStats.FixedFrameRateEnabled,
          PacingStats.FixedFrameRate,
          PacingStats.FixedTimeStepEnabled,
          PacingStats.FixedDeltaMilliseconds,
          PacingStats.VsyncEnabled,
          PacingStats.IdleWhenNotForegroundEnabled,
          PacingStats.AppHasFocus,
          PacingStats.CpuThrottleEnabled,
          PacingStats.AllWindowsHidden,
          ReducerDiagnostics.LastRootReducerMilliseconds,
          ReducerDiagnostics.LastCombinedReducerMilliseconds,
          ReducerDiagnostics.LastProjectionMilliseconds,
          ReducerDiagnostics.LastProjectedEntityCount,
          ReducerDiagnostics.LastProjectedComponentTypeCount};
}

void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  const FString Directory = RenderingSelectors::RuntimeBudgetScreenshotDirectory(Settings);
  IFileManager::Get().MakeDirectory(
      *Directory, Settings.bBudgetScreenshotCreateDirectoryTree);
  const FString Path = RenderingSelectors::RuntimeBudgetScreenshotPath(Settings, Index);
  FScreenshotRequest::RequestScreenshot(
      Path, Settings.bBudgetScreenshotShowUI,
      Settings.bBudgetScreenshotAddFilenameSuffix);
  UE_LOG(LogForbocRenderingDiagnostics, Display,
         TEXT("runtime-budget screenshot requested path=%s"), *Path);
}

} // namespace RenderingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
