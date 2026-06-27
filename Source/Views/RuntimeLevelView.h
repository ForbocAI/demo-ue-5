#pragma once

#include "Core/functional_core.hpp"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeLevelView.generated.h"

class AStaticMeshActor;
class ATerrainMeshView;
class ATownspersonView;
class AHorseView;
class UMaterialInterface;
class UStaticMesh;

namespace ForbocAI {
namespace Demo {
namespace Level {
struct FLevelBlockSpawn;
struct FLevelLabelSpawn;
struct FLevelRuntimeSectionSpawn;
struct FRuntimeHorseViewSpawn;
struct FRuntimeLevelViewPayload;
struct FRuntimeTownspersonViewSpawn;
} // namespace Level
} // namespace Demo
} // namespace ForbocAI

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

  void RenderLevel();
  void RenderLevelPayload(
      const ForbocAI::Demo::Level::FRuntimeLevelViewPayload &Payload);
  void RenderTerrain();
  void RenderTerrain(
      const ForbocAI::Demo::Level::FRuntimeLevelViewPayload &Payload);
  void RenderTownspeople(
      const TArray<
          ForbocAI::Demo::Level::FRuntimeTownspersonViewSpawn> &Townspeople,
      int32 Index);
  void RenderHorses(
      const TArray<ForbocAI::Demo::Level::FRuntimeHorseViewSpawn> &Horses,
      int32 Index);
  void RenderSections(
      const TArray<ForbocAI::Demo::Level::FLevelRuntimeSectionSpawn> &Sections,
      int32 Index);

  void RenderSection(
      const ForbocAI::Demo::Level::FLevelRuntimeSectionSpawn &Section);
  void RenderBlocks(
      const TArray<ForbocAI::Demo::Level::FLevelBlockSpawn> &Blocks,
      int32 Index);
  void RenderLabels(
      const TArray<ForbocAI::Demo::Level::FLevelLabelSpawn> &Labels,
      int32 Index);
  AStaticMeshActor *RenderBlock(
      const ForbocAI::Demo::Level::FLevelBlockSpawn &BlockSpawn);
  void RenderLabel(const ForbocAI::Demo::Level::FLevelLabelSpawn &LabelSpawn);
  ATownspersonView *RenderTownsperson(
      const ForbocAI::Demo::Level::FRuntimeTownspersonViewSpawn &Spawn);
  AHorseView *RenderHorse(
      const ForbocAI::Demo::Level::FRuntimeHorseViewSpawn &Spawn);
};
