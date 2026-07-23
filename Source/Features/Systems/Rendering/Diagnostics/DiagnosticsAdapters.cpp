#include "Features/Systems/Rendering/Diagnostics/DiagnosticsAdapters.h"

#include "Core/frmt.hpp"
#include "Features/Systems/Rendering/Diagnostics/DiagnosticsSelectors.h"
#include "Features/Systems/SystemsSelectors.h"
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

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select command line float through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn float SelectCommandLineFloat(const FString &Key, float DefaultValue) */
float SelectCommandLineFloat(const FString &Key, float DefaultValue) {
  float Value = DefaultValue;
  const bool bHasValue = FParse::Value(FCommandLine::Get(), *Key, Value);
  return bHasValue ? Value : DefaultValue;
}


/** User Story: As a systems rendering diagnostics consumer, I need to invoke find rendering diagnostic console variable through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn func::Maybe<IConsoleVariable *> FindRenderingDiagnosticConsoleVariable(const FString &Name) */
func::Maybe<IConsoleVariable *>
FindRenderingDiagnosticConsoleVariable(const FString &Name) {
  IConsoleVariable *Found =
      IConsoleManager::Get().FindConsoleVariable(*Name);
  return func::from_nullable_value<IConsoleVariable *>(Found, Found != nullptr);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select console variable int through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn int32 SelectConsoleVariableInt(const FString &Name, int32 DefaultValue) */
int32 SelectConsoleVariableInt(const FString &Name, int32 DefaultValue) {
  return func::match(
      FindRenderingDiagnosticConsoleVariable(Name),
      [](IConsoleVariable *Found) { return Found->GetInt(); },
      [DefaultValue]() { return DefaultValue; });
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select console variable float through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn float SelectConsoleVariableFloat(const FString &Name, float DefaultValue) */
float SelectConsoleVariableFloat(const FString &Name, float DefaultValue) {
  return func::match(
      FindRenderingDiagnosticConsoleVariable(Name),
      [](IConsoleVariable *Found) { return Found->GetFloat(); },
      [DefaultValue]() { return DefaultValue; });
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime frame pacing stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeFramePacingStats SelectRuntimeFramePacingStats( const FFramePacingQuery &Query, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeFramePacingStats SelectRuntimeFramePacingStats(
    const FFramePacingQuery &Query,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  UGameUserSettings *GameUserSettings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  return {
      {RenderingSelectors::SelectRuntimeMilliseconds(Query.WallDeltaSeconds,
                                                      Settings),
       RenderingSelectors::SelectRuntimeMilliseconds(Query.DeltaSeconds,
                                                      Settings),
       Query.StatsSelectionMilliseconds, Query.PolyCountMilliseconds,
       RenderingSelectors::SelectRuntimeMilliseconds(FApp::GetIdleTime(),
                                                      Settings),
       RenderingSelectors::SelectRuntimeMilliseconds(
           FApp::GetIdleTimeOvershoot(), Settings)},
      {SelectConsoleVariableFloat(Settings.MaxFpsCVarName,
                                  Settings.DiagnosticDefaultFloatValue),
       GameUserSettings != nullptr
           ? GameUserSettings->GetFrameRateLimit()
           : Settings.DiagnosticDefaultFloatValue,
       GEngine != nullptr
           ? GEngine->GetMaxTickRate(
                 Query.DeltaSeconds,
                 Settings.bDiagnosticAllowFrameRateSmoothing)
           : Settings.DiagnosticDefaultFloatValue,
       GEngine != nullptr ? GEngine->FixedFrameRate
                          : Settings.DiagnosticDefaultFloatValue},
      {RenderingSelectors::SelectIntFromBool(
           GEngine != nullptr && GEngine->bUseFixedFrameRate, Settings),
       RenderingSelectors::SelectIntFromBool(FApp::UseFixedTimeStep(),
                                             Settings),
       RenderingSelectors::SelectRuntimeMilliseconds(FApp::GetFixedDeltaTime(),
                                                      Settings),
       SelectConsoleVariableInt(Settings.VsyncCVarName,
                                Settings.DiagnosticDefaultIntValue),
       SelectConsoleVariableInt(Settings.IdleWhenNotForegroundCVarName,
                                Settings.DiagnosticDefaultIntValue),
       RenderingSelectors::SelectIntFromBool(FApp::HasFocus(), Settings)},
      {RenderingSelectors::SelectIntFromBool(
           GEngine != nullptr && GEngine->ShouldThrottleCPUUsage(), Settings),
       RenderingSelectors::SelectIntFromBool(
           GEngine != nullptr && GEngine->AreAllWindowsHidden(), Settings)}};
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke runtime budget screenshot directory through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FString RuntimeBudgetScreenshotDirectory( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FString RuntimeBudgetScreenshotDirectory(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return FPaths::Combine(FPaths::ProjectDir(),
                         Settings.BudgetScreenshotDirectory);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke runtime budget screenshot path through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FString RuntimeBudgetScreenshotPath( const ForbocAI::Game::Data::FOverlaySettings &Settings, int32 Index) */
FString RuntimeBudgetScreenshotPath(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index) {
  return FPaths::Combine(
      RuntimeBudgetScreenshotDirectory(Settings),
      frmt::RuntimeString(Settings.BudgetScreenshotFileNameFormat,
                          frmt::Args({frmt::Arg(Index)})));
}

} // namespace

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime budget screenshot interval seconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn float SelectRuntimeBudgetScreenshotIntervalSeconds( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
float SelectRuntimeBudgetScreenshotIntervalSeconds(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  return SelectCommandLineFloat(
      Settings.BudgetScreenshotIntervalCommandLineKey,
      Settings.BudgetScreenshotIntervalSeconds);
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime budget clock seconds through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn double SelectRuntimeBudgetClockSeconds() */
double SelectRuntimeBudgetClockSeconds() { return FPlatformTime::Seconds(); }

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime memory stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeMemoryStats SelectRuntimeMemoryStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
  return {RenderingSelectors::SelectMemoryMegabytes(Stats.UsedPhysical, Settings),
          RenderingSelectors::SelectMemoryMegabytes(Stats.PeakUsedPhysical, Settings),
          RenderingSelectors::SelectMemoryMegabytes(Stats.UsedVirtual, Settings)};
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime frame timing stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats( const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
  const int32 GpuIndex = RenderingSelectors::SelectRuntimeGpuIndex(Settings);
  return {RenderingSelectors::SelectThreadMilliseconds(GGameThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(GRenderThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(GRHIThreadTime),
          RenderingSelectors::SelectThreadMilliseconds(RHIGetGPUFrameCycles(GpuIndex)),
          GNumDrawCallsRHI[GpuIndex], GNumPrimitivesDrawnRHI[GpuIndex]};
}


/** User Story: As a systems rendering diagnostics consumer, I need to invoke select runtime stats through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn FRuntimeStatsViewModel SelectRuntimeStats( UWorld *, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount, double PolyCountMilliseconds, const ForbocAI::Game::Data::FOverlaySettings &Settings) */
FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount,
    double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FOverlaySettings &Settings) {
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
  return {{RenderingSelectors::SelectRuntimeStatsFramesPerSecond(
               WallDeltaSeconds, Settings),
           RenderingSelectors::SelectRuntimeStatsStackDepth(EcsWorld,
                                                             Settings),
           PolyCount},
          MemoryStats,
          TimingStats,
          PacingStats,
          {ReducerDiagnostics.RootReducerMilliseconds,
           ReducerDiagnostics.CombinedReducerMilliseconds},
          {ReducerDiagnostics.ProjectionMilliseconds,
           ReducerDiagnostics.ProjectedEntityCount,
           ReducerDiagnostics.ProjectedComponentTypeCount}};
}

/** User Story: As a systems rendering diagnostics consumer, I need to invoke request runtime budget screenshot through a stable signature so the systems rendering diagnostics workflow remains explicit and composable. @fn void RequestRuntimeBudgetScreenshot( const ForbocAI::Game::Data::FOverlaySettings &Settings, int32 Index) */
void RequestRuntimeBudgetScreenshot(
    const ForbocAI::Game::Data::FOverlaySettings &Settings,
    int32 Index) {
  const FString Directory = RuntimeBudgetScreenshotDirectory(Settings);
  IFileManager::Get().MakeDirectory(
      *Directory, Settings.bBudgetScreenshotCreateDirectoryTree);
  const FString Path = RuntimeBudgetScreenshotPath(Settings, Index);
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
