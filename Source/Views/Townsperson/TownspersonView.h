// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/Actor.h"
#include "TownspersonView.generated.h"
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
struct FViewIdentity {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Id;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Role;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString Persona;
};

USTRUCT(BlueprintType)
struct FViewConversation {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString InteractionPrompt;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString DefaultPlayerLine;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FString PinnedResponse;
};

USTRUCT(BlueprintType)
struct FTownspersonViewConfig {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FViewIdentity Identity;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level|NPC")
  FViewConversation Conversation;

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
  /** User Story: As a views townsperson consumer, I need to invoke atownsperson view through a stable signature so the views townsperson workflow remains explicit and composable. @fn ATownspersonView() */
  ATownspersonView();

  /** User Story: As a views townsperson consumer, I need to invoke begin play through a stable signature so the views townsperson workflow remains explicit and composable. @fn virtual void BeginPlay() override */
  virtual void BeginPlay() override;
  /** User Story: As a views townsperson consumer, I need to invoke tick through a stable signature so the views townsperson workflow remains explicit and composable. @fn virtual void Tick(float DeltaTime) override */
  virtual void Tick(float DeltaTime) override;

  /**
   * @fn void ConfigureTownsperson(const FTownspersonViewConfig &Config)
   * @brief Applies a store-derived spawn/config payload to this actor.
   * User Story: As a views townsperson consumer, I need to invoke configure townsperson through a stable signature so the views townsperson workflow remains explicit and composable.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ConfigureTownsperson(const FTownspersonViewConfig &Config);

  /**
   * @fn void ShowDialogueReply(const FString &Reply)
   * @brief Applies a reducer/thunk-owned dialogue reply to presentation text.
   * User Story: As a views townsperson consumer, I need to invoke show dialogue reply through a stable signature so the views townsperson workflow remains explicit and composable.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|NPC")
  void ShowDialogueReply(const FString &Reply);

  /**
   * @fn bool IsPlayerNearby() const
   * @brief Returns the current overlap observation for action payload building.
   * User Story: As a views townsperson consumer, I need to invoke is player nearby through a stable signature so the views townsperson workflow remains explicit and composable.
   */
  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  bool IsPlayerNearby() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get townsperson id through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetTownspersonId() const */
  FString GetTownspersonId() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get townsperson name through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetTownspersonName() const */
  FString GetTownspersonName() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get role through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetRole() const */
  FString GetRole() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get persona through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetPersona() const */
  FString GetPersona() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get default player line through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetDefaultPlayerLine() const */
  FString GetDefaultPlayerLine() const;

  UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level|NPC")
  /** User Story: As a views townsperson consumer, I need to invoke get pinned response through a stable signature so the views townsperson workflow remains explicit and composable. @fn FString GetPinnedResponse() const */
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

  /** User Story: As a views townsperson consumer, I need to invoke advance patrol through a stable signature so the views townsperson workflow remains explicit and composable. @fn void AdvancePatrol(float DeltaTime) */
  void AdvancePatrol(float DeltaTime);
  /** User Story: As a views townsperson consumer, I need to invoke apply distance lod through a stable signature so the views townsperson workflow remains explicit and composable. @fn void ApplyDistanceLod( const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod) */
  void ApplyDistanceLod(
      const ForbocAI::Game::Level::FLevelDistanceLodStage &Lod);
  /** User Story: As a views townsperson consumer, I need to invoke configure sample character asset through a stable signature so the views townsperson workflow remains explicit and composable. @fn void ConfigureSampleCharacterAsset() */
  void ConfigureSampleCharacterAsset();
  /** User Story: As a views townsperson consumer, I need to invoke refresh text through a stable signature so the views townsperson workflow remains explicit and composable. @fn void RefreshText() */
  void RefreshText();

  UFUNCTION()
  /** User Story: As a views townsperson consumer, I need to invoke handle interaction begin through a stable signature so the views townsperson workflow remains explicit and composable. @fn void HandleInteractionBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) */
  void HandleInteractionBegin(UPrimitiveComponent *OverlappedComponent,
                              AActor *OtherActor,
                              UPrimitiveComponent *OtherComp,
                              int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult &SweepResult);

  UFUNCTION()
  /** User Story: As a views townsperson consumer, I need to invoke handle interaction end through a stable signature so the views townsperson workflow remains explicit and composable. @fn void HandleInteractionEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) */
  void HandleInteractionEnd(UPrimitiveComponent *OverlappedComponent,
                            AActor *OtherActor,
                            UPrimitiveComponent *OtherComp,
                            int32 OtherBodyIndex);
};
