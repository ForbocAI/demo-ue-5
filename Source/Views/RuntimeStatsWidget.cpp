// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/RuntimeStatsWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/HorizontalBox.h"
#include "Components/SkinnedMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Core/frmt.hpp"
#include "Core/ue_fp.hpp"
#include "Engine/Engine.h"
#include "Engine/SkinnedAsset.h"
#include "Engine/StaticMesh.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ProceduralMeshComponent.h"
#include "Rendering/SkeletalMeshRenderData.h"
#include "Store.h"

namespace FG = ForbocAI::Game::Level;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntimeBudget, Log, All);

namespace {

using FRuntimeStatsOverlaySettings =
    ForbocAI::Game::Data::FRuntimeStatsOverlaySettings;

struct FRuntimeStatsViewModel {
  int32 FramesPerSecond;
  int32 StackDepth;
  int64 PolyCount;
};

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

struct FRuntimeStatsLodIndexRequest {
  int32 ForcedLodModel;
  int32 AutomaticLodIndex;
  const FRuntimeStatsOverlaySettings *Settings;
};

struct FRuntimeStatsLodClampRequest {
  int32 LodIndex;
  int32 LodCount;
  const FRuntimeStatsOverlaySettings *Settings;
};

const FRuntimeStatsOverlaySettings &SelectStatsOverlaySettings() {
  return FG::RuntimeSelectors::SelectUIRuntimeSettings(
             FG::Store::GetStore().getState())
      .StatsOverlay;
}

int32 SelectFramesPerSecond(float DeltaSeconds,
                            const FRuntimeStatsOverlaySettings &Settings) {
  return DeltaSeconds > Settings.MinimumDeltaSeconds
             ? FMath::RoundToInt(Settings.FramesPerSecondNumerator /
                                 DeltaSeconds)
             : Settings.InitialFramesPerSecond;
}

int32 SelectEcsStackDepth(const ecs::FWorld &World,
                          const FRuntimeStatsOverlaySettings &Settings) {
  TArray<ecs::FDomainNode> Nodes;
  World.Domains.Nodes.GenerateValueArray(Nodes);
  return func::fold_array<ecs::FDomainNode, int32>(
      Nodes, Settings.EmptyStackDepth,
      [](const int32 &Depth, const ecs::FDomainNode &Node) {
        return FMath::Max(Depth, Node.Path.Segments.Num());
      });
}

int32 SelectRuntimeStatsLodIndex(const FRuntimeStatsLodIndexRequest &Request) {
  check(Request.Settings);
  return Request.ForcedLodModel > Request.Settings->ForcedLodAutomaticModel
             ? Request.ForcedLodModel - Request.Settings->LodModelIndexOffset
             : Request.AutomaticLodIndex;
}

int32 ClampRuntimeStatsLodIndex(const FRuntimeStatsLodClampRequest &Request) {
  check(Request.Settings);
  return FMath::Clamp(
      Request.LodIndex, Request.Settings->ForcedLodAutomaticModel,
      Request.LodCount - Request.Settings->LodModelIndexOffset);
}

bool ShouldCountPrimitiveComponent(UPrimitiveComponent *Component) {
  return Component != nullptr && Component->IsVisible() &&
         !Component->bHiddenInGame && Component->GetOwner() != nullptr &&
         !Component->GetOwner()->IsHidden();
}

int64 CountStaticMeshTriangles(UStaticMeshComponent *Component,
                               const FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Component, ShouldCountPrimitiveComponent(
                                               Component)),
      [&Settings](UStaticMeshComponent *ComponentValue) {
        UStaticMesh *Mesh = ComponentValue->GetStaticMesh();
        return func::match(
            func::from_nullable_value(Mesh, Mesh != nullptr),
            [ComponentValue, &Settings](UStaticMesh *MeshValue) -> int64 {
              const int32 LodIndex = ClampRuntimeStatsLodIndex(
                  {SelectRuntimeStatsLodIndex(
                       {ComponentValue->GetForcedLodModel(),
                        Settings.MeshLodIndex, &Settings}),
                   MeshValue->GetNumLODs(), &Settings});
              return static_cast<int64>(MeshValue->GetNumTriangles(LodIndex));
            },
            [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
      },
      [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
}

int64 CountProceduralMeshSectionTriangles(
    UProceduralMeshComponent &Component, int32 SectionIndex,
    const FRuntimeStatsOverlaySettings &Settings);

int64 CountExistingProceduralMeshSectionTriangles(
    UProceduralMeshComponent &Component, int32 SectionIndex,
    const FRuntimeStatsOverlaySettings &Settings) {
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
    const FRuntimeStatsOverlaySettings &Settings) {
  return SectionIndex >= Component.GetNumSections()
             ? static_cast<int64>(Settings.EmptyTriangleCount)
             : CountExistingProceduralMeshSectionTriangles(Component,
                                                           SectionIndex,
                                                           Settings);
}

int64 CountProceduralMeshTriangles(
    UProceduralMeshComponent *Component,
    const FRuntimeStatsOverlaySettings &Settings) {
  return func::match(
      func::from_nullable_value(Component, ShouldCountPrimitiveComponent(
                                               Component)),
      [&Settings](UProceduralMeshComponent *ComponentValue) -> int64 {
        return CountProceduralMeshSectionTriangles(
            *ComponentValue, Settings.ProcMeshFirstSectionIndex, Settings);
      },
      [&Settings]() -> int64 { return Settings.EmptyTriangleCount; });
}

int64 CountSkinnedMeshTriangles(USkinnedMeshComponent *Component,
                                const FRuntimeStatsOverlaySettings &Settings) {
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
                            {SelectRuntimeStatsLodIndex(
                                 {ComponentValue->GetForcedLOD(),
                                  ComponentValue->GetPredictedLODLevel(),
                                  &Settings}),
                             RenderData->LODRenderData.Num(), &Settings})
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
    int64 (*CountTriangles)(Component *,
                            const FRuntimeStatsOverlaySettings &),
    const FRuntimeStatsOverlaySettings &Settings) {
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

int64 CountActorTriangles(AActor *Actor,
                          const FRuntimeStatsOverlaySettings &Settings) {
  return CountActorComponentTriangles<UStaticMeshComponent>(
             Actor, CountStaticMeshTriangles, Settings) +
         CountActorComponentTriangles<UProceduralMeshComponent>(
             Actor, CountProceduralMeshTriangles, Settings) +
         CountActorComponentTriangles<USkinnedMeshComponent>(
             Actor, CountSkinnedMeshTriangles, Settings);
}

int64 CountWorldTriangles(UWorld *World,
                          const FRuntimeStatsOverlaySettings &Settings) {
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

FRuntimeStatsViewModel
SelectRuntimeStats(UWorld *World, float DeltaSeconds,
                   const FRuntimeStatsOverlaySettings &Settings,
                   int64 PolyCount) {
  const FG::FRuntimeState State = FG::Store::GetStore().getState();
  const ecs::FWorld &EcsWorld = FG::RuntimeSelectors::SelectWorld(State);
  return {SelectFramesPerSecond(DeltaSeconds, Settings),
          SelectEcsStackDepth(EcsWorld, Settings),
          PolyCount};
}

FString FormatRuntimeStatsValue(int64 Value,
                                const FRuntimeStatsOverlaySettings &Settings) {
  return frmt::RuntimeString(Settings.ValueFormat,
                             frmt::Args({frmt::Arg(Value)}));
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

void LogRuntimeBudgetSample(const FRuntimeStatsViewModel &Stats) {
  UE_LOG(LogForbocRuntimeBudget, Display,
         TEXT("runtime-budget sample fps=%d stack_depth=%d poly_count=%lld"),
         Stats.FramesPerSecond, Stats.StackDepth, Stats.PolyCount);
}

FString FormatRuntimeStatsDebugMessage(
    const FRuntimeStatsViewModel &Stats,
    const FRuntimeStatsOverlaySettings &Settings) {
  return frmt::RuntimeString(
      Settings.DebugMessageFormat,
      frmt::Args({frmt::Arg(Stats.FramesPerSecond),
                  frmt::Arg(Stats.StackDepth), frmt::Arg(Stats.PolyCount)}));
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
  BudgetLogLastSeconds = BudgetClockSeconds - Settings.BudgetLogIntervalSeconds;
  BudgetScreenshotIntervalSeconds =
      FG::RenderingSlice::SelectRuntimeBudgetScreenshotIntervalSeconds(
          Settings);
  BudgetScreenshotLastSeconds =
      BudgetClockSeconds - BudgetScreenshotIntervalSeconds;
  BudgetScreenshotIndex = Settings.BudgetScreenshotInitialIndex;
  CachedPolyCount = Settings.EmptyPolyCount;
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
  CachedPolyCount =
      bRefreshPolyCount ? CountWorldTriangles(GetWorld(), Settings)
                        : CachedPolyCount;
  PolyCountRefreshElapsedSeconds =
      bRefreshPolyCount ? Settings.IntervalResetElapsedSeconds
                        : PolyCountRefreshElapsedSeconds;

  const bool bRefreshStats =
      ShouldRunInterval(StatsRefreshElapsedSeconds,
                        Settings.StatsRefreshIntervalSeconds);
  const FRuntimeStatsViewModel Stats =
      SelectRuntimeStats(GetWorld(), WallDeltaSeconds, Settings,
                         CachedPolyCount);
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
         PresentRuntimeStatsDebugMessage(Stats, Settings),
         StatsRefreshElapsedSeconds = Settings.IntervalResetElapsedSeconds,
         void())
      : void();

  const bool bLogBudgetSample =
      FG::RenderingSlice::ShouldRunRuntimeBudgetWallInterval(
          BudgetClockSeconds, BudgetLogLastSeconds,
          Settings.BudgetLogIntervalSeconds);
  bLogBudgetSample ? (LogRuntimeBudgetSample(Stats),
                      BudgetLogLastSeconds = BudgetClockSeconds,
                      void())
                   : void();

  const bool bRequestBudgetScreenshot =
      FG::RenderingSlice::ShouldRunRuntimeBudgetScreenshot(
          BudgetClockSeconds, BudgetScreenshotLastSeconds,
          BudgetScreenshotIntervalSeconds, Settings);
  bRequestBudgetScreenshot
      ? (BudgetScreenshotIndex += Settings.BudgetScreenshotIndexStep,
         FG::RenderingSlice::RequestRuntimeBudgetScreenshot(
             Settings, BudgetScreenshotIndex),
         BudgetScreenshotLastSeconds = BudgetClockSeconds,
         void())
      : void();
}
