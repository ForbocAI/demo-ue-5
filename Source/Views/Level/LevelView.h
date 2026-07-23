// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "Core/fp.hpp"
#include "CoreMinimal.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/Actor.h"
#include "LevelView.generated.h"
class AStaticMeshActor;
class ATerrainMeshView;
class ATownspersonView;
class AHorseView;
class UMaterialInterface;
class UStaticMesh;

namespace ForbocAI {
namespace Game {
namespace Level {
struct FBlockSpawn;
struct FLabelSpawn;
struct FSectionSpawn;
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
  /** User Story: As a views level consumer, I need to invoke aruntime level view through a stable signature so the views level workflow remains explicit and composable. @fn ARuntimeLevelView() */
  ARuntimeLevelView();

  /** User Story: As a views level consumer, I need to invoke begin play through a stable signature so the views level workflow remains explicit and composable. @fn virtual void BeginPlay() override */
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
  ForbocAI::Game::Data::FGeometrySettings LevelGeometrySettings;
  ForbocAI::Game::Data::FRenderingSettings RenderingSettings;

  /**
   * @fn void RenderLevel()
   * @brief Dispatches the RuntimeSlice level-payload thunk and applies the
   * resolved payload to Unreal actors.
   * User Story: As a views level consumer, I need to invoke render level through a stable signature so the views level workflow remains explicit and composable.
   */
  void RenderLevel();

  /**
   * @fn void RenderLevelPayload( const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload)
   * @brief Applies a reducer/thunk-owned payload without deriving gameplay
   * state in the view.
   * User Story: As a views level consumer, I need to invoke render level payload through a stable signature so the views level workflow remains explicit and composable.
   */
  void RenderLevelPayload(
      const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload);
  /** User Story: As a views level consumer, I need to invoke render terrain through a stable signature so the views level workflow remains explicit and composable. @fn void RenderTerrain( const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload) */
  void RenderTerrain(
      const ForbocAI::Game::Level::FRuntimeLevelViewPayload &Payload);
  /** User Story: As a views level consumer, I need to invoke render townspeople through a stable signature so the views level workflow remains explicit and composable. @fn void RenderTownspeople( const TArray< ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn> &Townspeople, int32 Index) */
  void RenderTownspeople(
      const TArray<
          ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn> &Townspeople,
      int32 Index);
  /** User Story: As a views level consumer, I need to invoke render horses through a stable signature so the views level workflow remains explicit and composable. @fn void RenderHorses( const TArray<ForbocAI::Game::Level::FRuntimeHorseViewSpawn> &Horses, int32 Index) */
  void RenderHorses(
      const TArray<ForbocAI::Game::Level::FRuntimeHorseViewSpawn> &Horses,
      int32 Index);
  /** User Story: As a views level consumer, I need to invoke render sections through a stable signature so the views level workflow remains explicit and composable. @fn void RenderSections( const TArray<ForbocAI::Game::Level::FSectionSpawn> &Sections, int32 Index) */
  void RenderSections(
      const TArray<ForbocAI::Game::Level::FSectionSpawn> &Sections,
      int32 Index);

  /** User Story: As a views level consumer, I need to invoke render section through a stable signature so the views level workflow remains explicit and composable. @fn void RenderSection( const ForbocAI::Game::Level::FSectionSpawn &Section) */
  void RenderSection(
      const ForbocAI::Game::Level::FSectionSpawn &Section);
  /** User Story: As a views level consumer, I need to invoke render blocks through a stable signature so the views level workflow remains explicit and composable. @fn void RenderBlocks( const TArray<ForbocAI::Game::Level::FBlockSpawn> &Blocks, int32 Index) */
  void RenderBlocks(
      const TArray<ForbocAI::Game::Level::FBlockSpawn> &Blocks,
      int32 Index);
  /** User Story: As a views level consumer, I need to invoke render labels through a stable signature so the views level workflow remains explicit and composable. @fn void RenderLabels( const TArray<ForbocAI::Game::Level::FLabelSpawn> &Labels, int32 Index) */
  void RenderLabels(
      const TArray<ForbocAI::Game::Level::FLabelSpawn> &Labels,
      int32 Index);
  /** User Story: As a views level consumer, I need to invoke render block through a stable signature so the views level workflow remains explicit and composable. @fn AStaticMeshActor *RenderBlock( const ForbocAI::Game::Level::FBlockSpawn &BlockSpawn) */
  AStaticMeshActor *RenderBlock(
      const ForbocAI::Game::Level::FBlockSpawn &BlockSpawn);
  /** User Story: As a views level consumer, I need to invoke render label through a stable signature so the views level workflow remains explicit and composable. @fn void RenderLabel(const ForbocAI::Game::Level::FLabelSpawn &LabelSpawn) */
  void RenderLabel(const ForbocAI::Game::Level::FLabelSpawn &LabelSpawn);
  /** User Story: As a views level consumer, I need to invoke render townsperson through a stable signature so the views level workflow remains explicit and composable. @fn ATownspersonView *RenderTownsperson( const ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn &Spawn) */
  ATownspersonView *RenderTownsperson(
      const ForbocAI::Game::Level::FRuntimeTownspersonViewSpawn &Spawn);
  /** User Story: As a views level consumer, I need to invoke render horse through a stable signature so the views level workflow remains explicit and composable. @fn AHorseView *RenderHorse( const ForbocAI::Game::Level::FRuntimeHorseViewSpawn &Spawn) */
  AHorseView *RenderHorse(
      const ForbocAI::Game::Level::FRuntimeHorseViewSpawn &Spawn);
};
