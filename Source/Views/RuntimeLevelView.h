#pragma once

#include "Core/functional_core.hpp"
#include "CoreMinimal.h"
#include "Features/Components/Level/LevelTypes.h"
#include "Features/Systems/Level/LevelTypes.h"
#include "Features/Systems/Runtime/RuntimeAdapters.h"
#include "GameFramework/Actor.h"
#include "RuntimeLevelView.generated.h"

class AStaticMeshActor;
class ATerrainMeshView;
class ATalkableTownsperson;
class AWalkingHorse;
class UMaterialInterface;
class UStaticMesh;

UCLASS()
class DEMOPROJECT_API ARuntimeLevelView : public AActor {
  GENERATED_BODY()

public:
  ARuntimeLevelView();

  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|View")
  bool bSpawnOnBeginPlay;

private:
  UStaticMesh *CubeMesh;
  UMaterialInterface *BlockBaseMaterial;
  FLevelTerrainData TerrainData;
  FLevelOrthoData OrthoData;
  ForbocAI::Demo::Level::FLevelRuntimeLayoutSeed RuntimeLayout;
  ForbocAI::Demo::Level::RuntimeAdapters::FRuntimeLevelSession RuntimeSession;

  void RenderLevel();
  void RenderTerrain();
  void RenderNaturalEnvironment();
  void RenderTown();
  void RenderMineApproach();
  void RenderTownspeople();
  void RenderHorses();

  void RenderSection(
      const ForbocAI::Demo::Level::FLevelRuntimeSectionSpawn &Section);
  void RenderBlocks(
      const TArray<ForbocAI::Demo::Level::FLevelBlockSpawn> &Blocks,
      int32 Index);
  void RenderLabels(
      const TArray<ForbocAI::Demo::Level::FLevelLabelSpawn> &Labels,
      int32 Index);
  void RenderTownspersonSeeds(
      const TArray<ForbocAI::Demo::Level::FTownspersonSeed> &Townspeople,
      int32 Index);
  void RenderHorseSeeds(
      const TArray<ForbocAI::Demo::Level::FHorseRouteSeed> &Horses,
      int32 Index);

  AStaticMeshActor *RenderBlock(
      const ForbocAI::Demo::Level::FLevelBlockSpawn &BlockSpawn);
  void RenderLabel(const ForbocAI::Demo::Level::FLevelLabelSpawn &LabelSpawn);
  ATalkableTownsperson *RenderTownsperson(
      const ForbocAI::Demo::Level::FTownspersonSeed &Seed);
  AWalkingHorse *RenderHorse(
      const ForbocAI::Demo::Level::FHorseRouteSeed &Seed);
};
