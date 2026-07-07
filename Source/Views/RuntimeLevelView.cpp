// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/RuntimeLevelView.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Views/TownspersonView.h"
#include "Views/HorseView.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeActions.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Materials/MaterialInterface.h"
#include "Views/TerrainMeshView.h"

namespace FG = ForbocAI::Game::Level;

DEFINE_LOG_CATEGORY_STATIC(LogForbocRuntime, Log, All);

namespace {

struct FBlockLevelOfDetailApplyRequest {
  UStaticMeshComponent *Component;
  ForbocAI::Game::Level::FLevelDistanceLodStage Lod;
};

void ApplyBlockLevelOfDetail(const FBlockLevelOfDetailApplyRequest &Request) {
  check(Request.Component);
  Request.Component->SetForcedLodModel(
      Request.Lod.StaticMeshForcedLodModel);
  Request.Component->SetCullDistance(Request.Lod.CullDistance);
  Request.Component->SetVisibility(Request.Lod.bStaticVisible);
  Request.Component->SetHiddenInGame(!Request.Lod.bStaticVisible);
  Request.Component->SetCollisionEnabled(Request.Lod.bCollisionEnabled
                                             ? ECollisionEnabled::QueryOnly
                                             : ECollisionEnabled::NoCollision);
  Request.Component->SetCastShadow(Request.Lod.bCastShadow);
  Request.Component->SetComponentTickEnabled(false);
}

} // namespace

ARuntimeLevelView::ARuntimeLevelView()
    : CubeMesh(nullptr), BlockBaseMaterial(nullptr) {
  PrimaryActorTick.bCanEverTick = false;

  const FG::FRuntimeState &State = FG::RuntimeSelectors::SelectState();
  const FG::FRenderingAssetPaths AssetPaths =
      FG::RuntimeSelectors::SelectRenderingAssetPaths(State);
  CubeMesh = LoadObject<UStaticMesh>(nullptr, *AssetPaths.LevelCubeMeshPath);
  BlockBaseMaterial =
      FG::RenderingSlice::LoadBlockoutMaterial(AssetPaths.BlockoutMaterialPath);
  LevelGeometrySettings = FG::RuntimeSelectors::SelectLevelGeometry(State);
  bSpawnOnBeginPlay = LevelGeometrySettings.bRuntimeSpawnOnBeginPlay;
  TextureCatalog = FG::RuntimeSelectors::SelectTextureCatalog(State);
  RenderingRuntimeSettings =
      FG::RuntimeSelectors::SelectRenderingRuntimeSettings(State);
}

void ARuntimeLevelView::BeginPlay() {
  Super::BeginPlay();
  UE_LOG(LogForbocRuntime, Display,
         TEXT("Forboc runtime smoke: RuntimeLevelView BeginPlay"));
  bSpawnOnBeginPlay ? RenderLevel() : void();
}

void ARuntimeLevelView::RenderLevel() {
  const FG::FLevelRetroRenderProfile Profile =
      FG::RuntimeSelectors::SelectRuntimeProfile(
          FG::RuntimeSelectors::SelectState());
  FG::RenderingSlice::ApplyRuntimeProfile(
      {GetWorld(), Profile,
       FG::RuntimeSelectors::SelectRenderingRuntimeSettings(
           FG::RuntimeSelectors::SelectState())});
  auto Payload = FG::FRuntimeLevelViewPayload();
  const auto Result = FG::RuntimeActions::Dispatch(
      FG::RuntimeSlice::RequestLevelViewPayload());
  func::thenAsync(Result, [&Payload](auto Resolved) {
    Payload = Resolved;
  });
  func::executeAsync(Result);
  TextureCatalog = FG::RuntimeSelectors::SelectTextureCatalog(
      FG::RuntimeSelectors::SelectState());
  LevelGeometrySettings = FG::RuntimeSelectors::SelectLevelGeometry(
      FG::RuntimeSelectors::SelectState());
  RenderingRuntimeSettings =
      FG::RuntimeSelectors::SelectRenderingRuntimeSettings(
          FG::RuntimeSelectors::SelectState());
  RenderLevelPayload(Payload);
}

void ARuntimeLevelView::RenderLevelPayload(
    const FG::FRuntimeLevelViewPayload &Payload) {
  RenderTerrain(Payload);
  RenderSections(Payload.Sections, LevelGeometrySettings.RuntimeFirstRenderIndex);
  RenderTownspeople(Payload.Townspeople,
                    LevelGeometrySettings.RuntimeFirstRenderIndex);
  RenderHorses(Payload.Horses, LevelGeometrySettings.RuntimeFirstRenderIndex);
}

void ARuntimeLevelView::RenderTerrain(
    const FG::FRuntimeLevelViewPayload &Payload) {
  UWorld *World = GetWorld();
  func::match(
      func::from_nullable_value(World,
                                World != nullptr &&
                                    Payload.bTerrainMeshLoaded),
      [this, &Payload](UWorld *WorldValue) {
        ATerrainMeshView *TerrainMesh =
            WorldValue->SpawnActor<ATerrainMeshView>(FVector::ZeroVector,
                                                     FRotator::ZeroRotator);
        TerrainMesh ? (TerrainMesh->ApplyTerrainMeshPayload(
                           Payload.TerrainMesh),
                       true)
                    : false;
      },
      []() {});
}

void ARuntimeLevelView::RenderSections(
    const TArray<FG::FLevelRuntimeSectionSpawn> &Sections, int32 Index) {
  Index >= Sections.Num()
      ? void()
      : (RenderSection(Sections[Index]),
         RenderSections(Sections, Index + LevelGeometrySettings.RuntimeIndexStep));
}

void ARuntimeLevelView::RenderSection(
    const FG::FLevelRuntimeSectionSpawn &Section) {
  RenderBlocks(Section.Blocks, LevelGeometrySettings.RuntimeFirstRenderIndex);
  RenderLabels(Section.Labels, LevelGeometrySettings.RuntimeFirstRenderIndex);
}

void ARuntimeLevelView::RenderBlocks(
    const TArray<FG::FLevelBlockSpawn> &Blocks, int32 Index) {
  Index >= Blocks.Num()
      ? void()
      : (RenderBlock(Blocks[Index]),
         RenderBlocks(Blocks, Index + LevelGeometrySettings.RuntimeIndexStep));
}

void ARuntimeLevelView::RenderLabels(
    const TArray<FG::FLevelLabelSpawn> &Labels, int32 Index) {
  Index >= Labels.Num()
      ? void()
      : (RenderLabel(Labels[Index]),
         RenderLabels(Labels, Index + LevelGeometrySettings.RuntimeIndexStep));
}

void ARuntimeLevelView::RenderTownspeople(
    const TArray<FG::FRuntimeTownspersonViewSpawn> &Townspeople,
    int32 Index) {
  Index >= Townspeople.Num()
      ? void()
      : (RenderTownsperson(Townspeople[Index]),
         RenderTownspeople(Townspeople,
                           Index + LevelGeometrySettings.RuntimeIndexStep));
}

void ARuntimeLevelView::RenderHorses(
    const TArray<FG::FRuntimeHorseViewSpawn> &Horses, int32 Index) {
  Index >= Horses.Num()
      ? void()
      : (RenderHorse(Horses[Index]),
         RenderHorses(Horses, Index + LevelGeometrySettings.RuntimeIndexStep));
}

AStaticMeshActor *ARuntimeLevelView::RenderBlock(
    const FG::FLevelBlockSpawn &BlockSpawn) {
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(World, World != nullptr && CubeMesh != nullptr),
      [this, &BlockSpawn](UWorld *WorldValue) -> AStaticMeshActor * {
        FActorSpawnParameters Params;
        Params.Name = MakeUniqueObjectName(
            WorldValue, AStaticMeshActor::StaticClass(),
            FName(*BlockSpawn.Name));

        AStaticMeshActor *Block =
            WorldValue->SpawnActor<AStaticMeshActor>(
                BlockSpawn.Location, BlockSpawn.Rotation, Params);
        return Block
                   ? ([this, &BlockSpawn, Block]() -> AStaticMeshActor * {
                       UStaticMeshComponent *MeshComponent =
                           Block->GetStaticMeshComponent();
                       check(MeshComponent);
                       Block->SetActorScale3D(BlockSpawn.Scale);
                       MeshComponent->SetStaticMesh(CubeMesh);
                       ApplyBlockLevelOfDetail({MeshComponent,
                                                BlockSpawn.Lod});
                       FG::RenderingSlice::ApplyTexture(
                           {MeshComponent, BlockBaseMaterial,
                            BlockSpawn.Texture, TextureCatalog,
                            RenderingRuntimeSettings});
                       MeshComponent->SetMobility(EComponentMobility::Static);
                       return Block;
                     }())
                   : nullptr;
      },
      []() -> AStaticMeshActor * { return nullptr; });
}

void ARuntimeLevelView::RenderLabel(
    const FG::FLevelLabelSpawn &LabelSpawn) {
  UWorld *World = GetWorld();
  func::match(
      func::from_nullable_value(World, World != nullptr),
      [&LabelSpawn](UWorld *WorldValue) {
        ATextRenderActor *Label = WorldValue->SpawnActor<ATextRenderActor>(
            LabelSpawn.Location, FRotator::ZeroRotator);
        Label ? (Label->GetTextRender()->SetText(
                     FText::FromString(LabelSpawn.Text)),
                 Label->GetTextRender()->SetHorizontalAlignment(EHTA_Center),
                 Label->GetTextRender()->SetWorldSize(LabelSpawn.WorldSize),
                 Label->GetTextRender()->SetVisibility(
                     LabelSpawn.Lod.bLabelsVisible),
                 Label->GetTextRender()->SetHiddenInGame(
                     !LabelSpawn.Lod.bLabelsVisible),
                 Label->GetTextRender()->SetComponentTickEnabled(false),
                 Label->SetActorTickEnabled(false),
                 true)
              : false;
      },
      []() {});
}

ATownspersonView *ARuntimeLevelView::RenderTownsperson(
    const FG::FRuntimeTownspersonViewSpawn &Spawn) {
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(
          World, World != nullptr &&
                     Spawn.PatrolRoute.Num() >=
                         LevelGeometrySettings.PatrolRouteRequiredPointCount),
      [this, &Spawn](UWorld *WorldValue) -> ATownspersonView * {
        ATownspersonView *Townsperson =
            WorldValue->SpawnActor<ATownspersonView>(
                Spawn.PatrolRoute[LevelGeometrySettings.InitialPatrolRouteIndex],
                FRotator::ZeroRotator);
        FTownspersonViewConfig Config;
        Config.Id = Spawn.Id;
        Config.Name = Spawn.Name;
        Config.Role = Spawn.Role;
        Config.Persona = Spawn.Persona;
        Config.InteractionPrompt = Spawn.InteractionPrompt;
        Config.DefaultPlayerLine = Spawn.DefaultPlayerLine;
        Config.PinnedResponse = Spawn.PinnedResponse;
        Config.PatrolRoute = Spawn.PatrolRoute;
        Config.Lod = Spawn.Lod;
        return Townsperson ? (Townsperson->ConfigureTownsperson(Config),
                              Townsperson)
                           : nullptr;
      },
      []() -> ATownspersonView * { return nullptr; });
}

AHorseView *ARuntimeLevelView::RenderHorse(
    const FG::FRuntimeHorseViewSpawn &Spawn) {
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(
          World, World != nullptr &&
                     Spawn.PatrolRoute.Num() >=
                         LevelGeometrySettings.PatrolRouteRequiredPointCount),
      [this, &Spawn](UWorld *WorldValue) -> AHorseView * {
        AHorseView *Horse = WorldValue->SpawnActor<AHorseView>(
            Spawn.PatrolRoute[LevelGeometrySettings.InitialPatrolRouteIndex],
            FRotator::ZeroRotator);
        FHorseViewConfig Config;
        Config.Name = Spawn.Name;
        Config.PatrolRoute = Spawn.PatrolRoute;
        Config.bMountedRider = Spawn.bMountedRider;
        Config.Lod = Spawn.Lod;
        return Horse ? (Horse->ConfigureHorse(Config), Horse) : nullptr;
      },
      []() -> AHorseView * { return nullptr; });
}
