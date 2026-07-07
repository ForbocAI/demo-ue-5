#include "Features/Systems/Rendering/RenderingDiagnosticsAdapters.h"

#include "Components/SkinnedMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Engine.h"
#include "Engine/SkinnedAsset.h"
#include "Engine/StaticMesh.h"
#include "Features/Systems/Rendering/RenderingDiagnosticsSelectors.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"

namespace ForbocAI {
namespace Game {
namespace Level {
namespace RenderingAdapters {
namespace {

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
              const int32 LodIndex = RenderingSelectors::ClampRuntimeStatsLodIndex(
                  {RenderingSelectors::SelectRuntimeStatsLodIndex(
                       {ComponentValue->GetForcedLodModel(),
                        Settings.MeshLodIndex}, Settings),
                   MeshValue->GetNumLODs()}, Settings);
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
                      ? RenderingSelectors::ClampRuntimeStatsLodIndex(
                            {RenderingSelectors::SelectRuntimeStatsLodIndex(
                                 {ComponentValue->GetForcedLOD(),
                                  ComponentValue->GetPredictedLODLevel()}, Settings),
                             RenderData->LODRenderData.Num()}, Settings)
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

} // namespace

FRuntimePolyCountStats SelectRuntimePolyCountStats(
    UWorld *World,
    const ForbocAI::Game::Data::FRuntimeStatsOverlaySettings &Settings) {
  const double StartedSeconds = SelectRuntimeBudgetClockSeconds();
  const int64 PolyCount = CountWorldTriangles(World, Settings);
  const double FinishedSeconds = SelectRuntimeBudgetClockSeconds();
  return {PolyCount, RenderingSelectors::SelectRuntimeElapsedMilliseconds(
                         {StartedSeconds, FinishedSeconds}, Settings)};
}

} // namespace RenderingAdapters
} // namespace Level
} // namespace Game
} // namespace ForbocAI
