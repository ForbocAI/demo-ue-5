// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "Core/ue_fp.hpp"
#include "CoreMinimal.h"
#include "Features/Components/Data/DataTypes.h"
#include "Features/Components/Data/RuntimeSettings/LevelSettingsTypes.h"
#include "Features/Components/Rendering/RenderingTypes.h"
#include "GameFramework/Actor.h"
#include "RuntimeLevelView.generated.h"

class AStaticMeshActor;
class ATerrainMeshView;
class ATownspersonView;
class AHorseView;
class UMaterialInterface;
class UStaticMesh;

namespace ForbocAI {
namespace Game {
namespace Level {
struct FLevelBlockSpawn;
struct FLevelLabelSpawn;
struct FLevelRuntimeSectionSpawn;
struct FRuntimeHorseViewSpawn;
struct FRuntimeLevelViewPayload;
struct FRuntimeTownspersonViewSpawn;
} // namespace Level
} // namespace Game
} // namespace ForbocAI

/**
 * @brief Runtime level presentation actor for store-derived ECS/RTK payloads.
 *
 * Architecture: The actor requests FRuntimeLevelViewPayload through the
 * RuntimeSlice thunk and applies that payload to Unreal actors. Layout, spawn
 * data and ECS projection are prepared below the view layer by reducers,
 * thunks, selectors, and neutral ECS primitives.
 *
 * User story: As a game developer, I need one source-controlled view actor that
 * materializes the current store state into the playable French Gulch scene.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API ARuntimeLevelView : public AActor {
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
  TArray<ForbocAI::Game::Level::FLevelRetroTextureSpec> TextureCatalog;
  ForbocAI::Game::Data::FLevelGeometrySettings LevelGeometrySettings;
  ForbocAI::Game::Data::FRenderingRuntimeSettings RenderingRuntimeSettings;

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
      const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload);
  void RenderTerrain(
      const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload);
  void RenderTownspeople(
      const TArray<
          ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn> &Townspeople,
      int32 Index);
  void RenderHorses(
      const TArray<ForbocAI::Game::Level::FRuntimeHorseViewSpawn> &Horses,
      int32 Index);
  void RenderSections(
      const TArray<ForbocAI::Game::Level::FLevelRuntimeSectionSpawn> &Sections,
      int32 Index);

  void RenderSection(
      const ForbocAI::Game::Level::FLevelRuntimeSectionSpawn &Section);
  void RenderBlocks(
      const TArray<ForbocAI::Game::Level::FLevelBlockSpawn> &Blocks,
      int32 Index);
  void RenderLabels(
      const TArray<ForbocAI::Game::Level::FLevelLabelSpawn> &Labels,
      int32 Index);
  AStaticMeshActor *RenderBlock(
      const ForbocAI::Game::Level::FLevelBlockSpawn &BlockSpawn);
  void RenderLabel(const ForbocAI::Game::Level::FLevelLabelSpawn &LabelSpawn);
  ATownspersonView *RenderTownsperson(
      const ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn &Spawn);
  AHorseView *RenderHorse(
      const ForbocAI::Game::Level::FRuntimeHorseViewSpawn &Spawn);
};
