#include "Views/RuntimeLevelView.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Views/TownspersonView.h"
#include "Views/HorseView.h"
#include "Features/Systems/Rendering/RenderingSlice.h"
#include "Features/Systems/Runtime/RuntimeSlice.h"
#include "Materials/MaterialInterface.h"
#include "Store.h"
#include "Views/TerrainMeshView.h"
#include "UObject/ConstructorHelpers.h"

namespace FG = ForbocAI::Demo::Level;

ARuntimeLevelView::ARuntimeLevelView()
    : bSpawnOnBeginPlay(true), CubeMesh(nullptr), BlockBaseMaterial(nullptr) {
  PrimaryActorTick.bCanEverTick = false;

  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
      TEXT("/Engine/BasicShapes/Cube.Cube"));
  CubeMesh = CubeAsset.Succeeded() ? CubeAsset.Object : nullptr;

  BlockBaseMaterial = FG::RenderingSlice::LoadBlockoutMaterial();
}

void ARuntimeLevelView::BeginPlay() {
  Super::BeginPlay();
  bSpawnOnBeginPlay ? RenderLevel() : void();
}

void ARuntimeLevelView::RenderLevel() {
  FG::RenderingSlice::ApplyRuntimeProfile();
  auto Payload = FG::FRuntimeLevelViewPayload();
  const auto Result = FG::Store::GetStore().dispatch(
      FG::RuntimeSlice::RequestLevelViewPayload());
  func::thenAsync(Result, [&Payload](auto Resolved) {
    Payload = Resolved;
  });
  func::executeAsync(Result);
  RenderLevelPayload(Payload);
}

void ARuntimeLevelView::RenderLevelPayload(
    const FG::FRuntimeLevelViewPayload &Payload) {
  RenderTerrain(Payload);
  RenderSections(Payload.Sections, 0);
  RenderTownspeople(Payload.Townspeople, 0);
  RenderHorses(Payload.Horses, 0);
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
      [this, &Payload]() {
        RenderBlock(Payload.FallbackTerrainBlock);
      });
}

void ARuntimeLevelView::RenderSections(
    const TArray<FG::FLevelRuntimeSectionSpawn> &Sections, int32 Index) {
  Index >= Sections.Num()
      ? void()
      : (RenderSection(Sections[Index]), RenderSections(Sections, Index + 1));
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

void ARuntimeLevelView::RenderTownspeople(
    const TArray<FG::FRuntimeTownspersonViewSpawn> &Townspeople,
    int32 Index) {
  Index >= Townspeople.Num()
      ? void()
      : (RenderTownsperson(Townspeople[Index]),
         RenderTownspeople(Townspeople, Index + 1));
}

void ARuntimeLevelView::RenderHorses(
    const TArray<FG::FRuntimeHorseViewSpawn> &Horses, int32 Index) {
  Index >= Horses.Num()
      ? void()
      : (RenderHorse(Horses[Index]), RenderHorses(Horses, Index + 1));
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
                      FG::RenderingSlice::ApplyTexture(
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

ATownspersonView *ARuntimeLevelView::RenderTownsperson(
    const FG::FRuntimeTownspersonViewSpawn &Spawn) {
  UWorld *World = GetWorld();
  return func::match(
      func::from_nullable_value(
          World, World != nullptr && Spawn.PatrolRoute.Num() > 0),
      [&Spawn](UWorld *WorldValue) -> ATownspersonView * {
        ATownspersonView *Townsperson =
            WorldValue->SpawnActor<ATownspersonView>(
                Spawn.PatrolRoute[0], FRotator::ZeroRotator);
        FTownspersonViewConfig Config;
        Config.Name = Spawn.Name;
        Config.Role = Spawn.Role;
        Config.Persona = Spawn.Persona;
        Config.InteractionPrompt = Spawn.InteractionPrompt;
        Config.DefaultPlayerLine = Spawn.DefaultPlayerLine;
        Config.PinnedResponse = Spawn.PinnedResponse;
        Config.PatrolRoute = Spawn.PatrolRoute;
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
          World, World != nullptr && Spawn.PatrolRoute.Num() > 0),
      [&Spawn](UWorld *WorldValue) -> AHorseView * {
        AHorseView *Horse = WorldValue->SpawnActor<AHorseView>(
            Spawn.PatrolRoute[0], FRotator::ZeroRotator);
        FHorseViewConfig Config;
        Config.Name = Spawn.Name;
        Config.PatrolRoute = Spawn.PatrolRoute;
        Config.bMountedRider = Spawn.bMountedRider;
        return Horse ? (Horse->ConfigureHorse(Config), Horse) : nullptr;
      },
      []() -> AHorseView * { return nullptr; });
}
