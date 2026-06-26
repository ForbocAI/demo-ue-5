#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
struct FInputActionValue;

UCLASS()
class DEMOPROJECT_API AThirdPersonCharacter : public ACharacter {
  GENERATED_BODY()

public:
  AThirdPersonCharacter();

  virtual void BeginPlay() override;
  virtual void SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
      override;

  UFUNCTION(BlueprintCallable, Category = "Map|Input")
  void DoMove(float Right, float Forward);

  UFUNCTION(BlueprintCallable, Category = "Map|Input")
  void DoLook(float Yaw, float Pitch);

  UFUNCTION(BlueprintCallable, Category = "Map|Input")
  void DoJumpStart();

  UFUNCTION(BlueprintCallable, Category = "Map|Input")
  void DoJumpEnd();

private:
  UPROPERTY(VisibleAnywhere, Category = "Map|Camera")
  USpringArmComponent *CameraBoom;

  UPROPERTY(VisibleAnywhere, Category = "Map|Camera")
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

  void ConfigureTemplateCharacter();
  void ConfigureEnhancedInput();
  void Move(const FInputActionValue &Value);
  void Look(const FInputActionValue &Value);
};
