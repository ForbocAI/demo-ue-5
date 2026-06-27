#include "Views/RuntimeLevelView.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Features/Entities/Characters/Bots/TalkableTownsperson.h"
#include "Features/Entities/Characters/Bots/WalkingHorse.h"
#include "Features/Systems/Level/LevelRuntimeAdapters.h"
#include "Features/Systems/Level/LevelReducers.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Terrain/TerrainReducers.h"
#include "Materials/MaterialInterface.h"
#include "Views/TerrainMeshView.h"
#include "UObject/ConstructorHelpers.h"

namespace FG = ForbocAI::Demo::Level;

ARuntimeLevelView::ARuntimeLevelView()
    : bSpawnOnBeginPlay(true), CubeMesh(nullptr), BlockBaseMaterial(nullptr),
      RuntimeSession() {
  PrimaryActorTick.bCanEverTick = false;

  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
      TEXT("/Engine/BasicShapes/Cube.Cube"));
  CubeMesh = CubeAsset.Succeeded() ? CubeAsset.Object : nullptr;

  BlockBaseMaterial = FG::RenderingActions::LoadBlockoutMaterial();
}

void ARuntimeLevelView::BeginPlay() {
  Super::BeginPlay();
  bSpawnOnBeginPlay ? RenderLevel() : void();
}

void ARuntimeLevelView::RenderLevel() {
  FG::RenderingActions::ApplyRuntimeProfile();
  TerrainData.LoadFromContent();
  OrthoData.LoadFromContent();
  RuntimeLayout = FG::LevelSystemAdapters::BuildFrenchGulchRuntimeLayoutSeed();
  RuntimeSession.Seed(TerrainData, OrthoData);

  RenderTerrain();
  RenderNaturalEnvironment();
  RenderTown();
  RenderMineApproach();
  RenderTownspeople();
  RenderHorses();
  RenderSection(FG::LevelSystemReducers::BuildOverlaySectionSpawn(
      RuntimeLayout, TerrainData));
}

void ARuntimeLevelView::RenderTerrain() {
  UWorld *World = GetWorld();
  func::match(
      func::from_nullable_value(World, World != nullptr && TerrainData.IsLoaded()),
      [this](UWorld *WorldValue) {
        ATerrainMeshView *TerrainMesh =
            WorldValue->SpawnActor<ATerrainMeshView>(FVector::ZeroVector,
                                                     FRotator::ZeroRotator);
        TerrainMesh ? (TerrainMesh->ApplyTerrainMeshPayload(
                           FG::TerrainReducers::BuildTerrainMeshPayload(
                               TerrainData, OrthoData)),
                       true)
                    : false;
      },
      [this]() {
        RenderBlock(FG::LevelSystemReducers::BuildRuntimeBlockSpawn(
            RuntimeLayout.FallbackTerrainBlock, TerrainData));
      });

  RenderSection(FG::LevelSystemReducers::BuildRuntimeSectionSpawn(
      RuntimeLayout.Terrain, TerrainData));
}

void ARuntimeLevelView::RenderNaturalEnvironment() {
  RenderSection(FG::LevelSystemReducers::BuildNatureSectionSpawn(
      RuntimeSession.SelectNatureFeatures(), TerrainData));
}

void ARuntimeLevelView::RenderTown() {
  RenderSection(FG::LevelSystemReducers::BuildLandmarkSectionSpawn(
      RuntimeSession.SelectLandmarks()));
  RenderSection(FG::LevelSystemReducers::BuildRuntimeSectionSpawn(
      RuntimeLayout.Town, TerrainData));
}

void ARuntimeLevelView::RenderMineApproach() {
  RenderSection(FG::LevelSystemReducers::BuildRuntimeSectionSpawn(
      RuntimeLayout.Mine, TerrainData));
}

void ARuntimeLevelView::RenderTownspeople() {
  RenderTownspersonSeeds(RuntimeSession.SelectTownspeople(), 0);
}

void ARuntimeLevelView::RenderHorses() {
  RenderHorseSeeds(RuntimeSession.SelectHorses(), 0);
}

void ARuntimeLevelView::RenderSection(
    const FG::FLevelRuntimeSectionSpawn &Section) {
  RenderBlocks(Section.Blocks, 0);
  RenderLabels(Section.Labels, 0);
}

void ARuntimeLevelView::RenderBlocks(
    const TArray<FG::FLevelBlockSpawn> &Blocks, int32 Index) {
  Index >= Blocks.Num()
      ? void()
      : (RenderBlock(Blocks[Index]), RenderBlocks(Blocks, Index + 1));
}

void ARuntimeLevelView::RenderLabels(
    const TArray<FG::FLevelLabelSpawn> &Labels, int32 Index) {
  Index >= Labels.Num()
      ? void()
      : (RenderLabel(Labels[Index]), RenderLabels(Labels, Index + 1));
}

void ARuntimeLevelView::RenderTownspersonSeeds(
    const TArray<FG::FTownspersonSeed> &Townspeople, int32 Index) {
  Index >= Townspeople.Num()
      ? void()
      : (RenderTownsperson(Townspeople[Index]),
         RenderTownspersonSeeds(Townspeople, Index + 1));
}

void ARuntimeLevelView::RenderHorseSeeds(
    const TArray<FG::FHorseRouteSeed> &Horses, int32 Index) {
  Index >= Horses.Num()
      ? void()
      : (RenderHorse(Horses[Index]), RenderHorseSeeds(Horses, Index + 1));
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
                BlockSpawn.Location, FRotator::ZeroRotator, Params);
        return Block
                   ? (Block->SetActorScale3D(BlockSpawn.Scale),
                      Block->GetStaticMeshComponent()->SetStaticMesh(CubeMesh),
                      FG::RenderingActions::ApplyTexture(
                          {Block->GetStaticMeshComponent(), BlockBaseMaterial,
                           BlockSpawn.Texture}),
                      Block->GetStaticMeshComponent()->SetMobility(
                          EComponentMobility::Static),
                      Block)
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
                 true)
              : false;
      },
      []() {});
}

ATalkableTownsperson *ARuntimeLevelView::RenderTownsperson(
    const FG::FTownspersonSeed &Seed) {
  const TArray<FVector> Route =
      FG::LevelSystemReducers::BuildWorldRoute(Seed.PatrolRoute, TerrainData);
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(World, World != nullptr && Route.Num() > 0),
      [&Seed, &Route](UWorld *WorldValue) -> ATalkableTownsperson * {
        ATalkableTownsperson *Townsperson =
            WorldValue->SpawnActor<ATalkableTownsperson>(
                Route[0], FRotator::ZeroRotator);
        FTownspersonConfig Config;
        Config.Name = Seed.Name;
        Config.Role = Seed.Role;
        Config.Persona = Seed.Persona;
        Config.InteractionPrompt = Seed.InteractionPrompt;
        Config.DefaultPlayerLine = Seed.DefaultPlayerLine;
        Config.PinnedResponse = Seed.PinnedResponse;
        Config.PatrolRoute = Route;
        return Townsperson ? (Townsperson->ConfigureTownsperson(Config),
                              Townsperson)
                           : nullptr;
      },
      []() -> ATalkableTownsperson * { return nullptr; });
}

AWalkingHorse *ARuntimeLevelView::RenderHorse(
    const FG::FHorseRouteSeed &Seed) {
  const TArray<FVector> Route =
      FG::LevelSystemReducers::BuildWorldRoute(Seed.PatrolRoute, TerrainData);
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(World, World != nullptr && Route.Num() > 0),
      [&Seed, &Route](UWorld *WorldValue) -> AWalkingHorse * {
        AWalkingHorse *Horse = WorldValue->SpawnActor<AWalkingHorse>(
            Route[0], FRotator::ZeroRotator);
        FWalkingHorseConfig Config;
        Config.Name = Seed.Name;
        Config.PatrolRoute = Route;
        Config.bMountedRider = Seed.bMountedRider;
        return Horse ? (Horse->ConfigureHorse(Config), Horse) : nullptr;
      },
      []() -> AWalkingHorse * { return nullptr; });
}
