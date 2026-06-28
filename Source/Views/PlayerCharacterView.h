#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacterView.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class DEMOPROJECT_API APlayerCharacterView : public ACharacter {
  GENERATED_BODY()

public:
  APlayerCharacterView();

  virtual void BeginPlay() override;
  virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
      override;

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoMove(float Right, float Forward);

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoLook(float Yaw, float Pitch);

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoJumpStart();

  UFUNCTION(BlueprintCallable, Category = "Level|Input")
  void DoJumpEnd();

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

  void ConfigureTemplateCharacter();
  void ConfigureEnhancedInput();
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
};
