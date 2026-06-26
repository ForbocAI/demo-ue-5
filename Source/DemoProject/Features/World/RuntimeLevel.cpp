#include "Features/World/RuntimeLevel.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/TextRenderActor.h"
#include "Features/Entities/Entities.h"
#include "Features/World/TerrainMesh.h"
#include "Features/Systems/RetroRendering.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace FG = ForbocAI::Demo::Map;
namespace FGL = ForbocAI::Demo::Map::MapLayout;

namespace ForbocAI {
namespace Demo {
namespace Map {
struct FMapBlockSpawn {
  FString Name;
  FVector Location;
  FVector Scale;
  EMapRetroTexture Texture;
};

struct FMapTerrainBlockSpawn {
  FString Name;
  FMapLocalPoint Point;
  FVector Scale;
  EMapRetroTexture Texture;
};
} // namespace Map
} // namespace Demo
} // namespace ForbocAI

namespace {
FG::FMapLocalPoint GroundLots(float EastLots, float NorthLots,
                                      const FVector &Scale,
                                      float Clearance) {
  return FGL::CenteredOnGround(FGL::FromPostOfficeLots(EastLots, NorthLots),
                               Scale, Clearance);
}

FG::FMapLocalPoint BuildingLots(float EastLots, float NorthLots,
                                        const FVector &Scale) {
  return GroundLots(EastLots, NorthLots, Scale, FGL::BuildingFoundationHeight());
}

FG::FMapLocalPoint FeatureLots(float EastLots, float NorthLots,
                                       const FVector &Scale) {
  return GroundLots(EastLots, NorthLots, Scale, FGL::RoadSurfaceClearance());
}

FG::FMapLocalPoint RouteLots(float EastLots, float NorthLots) {
  return FGL::FromPostOfficeLots(EastLots, NorthLots,
                                 FGL::CharacterHeightOffset());
}

FVector FallbackTerrainScale() {
  const float PlaneScale =
      FMapTerrainData::TerrainWorldSize / (FGL::CubeHalfExtent() * 2.0f);
  return FVector(PlaneScale, PlaneScale, FGL::RoadSurfaceClearance() /
                                        FGL::CubeHalfExtent());
}

FVector LabelLocationForBlock(const FG::FMapLandmark &Landmark) {
  return Landmark.Location +
         FVector::UpVector *
             (FGL::CubeHalfExtent() * Landmark.Scale.Z +
              FGL::BuildingFoundationHeight() + FGL::CubeHalfExtent());
}

FG::EMapRetroTexture BuildingTexture() {
  return FG::EMapRetroTexture::BuildingTimber;
}

FG::EMapRetroTexture RoadTexture() {
  return FG::EMapRetroTexture::RoadDust;
}

FG::EMapRetroTexture WaterTexture() {
  return FG::EMapRetroTexture::WaterCreek;
}

FG::EMapRetroTexture FoliageTexture() {
  return FG::EMapRetroTexture::FoliageRiparian;
}

FG::EMapRetroTexture RockTexture() {
  return FG::EMapRetroTexture::RockGranite;
}

FG::EMapRetroTexture MineTexture() {
  return FG::EMapRetroTexture::MineTimber;
}

FG::EMapRetroTexture MarkerTexture() {
  return FG::EMapRetroTexture::MarkerPaint;
}

FG::EMapRetroTexture
TextureForNatureKind(FG::ENatureFeatureKind Kind) {
  switch (Kind) {
  case FG::ENatureFeatureKind::Water:
    return WaterTexture();
  case FG::ENatureFeatureKind::Rock:
    return RockTexture();
  case FG::ENatureFeatureKind::TreeGrove:
  case FG::ENatureFeatureKind::Shrub:
    return FoliageTexture();
  case FG::ENatureFeatureKind::PCGMarker:
  case FG::ENatureFeatureKind::WaterSystemMarker:
    return MarkerTexture();
  }
  return MarkerTexture();
}
} // namespace

ARuntimeLevel::ARuntimeLevel()
    : bSpawnOnBeginPlay(true), CubeMesh(nullptr), BlockBaseMaterial(nullptr),
      MapStore(FG::MapSlice::ConfigureStore()) {
  PrimaryActorTick.bCanEverTick = false;

  static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
      TEXT("/Engine/BasicShapes/Cube.Cube"));
  CubeMesh = CubeAsset.Succeeded() ? CubeAsset.Object : nullptr;

  BlockBaseMaterial = FG::RetroRendering::LoadBlockoutMaterial();
}

void ARuntimeLevel::BeginPlay() {
  Super::BeginPlay();
  if (bSpawnOnBeginPlay) {
    SpawnLevel();
  }
}

void ARuntimeLevel::SpawnLevel() {
  FG::RetroRendering::ApplyRuntimeProfile();
  TerrainData.LoadFromContent();
  OrthoData.LoadFromContent();
  SeedMapStore();

  SpawnTerrain();
  SpawnNaturalEnvironment();
  SpawnTown();
  SpawnMineApproach();
  SpawnTownspeople();
  SpawnHorses();

  const FVector TitleScale = FGL::BuildingScaleFromFeet(40.0f, 20.0f, 1.9f);
  SpawnTerrainLabel(TEXT("French Gulch, California - 1899 runtime blockout"),
                    FGL::FromPostOfficeLots(1.85f, 1.3f,
                                            FGL::LabelHeightForScale(
                                                TitleScale)),
                    FGL::CubeHalfExtent() * 1.08f);
  SpawnTerrainLabel(TEXT("spawn: U.S. Post Office / 14200 Main St"),
                    FGL::AboveBlock(FGL::FromPostOfficeLots(0.0f, 0.0f),
                                    FGL::BuildingScaleFromFeet(34.0f, 19.0f,
                                                               1.5f)),
                    FGL::CubeHalfExtent() * 0.6f);
}

void ARuntimeLevel::SeedMapStore() {
  MapStore.dispatch(FG::MapActions::TerrainLoaded()(
      FG::MapFactories::TerrainLoadedPayload(
          {TerrainData.GetSourcePath(), OrthoData.GetSourcePath(),
           TerrainData.GetGridSize(), TerrainData.GetMinElevationMeters(),
           TerrainData.GetMaxElevationMeters()})));
  MapStore.dispatch(FG::MapActions::LandmarksSeeded()(
      FG::TownLandmarks::Build1899LandmarkSeed(TerrainData)));
  MapStore.dispatch(FG::MapActions::PlayerSpawnAnchored()(
      FG::MapFactories::SpawnPointPayload(
          {FGL::ToWorld(TerrainData, FGL::PlayerSpawnPoint()),
           FGL::PlayerSpawnRotation(), FGL::PlayerSpawnAnchorLabel()})));
}

void ARuntimeLevel::SpawnTerrain() {
  UWorld *World = GetWorld();
  if (World && TerrainData.IsLoaded()) {
    ATerrainMesh *TerrainMesh =
        World->SpawnActor<ATerrainMesh>(FVector::ZeroVector,
                                                   FRotator::ZeroRotator);
    if (TerrainMesh) {
      TerrainMesh->BuildFromTerrainData(TerrainData, OrthoData);
    }
  } else {
    const FVector Scale = FallbackTerrainScale();
    SpawnBlock({TEXT("Fallback terrain plane"),
                FVector(0.0f, 0.0f, -FGL::CubeHalfExtent() * Scale.Z), Scale,
                FoliageTexture()});
  }

  const FVector MainRoadScale = FGL::LongFeatureScale(24.0f, 22.0f, 1.6f);
  const FVector ApproachRoadScale = FGL::LongFeatureScale(20.0f, 7.0f, 1.6f);
  const FVector SpurRoadScale = FGL::LongFeatureScale(17.0f, 6.0f, 1.6f);
  const FVector CreekScale = FGL::LongFeatureScale(11.0f, 23.0f, 1.0f);
  const FVector RiparianScale = FGL::LongFeatureScale(22.0f, 23.0f, 0.8f);

  SpawnTerrainBlock({TEXT("Trinity Mountain / Main Street road"),
                     FeatureLots(2.55f, 0.0f, MainRoadScale), MainRoadScale,
                     RoadTexture()});
  SpawnTerrainBlock({TEXT("French Gulch Road south approach"),
                     FeatureLots(-0.1f, -5.9f, ApproachRoadScale),
                     ApproachRoadScale, RoadTexture()});
  SpawnTerrainBlock({TEXT("Cline Gulch road north spur"),
                     FeatureLots(-1.2f, 5.45f, SpurRoadScale), SpurRoadScale,
                     RoadTexture()});
  SpawnTerrainBlock({TEXT("Clear Creek corridor"),
                     FeatureLots(4.7f, 0.0f, CreekScale), CreekScale,
                     WaterTexture()});
  SpawnTerrainBlock({TEXT("Green riparian creek band"),
                     FeatureLots(5.2f, 0.0f, RiparianScale), RiparianScale,
                     FoliageTexture()});

  const FVector CemeteryScale = FGL::PadScaleFromFeet(40.0f, 26.0f, 2.0f);
  const FVector ParkScale = FGL::PadScaleFromFeet(36.0f, 38.0f, 1.6f);
  SpawnTerrainBlock({TEXT("IOOF cemetery terrace"),
                     FeatureLots(-0.425f, 2.55f, CemeteryScale),
                     CemeteryScale, RockTexture()});
  SpawnTerrainBlock({TEXT("French Gulch County Park green"),
                     FeatureLots(3.7f, 5.25f, ParkScale), ParkScale,
                     FoliageTexture()});

  SpawnTerrainLabel(TEXT("actual terrain: Mapzen/AWS Terrarium heightmap"),
                    FGL::FromPostOfficeLots(-3.1f, 6.15f,
                                            FGL::CharacterHeightOffset() * 3.5f),
                    FGL::CubeHalfExtent() * 0.56f);
  SpawnTerrainLabel(TEXT("USGS ortho imagery colors terrain mesh"),
                    FGL::FromPostOfficeLots(8.25f, -5.9f,
                                            FGL::CharacterHeightOffset() * 3.5f),
                    FGL::CubeHalfExtent() * 0.56f);
  SpawnTerrainLabel(TEXT("Clear Creek"),
                    FGL::FromPostOfficeLots(5.5f, -0.35f,
                                            FGL::CharacterHeightOffset() * 1.5f),
                    FGL::CubeHalfExtent() * 0.6f);
}

void ARuntimeLevel::SpawnNaturalEnvironment() {
  const TArray<FG::FNatureFeatureSeed> NatureFeatures =
      FG::NatureFeatures::BuildClearCreekNatureSeed();
  for (const FG::FNatureFeatureSeed &Feature : NatureFeatures) {
    SpawnTerrainBlock({Feature.Name, Feature.Location, Feature.Scale,
                       TextureForNatureKind(Feature.Kind)});
    if (Feature.Kind == FG::ENatureFeatureKind::PCGMarker ||
        Feature.Kind == FG::ENatureFeatureKind::WaterSystemMarker) {
      SpawnTerrainLabel(Feature.Name,
                        FGL::AboveBlock(Feature.Location, Feature.Scale),
                        FGL::CubeHalfExtent() * 0.36f);
    }
  }
}

void ARuntimeLevel::SpawnTown() {
  const TArray<FG::FMapLandmark> Landmarks =
      FG::MapSelectors::SelectLandmarks(MapStore.getState());
  for (const FG::FMapLandmark &Landmark : Landmarks) {
    SpawnBlock({Landmark.Label, Landmark.Location, Landmark.Scale,
                BuildingTexture()});
    SpawnLabel(Landmark.Label, LabelLocationForBlock(Landmark),
               FGL::CubeHalfExtent() * 0.48f);
  }

  const FVector PorchScale = FGL::PadScaleFromFeet(37.0f, 6.4f, 3.2f);
  const FVector CabinScale = FGL::BuildingScaleFromFeet(18.0f, 15.0f, 0.95f);
  const FVector CreekCabinScale =
      FGL::BuildingScaleFromFeet(19.0f, 14.4f, 0.95f);
  const FVector BarnScale = FGL::BuildingScaleFromFeet(34.0f, 28.0f, 1.2f);
  const FVector WagonYardScale = FGL::PadScaleFromFeet(33.0f, 33.0f, 1.6f);

  SpawnTerrainBlock({TEXT("Post office porch"),
                     BuildingLots(0.0f, -0.36f, PorchScale), PorchScale,
                     BuildingTexture()});
  SpawnTerrainBlock({TEXT("Cabin row west 1"),
                     BuildingLots(-0.975f, -1.275f, CabinScale), CabinScale,
                     BuildingTexture()});
  SpawnTerrainBlock({TEXT("Cabin row west 2"),
                     BuildingLots(-0.8f, 1.5f, CabinScale), CabinScale,
                     BuildingTexture()});
  SpawnTerrainBlock({TEXT("Cabin row creek 1"),
                     BuildingLots(4.55f, 0.4f, CreekCabinScale),
                     CreekCabinScale, BuildingTexture()});
  SpawnTerrainBlock({TEXT("Cabin row creek 2"),
                     BuildingLots(4.9f, 1.9f, CreekCabinScale),
                     CreekCabinScale, BuildingTexture()});
  SpawnTerrainBlock({TEXT("Barn and corral"),
                     BuildingLots(1.2f, -2.6f, BarnScale), BarnScale,
                     BuildingTexture()});
  SpawnTerrainBlock({TEXT("Wagon yard"),
                     FeatureLots(2.65f, -2.7f, WagonYardScale),
                     WagonYardScale, RoadTexture()});

  SpawnTerrainLabel(TEXT("cemetery"),
                    FGL::FromPostOfficeLots(-0.425f, 2.55f,
                                            FGL::CharacterHeightOffset() *
                                                1.6f),
                    FGL::CubeHalfExtent() * 0.48f);
}

void ARuntimeLevel::SpawnMineApproach() {
  const FVector MineRoadScale = FGL::LongFeatureScale(15.0f, 5.5f, 1.6f);
  const FVector MineEntranceScale =
      FGL::BuildingScaleFromFeet(31.0f, 14.0f, 1.7f);
  const FVector LintelScale = FGL::PadScaleFromFeet(38.0f, 3.6f, 4.4f);
  const FVector OreCartScale = FGL::PadScaleFromFeet(13.0f, 9.0f, 7.0f);
  const FVector RockPileScale = FGL::PadScaleFromFeet(22.0f, 17.0f, 6.4f);

  SpawnTerrainBlock({TEXT("Mine road up gulch"),
                     FeatureLots(-1.95f, 4.8f, MineRoadScale), MineRoadScale,
                     RoadTexture()});
  SpawnTerrainBlock({TEXT("Mine entrance"),
                     BuildingLots(-2.95f, 6.95f, MineEntranceScale),
                     MineEntranceScale, MineTexture()});
  SpawnTerrainBlock({TEXT("Mine timber lintel"),
                     BuildingLots(-2.95f, 6.65f, LintelScale), LintelScale,
                     BuildingTexture()});
  SpawnTerrainBlock({TEXT("Ore cart marker"),
                     BuildingLots(-2.05f, 5.7f, OreCartScale), OreCartScale,
                     MarkerTexture()});
  SpawnTerrainBlock({TEXT("Waste rock pile"),
                     BuildingLots(-2.5f, 6.25f, RockPileScale), RockPileScale,
                     RockTexture()});
  SpawnTerrainLabel(TEXT("mine approach"),
                    FGL::FromPostOfficeLots(-2.95f, 6.95f,
                                            FGL::LabelHeightForScale(
                                                MineEntranceScale)),
                    FGL::CubeHalfExtent() * 0.6f);
}

void ARuntimeLevel::SpawnTownspeople() {
  SpawnTownsperson(
      TEXT("Elias Reed"), TEXT("Miner"),
      TEXT("I know which shafts are paying and which timbers are tired."),
      BuildRoute({RouteLots(-1.8f, 5.3f), RouteLots(-2.825f, 6.725f),
                  RouteLots(-0.5f, 2.6f)}));

  SpawnTownsperson(
      TEXT("Clara Bell"), TEXT("Postmaster"),
      TEXT("I sort mail, freight news, and every rumor that rides Main Street."),
      BuildRoute({RouteLots(0.0f, -0.175f), RouteLots(0.9f, 0.4f),
                  RouteLots(-0.35f, 0.625f)}));

  SpawnTownsperson(
      TEXT("Deputy Hale"), TEXT("Deputy"),
      TEXT("I keep the peace between claim jumpers, teamsters, and drifters."),
      BuildRoute({RouteLots(2.3f, 1.8f), RouteLots(3.475f, 4.2f),
                  RouteLots(2.9f, -0.85f)}));

  SpawnTownsperson(
      TEXT("Mara Quinn"), TEXT("Traveler"),
      TEXT("I came over the ridge with news from Redding and a wary horse."),
      BuildRoute({RouteLots(1.525f, -2.5f), RouteLots(3.825f, 1.15f),
                  RouteLots(4.775f, -1.15f)}));

  SpawnTownsperson(
      TEXT("Samuel Pike"), TEXT("Hotel Keeper"),
      TEXT("Beds, meals, and claims gossip all pass through the hotel desk."),
      BuildRoute({RouteLots(3.85f, 1.175f), RouteLots(2.9f, -0.95f),
                  RouteLots(4.6f, 0.4f)}));
}

void ARuntimeLevel::SpawnHorses() {
  const TArray<FG::FMapHorseRouteSeed> HorseRoutes =
      FG::HorseRoutes::Build1899HorseRouteSeed();
  for (const FG::FMapHorseRouteSeed &HorseRoute : HorseRoutes) {
    SpawnHorse(HorseRoute.Name, BuildRoute(HorseRoute.PatrolRoute),
               HorseRoute.bMountedRider);
  }
}

AStaticMeshActor *ARuntimeLevel::SpawnBlock(
    const FG::FMapBlockSpawn &BlockSpawn) {
  UWorld *World = GetWorld();
  if (!World || !CubeMesh) {
    return nullptr;
  }

  FActorSpawnParameters Params;
  Params.Name = MakeUniqueObjectName(World, AStaticMeshActor::StaticClass(),
                                     FName(*BlockSpawn.Name));

  AStaticMeshActor *Block =
      World->SpawnActor<AStaticMeshActor>(BlockSpawn.Location,
                                          FRotator::ZeroRotator, Params);
  if (!Block) {
    return nullptr;
  }

  Block->SetActorScale3D(BlockSpawn.Scale);
  Block->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
  FG::RetroRendering::ApplyTexture(
      {Block->GetStaticMeshComponent(), BlockBaseMaterial, BlockSpawn.Texture});
  Block->GetStaticMeshComponent()->SetMobility(EComponentMobility::Static);
  return Block;
}

AStaticMeshActor *ARuntimeLevel::SpawnTerrainBlock(
    const FG::FMapTerrainBlockSpawn &BlockSpawn) {
  return SpawnBlock({BlockSpawn.Name, FGL::ToWorld(TerrainData, BlockSpawn.Point),
                     BlockSpawn.Scale, BlockSpawn.Texture});
}

void ARuntimeLevel::SpawnLabel(const FString &Text,
                                          const FVector &Location,
                                          float WorldSize) {
  UWorld *World = GetWorld();
  if (!World) {
    return;
  }

  ATextRenderActor *Label =
      World->SpawnActor<ATextRenderActor>(Location, FRotator::ZeroRotator);
  if (!Label) {
    return;
  }

  Label->GetTextRender()->SetText(FText::FromString(Text));
  Label->GetTextRender()->SetHorizontalAlignment(EHTA_Center);
  Label->GetTextRender()->SetWorldSize(WorldSize);
}

void ARuntimeLevel::SpawnTerrainLabel(
    const FString &Text, const FG::FMapLocalPoint &Point,
    float WorldSize) {
  SpawnLabel(Text, FGL::ToWorld(TerrainData, Point), WorldSize);
}

TArray<FVector> ARuntimeLevel::BuildRoute(
    const TArray<FG::FMapLocalPoint> &Route) const {
  TArray<FVector> WorldRoute;
  WorldRoute.Reserve(Route.Num());
  for (const FG::FMapLocalPoint &Point : Route) {
    WorldRoute.Add(FGL::ToWorld(TerrainData, Point));
  }
  return WorldRoute;
}

ATalkableTownsperson *ARuntimeLevel::SpawnTownsperson(
    const FString &Name, const FString &InRole, const FString &Persona,
    const TArray<FVector> &Route) {
  UWorld *World = GetWorld();
  if (!World || Route.Num() == 0) {
    return nullptr;
  }

  ATalkableTownsperson *Townsperson =
      World->SpawnActor<ATalkableTownsperson>(Route[0], FRotator::ZeroRotator);
  if (Townsperson) {
    Townsperson->ConfigureTownsperson(Name, InRole, Persona, Route);
  }
  return Townsperson;
}

AWalkingHorse *ARuntimeLevel::SpawnHorse(
    const FString &Name, const TArray<FVector> &Route, bool bMountedRider) {
  UWorld *World = GetWorld();
  if (!World || Route.Num() == 0) {
    return nullptr;
  }

  AWalkingHorse *Horse =
      World->SpawnActor<AWalkingHorse>(Route[0], FRotator::ZeroRotator);
  if (Horse) {
    Horse->ConfigureHorse(Name, Route, bMountedRider);
  }
  return Horse;
}
