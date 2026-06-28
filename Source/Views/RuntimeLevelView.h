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

/**
 * @brief Runtime level presentation actor for store-derived ECS/RTK payloads.
 *
 * Architecture: The actor requests FRuntimeLevelViewPayload through the
 * RuntimeSlice thunk and applies that payload to Unreal actors. Layout, spawn
 * data, terrain fallback choices, and ECS projection are prepared below the
 * view layer by reducers, thunks, selectors, and neutral ECS primitives.
 *
 * User story: As a game developer, I need one source-controlled view actor that
 * materializes the current store state into the playable French Gulch scene.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API ARuntimeLevelView : public AActor {
  GENERATED_BODY()

public:
  ARuntimeLevelView();

  virtual void BeginPlay() override;

  /**
   * @brief Controls whether BeginPlay asks the runtime store for a level view
   * payload and renders it.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|View")
  bool bSpawnOnBeginPlay;

private:
  UStaticMesh *CubeMesh;
  UMaterialInterface *BlockBaseMaterial;

  /**
   * @brief Dispatches the RuntimeSlice level-payload thunk and applies the
   * resolved payload to Unreal actors.
   */
  void RenderLevel();

  /**
   * @brief Applies a reducer/thunk-owned payload without deriving gameplay
   * state in the view.
   */
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
