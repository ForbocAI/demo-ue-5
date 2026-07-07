// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterView.generated.h"

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
  APlayerCharacterView();

  virtual void BeginPlay() override;
  virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
      override;

  /**
   * @brief Dispatches observed movement input and applies selector output to
   * Unreal movement.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoMove(float Right, float Forward);

  /**
   * @brief Applies raw look input to the Unreal controller.
   */
  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoLook(float Yaw, float Pitch);

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoJumpStart();

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoJumpEnd();

  UCameraComponent *GetRuntimeFollowCamera() const;
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

  void ConfigureTemplateCharacter();
  void ConfigureEnhancedInput();
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
};
