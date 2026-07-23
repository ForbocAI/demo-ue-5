// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterView.generated.h"
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

/**
 * @brief Player pawn display/input boundary for store-derived presentation.
 *
 * Architecture: The pawn applies selector output for mesh, camera, and
 * movement presentation, then dispatches input observations through action
 * creators. Movement-vector decisions are selected from reducer-owned state;
 * direct Unreal calls only apply the selected result to the Character API.
 *
 * User story: As a player, movement and camera input should feel immediate
 * while the reusable movement semantics stay in RTK reducers/selectors.
 */
UCLASS()
class FORBOCAIDEMO_API APlayerCharacterView : public ACharacter {
  GENERATED_BODY()

public:
  /** User Story: As a views player character consumer, I need to invoke aplayer character view through a stable signature so the views player character workflow remains explicit and composable. @fn APlayerCharacterView() */
  APlayerCharacterView();

  /** User Story: As a views player character consumer, I need to invoke begin play through a stable signature so the views player character workflow remains explicit and composable. @fn virtual void BeginPlay() override */
  virtual void BeginPlay() override;
  /** User Story: As a views player character consumer, I need to invoke setup player input component through a stable signature so the views player character workflow remains explicit and composable. @fn virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent) override */
  virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
      override;

  /**
   * @fn void DoMove(float Right, float Forward)
   * @brief Dispatches observed movement input and applies selector output to
   * Unreal movement.
   * User Story: As a views player character consumer, I need to invoke do move through a stable signature so the views player character workflow remains explicit and composable.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoMove(float Right, float Forward);

  /**
   * @fn void DoLook(float Yaw, float Pitch)
   * @brief Applies raw look input to the Unreal controller.
   * User Story: As a views player character consumer, I need to invoke do look through a stable signature so the views player character workflow remains explicit and composable.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoLook(float Yaw, float Pitch);

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  /** User Story: As a views player character consumer, I need to invoke do jump start through a stable signature so the views player character workflow remains explicit and composable. @fn void DoJumpStart() */
  void DoJumpStart();

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  /** User Story: As a views player character consumer, I need to invoke do jump end through a stable signature so the views player character workflow remains explicit and composable. @fn void DoJumpEnd() */
  void DoJumpEnd();

  /** User Story: As a views player character consumer, I need to invoke get runtime follow camera through a stable signature so the views player character workflow remains explicit and composable. @fn UCameraComponent *GetRuntimeFollowCamera() const */
  UCameraComponent *GetRuntimeFollowCamera() const;
  /** User Story: As a views player character consumer, I need to invoke get runtime camera boom through a stable signature so the views player character workflow remains explicit and composable. @fn USpringArmComponent *GetRuntimeCameraBoom() const */
  USpringArmComponent *GetRuntimeCameraBoom() const;

private:
  UPROPERTY(VisibleAnywhere, Category = "Level|Camera")
  USpringArmComponent *CameraBoom;

  UPROPERTY(VisibleAnywhere, Category = "Level|Camera")
  UCameraComponent *FollowCamera;

  UPROPERTY()
  UInputMappingContext *MappingContext;

  UPROPERTY()
  UInputMappingContext *MouseMappingContext;

  UPROPERTY()
  UInputAction *MoveAction;

  UPROPERTY()
  UInputAction *LookAction;

  UPROPERTY()
  UInputAction *MouseLookAction;

  UPROPERTY()
  UInputAction *JumpAction;

  FVector MeshRelativeLocation;
  FRotator MeshRelativeRotation;
  int32 CharacterForcedLodModel;
  int32 CharacterMinLodModel;
  float CharacterCullDistance;
  bool bCharacterCastShadow;
  bool bCharacterComponentTickEnabled;
  bool bCharacterUpdateRateOptimizationsEnabled;
  FString CharacterMeshPath;
  FString CharacterAnimationBlueprintClassPath;
  FString MoveActionPath;
  FString LookActionPath;
  FString MouseLookActionPath;
  FString JumpActionPath;
  FString DefaultMappingContextPath;
  FString MouseMappingContextPath;

  /** User Story: As a views player character consumer, I need to invoke configure template character through a stable signature so the views player character workflow remains explicit and composable. @fn void ConfigureTemplateCharacter() */
  void ConfigureTemplateCharacter();
  /** User Story: As a views player character consumer, I need to invoke configure enhanced input through a stable signature so the views player character workflow remains explicit and composable. @fn void ConfigureEnhancedInput() */
  void ConfigureEnhancedInput();
  /** User Story: As a views player character consumer, I need to invoke move through a stable signature so the views player character workflow remains explicit and composable. @fn void Move(const FInputActionValue &Value) */
  void Move(const FInputActionValue &Value);
  /** User Story: As a views player character consumer, I need to invoke look through a stable signature so the views player character workflow remains explicit and composable. @fn void Look(const FInputActionValue &Value) */
  void Look(const FInputActionValue &Value);
};
