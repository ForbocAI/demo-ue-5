// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "Features/Systems/Selectors.h"
#include "GameFramework/Actor.h"
#include "View.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UTextRenderComponent;

/**
 * @brief View payload for configuring one townsperson actor from store data.
 *
 * Architecture: Runtime reducers/thunks prepare this display payload; the view
 * stores only the fields needed to render text, mesh, overlap, and patrol
 * presentation.
 */
USTRUCT(BlueprintType)
struct FTownspersonViewConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Id;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Role;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString InteractionPrompt;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString DefaultPlayerLine;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString PinnedResponse;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  TArray<FVector> PatrolRoute;

  ForbocAI::Game::Level::FLevelDistanceLodStage Lod;
};

/**
 * @brief Display actor for one reducer-owned townsperson spawn payload.
 *
 * Architecture: The actor exposes Blueprint-readable presentation facts and UE
 * overlap observations. Dialogue rules, nearest-target selection, and UI state
 * remain below Views in RTK reducers/thunks/selectors and ECS projections.
 *
 * User story: As a player, I can approach an NPC and see prompt/reply text
 * while the game logic remains in store-owned systems.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API ATownspersonView : public AActor {
  GENERATED_BODY()

public:
  ATownspersonView();

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  /**
   * @brief Applies a store-derived spawn/config payload to this actor.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ConfigureTownsperson(const FTownspersonViewConfig &Config);

  /**
   * @brief Applies a reducer/thunk-owned dialogue reply to presentation text.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ShowDialogueReply(const FString &Reply);

  /**
   * @brief Returns the current overlap observation for action payload building.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetTownspersonId() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetRole() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetPersona() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetDefaultPlayerLine() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  FString GetPinnedResponse() const;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonId;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString TownspersonRole;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Persona;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString InteractionPrompt;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString DefaultPlayerLine;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString PinnedResponse;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  TArray<FVector> PatrolRoute;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  float WalkSpeed;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  float PauseDuration;

private:
  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USkeletalMeshComponent *CharacterMesh;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  USphereComponent *InteractionSphere;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *NameText;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *PromptText;

  UPROPERTY(VisibleAnywhere, Category = "Level|NPC")
  UTextRenderComponent *DialogueText;

  int32 PatrolIndex;
  float PauseRemaining;
  float PatrolArrivalDistance;
  bool bPlayerNearby;
  FString CharacterMeshPath;
  FString CharacterAnimationBlueprintClassPath;
  ForbocAI::Game::Level::FLevelDistanceLodStage CurrentLod;

  void AdvancePatrol(float DeltaTime);
  void ApplyDistanceLod(
      const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod);
  void ConfigureSampleCharacterAsset();
  void RefreshText();

  UFUNCTION()
  void HandleInteractionBegin(UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor,
                              UPrimitiveComponent *OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult &SweepResult);

  UFUNCTION()
  void HandleInteractionEnd(UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor,
                            UPrimitiveComponent *OtherComp,
                            int32 OtherBodyIndex);
};
