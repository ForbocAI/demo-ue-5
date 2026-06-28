#include "Views/PlayerCharacterView.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Features/Entities/Characters/Player/PlayerActions.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Store.h"

namespace FG = ForbocAI::Demo::Level;

namespace {
FG::FPlayerPresentationViewModel ObservePlayerPresentation() {
  FG::Store::GetStore().dispatch(
      FG::PlayerActions::PlayerPresentationRequested()(
          {TEXT("entities/player/presentationRequested")}));
  return FG::RuntimeSelectors::SelectPlayerPresentation(
      FG::Store::GetStore().getState());
}

UInputAction *LoadInputAction(const FString &AssetPath) {
  return LoadObject<UInputAction>(nullptr, *AssetPath);
}
} // namespace

APlayerCharacterView::APlayerCharacterView()
    : MappingContext(nullptr), MouseMappingContext(nullptr), MoveAction(nullptr),
      LookAction(nullptr), MouseLookAction(nullptr), JumpAction(nullptr),
      MeshRelativeLocation(FVector::ZeroVector),
      MeshRelativeRotation(FRotator::ZeroRotator) {
  const FG::FPlayerPresentationViewModel Presentation =
      ObservePlayerPresentation();
  MeshRelativeLocation = Presentation.MeshRelativeLocation;
  MeshRelativeRotation = Presentation.MeshRelativeRotation;
  CharacterMeshPath = Presentation.MeshPath;
  CharacterAnimationBlueprintClassPath =
      Presentation.AnimationBlueprintClassPath;
  MoveActionPath = Presentation.MoveActionPath;
  LookActionPath = Presentation.LookActionPath;
  MouseLookActionPath = Presentation.MouseLookActionPath;
  JumpActionPath = Presentation.JumpActionPath;
  DefaultMappingContextPath = Presentation.DefaultMappingContextPath;
  MouseMappingContextPath = Presentation.MouseMappingContextPath;
  GetCapsuleComponent()->InitCapsuleSize(Presentation.CapsuleRadius,
                                         Presentation.CapsuleHalfHeight);

  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  UCharacterMovementComponent *Movement = GetCharacterMovement();
  Movement->bOrientRotationToMovement = true;
  Movement->RotationRate = FRotator(0.0f, Presentation.RotationRateYaw, 0.0f);
  Movement->JumpZVelocity = Presentation.JumpZVelocity;
  Movement->AirControl = Presentation.AirControl;
  Movement->MaxWalkSpeed = Presentation.MaxWalkSpeed;
  Movement->MinAnalogWalkSpeed = Presentation.MinAnalogWalkSpeed;
  Movement->BrakingDecelerationWalking =
      Presentation.BrakingDecelerationWalking;
  Movement->BrakingDecelerationFalling =
      Presentation.BrakingDecelerationFalling;

  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = Presentation.FollowCameraArmLength;
  CameraBoom->bUsePawnControlRotation = true;

  FollowCamera =
      CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
  FollowCamera->bUsePawnControlRotation = false;

  ConfigureTemplateCharacter();
  ConfigureEnhancedInput();
}

void APlayerCharacterView::BeginPlay() {
  Super::BeginPlay();

  if (APlayerController *PlayerController = Cast<APlayerController>(Controller)) {
    if (ULocalPlayer *LocalPlayer = PlayerController->GetLocalPlayer()) {
      if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
              ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                  LocalPlayer)) {
        if (MappingContext) {
          Subsystem->AddMappingContext(MappingContext, 0);
        }
        if (MouseMappingContext) {
          Subsystem->AddMappingContext(MouseMappingContext, 0);
        }
      }
    }
  }
}

void APlayerCharacterView::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  if (UEnhancedInputComponent *EnhancedInput =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    JumpAction ? (EnhancedInput->BindAction(
                      JumpAction, ETriggerEvent::Started, this,
                      &APlayerCharacterView::DoJumpStart),
                  EnhancedInput->BindAction(
                      JumpAction, ETriggerEvent::Completed, this,
                      &APlayerCharacterView::DoJumpEnd),
                  void())
               : void();
    MoveAction ? (EnhancedInput->BindAction(
                      MoveAction, ETriggerEvent::Triggered, this,
                      &APlayerCharacterView::Move),
                  void())
               : void();
    LookAction ? (EnhancedInput->BindAction(
                      LookAction, ETriggerEvent::Triggered, this,
                      &APlayerCharacterView::Look),
                  void())
               : void();
    MouseLookAction ? (EnhancedInput->BindAction(
                           MouseLookAction, ETriggerEvent::Triggered, this,
                           &APlayerCharacterView::Look),
                       void())
                    : void();
  }
}

void APlayerCharacterView::DoMove(float Right, float Forward) {
  const ForbocAI::Demo::Level::FPlayerMovementInputViewModel Model =
      (FG::Store::GetStore().dispatch(
           FG::PlayerActions::PlayerMovementInputObserved()(
               {TEXT("entities/player/movementInputObserved"),
                Controller ? Controller->GetControlRotation()
                           : FRotator::ZeroRotator,
                Right, Forward, Controller != nullptr})),
       FG::RuntimeSelectors::SelectPlayerMovementInput(
           FG::Store::GetStore().getState()));
  Model.bShouldMove
      ? (AddMovementInput(Model.ForwardDirection, Model.ForwardScale),
         AddMovementInput(Model.RightDirection, Model.RightScale), void())
      : void();
}

void APlayerCharacterView::DoLook(float Yaw, float Pitch) {
  AddControllerYawInput(Yaw);
  AddControllerPitchInput(Pitch);
}

void APlayerCharacterView::DoJumpStart() { Jump(); }

void APlayerCharacterView::DoJumpEnd() { StopJumping(); }

void APlayerCharacterView::ConfigureTemplateCharacter() {
  if (USkeletalMesh *CharacterMesh =
          LoadObject<USkeletalMesh>(nullptr, *CharacterMeshPath)) {
    GetMesh()->SetSkeletalMesh(CharacterMesh);
    GetMesh()->SetRelativeLocation(MeshRelativeLocation);
    GetMesh()->SetRelativeRotation(MeshRelativeRotation);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("ThirdPerson: required Manny skeletal mesh is missing."));
  }

  if (UClass *AnimClass = LoadClass<UAnimInstance>(
          nullptr, *CharacterAnimationBlueprintClassPath)) {
    GetMesh()->SetAnimInstanceClass(AnimClass);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("ThirdPerson: required ABP_Unarmed animation blueprint is "
                "missing."));
  }
}

void APlayerCharacterView::ConfigureEnhancedInput() {
  MoveAction = LoadInputAction(MoveActionPath);
  LookAction = LoadInputAction(LookActionPath);
  MouseLookAction = LoadInputAction(MouseLookActionPath);
  JumpAction = LoadInputAction(JumpActionPath);

  MappingContext = LoadObject<UInputMappingContext>(
      nullptr, *DefaultMappingContextPath);
  MouseMappingContext = LoadObject<UInputMappingContext>(
      nullptr, *MouseMappingContextPath);
  if (!MoveAction || !LookAction || !MouseLookAction || !JumpAction ||
      !MappingContext || !MouseMappingContext) {
    UE_LOG(LogTemp, Error,
           TEXT("ThirdPerson: input assets missing from JSON-authored paths."));
  }
}

void APlayerCharacterView::Move(const FInputActionValue &Value) {
  const FVector2D MovementVector = Value.Get<FVector2D>();
  DoMove(MovementVector.X, MovementVector.Y);
}

void APlayerCharacterView::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  DoLook(LookAxisVector.X, LookAxisVector.Y);
}
