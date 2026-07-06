#include "Features/Systems/Rendering/RenderingThunks.h"

#include "Components/SkinnedMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "DynamicRHI.h"
#include "Engine/Engine.h"
#include "Engine/SkinnedAsset.h"
#include "Engine/StaticMesh.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/Actor.h"
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
#include "ProceduralMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "RenderTimer.h"
#include "RHIStats.h"
#include "Store.h"

DEFINE_LOG_CATEGORY_STATIC(LogForbocRenderingDiagnostics, Log, All);

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingThunks {
namespace {

bool ShouldRunInterval(float ElapsedSeconds, float IntervalSeconds) {
  return ElapsedSeconds >= IntervalSeconds;
}

float SelectCommandLineFloat(const FString &Key, float DefaultValue) {
  float Value = DefaultValue;
  const bool bHasValue = FParse::Value(FCommandLine::Get(), *Key, Value);
  return bHasValue ? Value : DefaultValue;
}

FString RuntimeBudgetScreenshotDirectory(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FPaths::Combine(FPaths::ProjectDir(),
                         Settings.BudgetScreenshotDirectory);
}

FString RuntimeBudgetScreenshotPath(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings,
    int32 Index) {
  return FPaths::Combine(
      RuntimeBudgetScreenshotDirectory(Settings),
      frmt::RuntimeString(Settings.BudgetScreenshotFileNameFormat,
                          frmt::Args({frmt::Arg(Index)})));
}

int32 SelectIntFromBool(
    bool bValue,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return bValue ? Settings.DiagnosticTrueIntValue
                : Settings.DiagnosticFalseIntValue;
}

double SelectRuntimeMilliseconds(
    double Seconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return Seconds * Settings.SecondsToMilliseconds;
}

double SelectRuntimeElapsedMilliseconds(
    double StartedSeconds,
    double FinishedSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return SelectRuntimeMilliseconds(FinishedSeconds - StartedSeconds, Settings);
}

int32 SelectRuntimeStatsFramesPerSecond(
    float DeltaSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return DeltaSeconds > Settings.MinimumDeltaSeconds
             ? FMath::RoundToInt(Settings.FramesPerSecondNumerator /
                                 DeltaSeconds)
             : Settings.InitialFramesPerSecond;
}

int32 SelectRuntimeStatsStackDepth(
    const ecs::FWorld &World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  TArray<ecs::FDomainNode> Nodes;
  World.Domains.Nodes.GenerateValueArray(Nodes);
  return func::fold_array<ecs::FDomainNode, int32>(
      Nodes, Settings.EmptyStackDepth,
      [](const int32 &Depth, const ecs::FDomainNode &Node) {
        return FMath::Max(Depth, Node.Path.Segments.Num());
      });
}

int32 SelectRuntimeStatsLodIndex(
    int32 ForcedLodModel, int32 AutomaticLodIndex,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return ForcedLodModel > Settings.ForcedLodAutomaticModel
             ? ForcedLodModel - Settings.LodModelIndexOffset
             : AutomaticLodIndex;
}

int32 ClampRuntimeStatsLodIndex(
    int32 LodIndex, int32 LodCount,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FMath::Clamp(LodIndex, Settings.ForcedLodAutomaticModel,
                      LodCount - Settings.LodModelIndexOffset);
}

bool ShouldCountPrimitiveComponent(UPrimitiveComponent *Component) {
  return Component != nullptr && Component->IsVisible() &&
         !Component->bHiddenInGame && Component->GetOwner() != nullptr &&
         !Component->GetOwner()->IsHidden();
}

int64 CountStaticMeshTriangles(
    UStaticMeshComponent *Component,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Component, ShouldCountPrimitiveComponent(
                                               Component)),
      [&Settings](UStaticMeshComponent *ComponentValue) {
        UStaticMesh *Mesh = ComponentValue->GetStaticMesh();
        return func::match(
            func::from_nullable_value(Mesh, Mesh != nullptr),
            [ComponentValue, &Settings](UStaticMesh *MeshValue) -> int64 {
              const int32 LodIndex = ClampRuntimeStatsLodIndex(
                  SelectRuntimeStatsLodIndex(
                      ComponentValue->GetForcedLodModel(),
                      Settings.MeshLodIndex, Settings),
                  MeshValue->GetNumLODs(), Settings);
              return static_cast<int64>(MeshValue->GetNumTriangles(LodIndex));
            },
            [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
      },
      [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
}

int64 CountProceduralMeshSectionTriangles(
    UProceduralMeshComponent &Component, int32 SectionIndex,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings);

int64 CountExistingProceduralMeshSectionTriangles(
    UProceduralMeshComponent &Component, int32 SectionIndex,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  FProcMeshSection *Section = Component.GetProcMeshSection(SectionIndex);
  return func::match(
      func::from_nullable_value(Section, Section != nullptr),
      [&Component, SectionIndex, &Settings](FProcMeshSection *SectionValue)
          -> int64 {
        return static_cast<int64>(SectionValue->ProcIndexBuffer.Num() /
                                  Settings.TriangleIndexDivisor) +
               CountProceduralMeshSectionTriangles(
                   Component, SectionIndex + Settings.ProcMeshSectionStep,
                   Settings);
      },
      [&Component, SectionIndex, &Settings]() -> int64 {
        return CountProceduralMeshSectionTriangles(
            Component, SectionIndex + Settings.ProcMeshSectionStep, Settings);
      });
}

int64 CountProceduralMeshSectionTriangles(
    UProceduralMeshComponent &Component, int32 SectionIndex,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return SectionIndex >= Component.GetNumSections()
             ? static_cast<int64>(Settings.EmptyTriangleCount)
             : CountExistingProceduralMeshSectionTriangles(Component,
                                                           SectionIndex,
                                                           Settings);
}

int64 CountProceduralMeshTriangles(
    UProceduralMeshComponent *Component,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Component, ShouldCountPrimitiveComponent(
                                               Component)),
      [&Settings](UProceduralMeshComponent *ComponentValue) -> int64 {
        return CountProceduralMeshSectionTriangles(
            *ComponentValue, Settings.ProcMeshFirstSectionIndex, Settings);
      },
      [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
}

int64 CountSkinnedMeshTriangles(
    USkinnedMeshComponent *Component,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Component, ShouldCountPrimitiveComponent(
                                               Component)),
      [&Settings](USkinnedMeshComponent *ComponentValue) {
        USkinnedAsset *Asset = ComponentValue->GetSkinnedAsset();
        return func::match(
            func::from_nullable_value(Asset, Asset != nullptr),
            [ComponentValue, &Settings](USkinnedAsset *AssetValue) -> int64 {
              const FSkeletalMeshRenderData *RenderData =
                  AssetValue->GetResourceForRendering();
              const int32 LodIndex =
                  RenderData != nullptr
                      ? ClampRuntimeStatsLodIndex(
                            SelectRuntimeStatsLodIndex(
                                ComponentValue->GetForcedLOD(),
                                ComponentValue->GetPredictedLODLevel(),
                                Settings),
                            RenderData->LODRenderData.Num(), Settings)
                      : Settings.MeshLodIndex;
              return RenderData != nullptr &&
                             RenderData->LODRenderData.IsValidIndex(
                                 LodIndex)
                         ? static_cast<int64>(
                               RenderData->LODRenderData[LodIndex]
                                   .GetTotalFaces())
                         : static_cast<int64>(Settings.EmptyTriangleCount);
            },
            [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
      },
      [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
}

template <typename Component>
int64 CountActorComponentTriangles(
    AActor *Actor,
    int64 (*CountTriangles)(
        Component *,
        const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &),
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Actor, Actor != nullptr),
      [CountTriangles, &Settings](AActor *ActorValue) {
        TArray<Component *> Components;
        ActorValue->GetComponents<Component>(Components);
        return func::fold_array<Component *, int64>(
            Components, Settings.EmptyPolyCount,
            [CountTriangles,
             &Settings](const int64 &Total, Component *MeshComponent) {
              return Total + CountTriangles(MeshComponent, Settings);
            });
      },
      [&Settings]() -> int64 { return Settings.EmptyPolyCount; });
}

int64 CountActorTriangles(
    AActor *Actor,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return CountActorComponentTriangles<UStaticMeshComponent>(
             Actor, CountStaticMeshTriangles, Settings) +
         CountActorComponentTriangles<UProceduralMeshComponent>(
             Actor, CountProceduralMeshTriangles, Settings) +
         CountActorComponentTriangles<USkinnedMeshComponent>(
             Actor, CountSkinnedMeshTriangles, Settings);
}

int64 CountWorldTriangles(
    UWorld *World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(World, World != nullptr),
      [&Settings](UWorld *WorldValue) {
        TArray<AActor *> Actors;
        UGameplayStatics::GetAllActorsOfClass(WorldValue, AActor::StaticClass(),
                                              Actors);
        return func::fold_array<AActor *, int64>(
            Actors, Settings.EmptyPolyCount,
            [&Settings](const int64 &Total, AActor *Actor) {
              return Total + CountActorTriangles(Actor, Settings);
            });
      },
      [&Settings]() -> int64 { return Settings.EmptyPolyCount; });
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

int64 SelectMemoryMegabytes(
    uint64 Bytes,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  check(Settings.MemoryBytesPerMegabyte > Settings.EmptyMemoryMegabytes);
  return static_cast<int64>(
      Bytes / static_cast<uint64>(Settings.MemoryBytesPerMegabyte));
}

double SelectThreadMilliseconds(uint32 Cycles) {
  return FPlatformTime::ToMilliseconds(Cycles);
}

int32 SelectRuntimeGpuIndex(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return FMath::Clamp(Settings.RhiStatsGpuIndex,
                      Settings.RhiStatsMinimumGpuIndex,
                      Settings.RhiStatsMaximumGpuIndex);
}

FRuntimeFramePacingStats SelectRuntimeFramePacingStats(
    float DeltaSeconds, double WallDeltaSeconds,
    double StatsSelectionMilliseconds, double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  UGameUserSettings *GameUserSettings =
      GEngine != nullptr ? GEngine->GetGameUserSettings() : nullptr;
  return {
      SelectRuntimeMilliseconds(WallDeltaSeconds, Settings),
      SelectRuntimeMilliseconds(DeltaSeconds, Settings),
      StatsSelectionMilliseconds,
      PolyCountMilliseconds,
      SelectRuntimeMilliseconds(FApp::GetIdleTime(), Settings),
      SelectRuntimeMilliseconds(FApp::GetIdleTimeOvershoot(), Settings),
      SelectConsoleVariableFloat(Settings.MaxFpsCVarName,
                                 Settings.DiagnosticDefaultFloatValue),
      GameUserSettings != nullptr
          ? GameUserSettings->GetFrameRateLimit()
          : Settings.DiagnosticDefaultFloatValue,
      GEngine != nullptr
          ? GEngine->GetMaxTickRate(
                DeltaSeconds, Settings.bDiagnosticAllowFrameRateSmoothing)
          : Settings.DiagnosticDefaultFloatValue,
      SelectIntFromBool(GEngine != nullptr && GEngine->bUseFixedFrameRate,
                        Settings),
      GEngine != nullptr ? GEngine->FixedFrameRate
                         : Settings.DiagnosticDefaultFloatValue,
      SelectIntFromBool(FApp::UseFixedTimeStep(), Settings),
      SelectRuntimeMilliseconds(FApp::GetFixedDeltaTime(), Settings),
      SelectConsoleVariableInt(Settings.VsyncCVarName,
                               Settings.DiagnosticDefaultIntValue),
      SelectConsoleVariableInt(Settings.IdleWhenNotForegroundCVarName,
                               Settings.DiagnosticDefaultIntValue),
      SelectIntFromBool(FApp::HasFocus(), Settings),
      SelectIntFromBool(GEngine != nullptr && GEngine->ShouldThrottleCPUUsage(),
                        Settings),
      SelectIntFromBool(GEngine != nullptr && GEngine->AreAllWindowsHidden(),
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

bool ShouldRunRuntimeBudgetWallInterval(double CurrentSeconds,
                                        double LastSeconds,
                                        float IntervalSeconds) {
  return IntervalSeconds > 0.0f &&
         CurrentSeconds - LastSeconds >= IntervalSeconds;
}

bool ShouldRunRuntimeBudgetScreenshot(
    double CurrentSeconds, double LastSeconds, float IntervalSeconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  return IntervalSeconds > Settings.BudgetScreenshotDisabledIntervalSeconds &&
         ShouldRunRuntimeBudgetWallInterval(CurrentSeconds, LastSeconds,
                                            IntervalSeconds);
}

FRuntimeMemoryStats SelectRuntimeMemoryStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const FPlatformMemoryStats Stats = FPlatformMemory::GetStats();
  return {SelectMemoryMegabytes(Stats.UsedPhysical, Settings),
          SelectMemoryMegabytes(Stats.PeakUsedPhysical, Settings),
          SelectMemoryMegabytes(Stats.UsedVirtual, Settings)};
}

FRuntimeFrameTimingStats SelectRuntimeFrameTimingStats(
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const int32 GpuIndex = SelectRuntimeGpuIndex(Settings);
  return {SelectThreadMilliseconds(GGameThreadTime),
          SelectThreadMilliseconds(GRenderThreadTime),
          SelectThreadMilliseconds(GRHIThreadTime),
          SelectThreadMilliseconds(RHIGetGPUFrameCycles(GpuIndex)),
          GNumDrawCallsRHI[GpuIndex], GNumPrimitivesDrawnRHI[GpuIndex]};
}

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const double StartedSeconds = SelectRuntimeBudgetClockSeconds();
  const int64 PolyCount = CountWorldTriangles(World, Settings);
  const double FinishedSeconds = SelectRuntimeBudgetClockSeconds();
  return {PolyCount, SelectRuntimeElapsedMilliseconds(StartedSeconds,
                                                      FinishedSeconds,
                                                      Settings)};
}

FRuntimeStatsViewModel SelectRuntimeStats(
    UWorld *, float DeltaSeconds, double WallDeltaSeconds, int64 PolyCount,
    double PolyCountMilliseconds,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const double StartedSeconds = SelectRuntimeBudgetClockSeconds();
  const FRuntimeState &State = Store::GetStore().getState();
  const ecs::FWorld &EcsWorld = RuntimeSelectors::SelectWorld(State);
  const FRuntimeReducerDiagnosticsState &ReducerDiagnostics =
      RuntimeSelectors::SelectRuntimeReducerDiagnostics(State);
  const FRuntimeMemoryStats MemoryStats = SelectRuntimeMemoryStats(Settings);
  const FRuntimeFrameTimingStats TimingStats =
      SelectRuntimeFrameTimingStats(Settings);
  const double SelectedSeconds = SelectRuntimeBudgetClockSeconds();
  const double StatsSelectionMilliseconds =
      SelectRuntimeElapsedMilliseconds(StartedSeconds, SelectedSeconds,
                                       Settings);
  const FRuntimeFramePacingStats PacingStats = SelectRuntimeFramePacingStats(
      DeltaSeconds, WallDeltaSeconds, StatsSelectionMilliseconds,
      PolyCountMilliseconds, Settings);
  return {SelectRuntimeStatsFramesPerSecond(WallDeltaSeconds, Settings),
          SelectRuntimeStatsStackDepth(EcsWorld, Settings),
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

} // namespace RenderingThunks
} // namespace Level
} // namespace Game
} // namespace ForbocAI
