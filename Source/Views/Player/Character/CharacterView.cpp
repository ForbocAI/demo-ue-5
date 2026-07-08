// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Player/Character/CharacterView.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Views/Skeletal/Lod/Clamp/ClampView.h"

namespace FG = ForbocAI::Game::Level;

namespace {
FG::FPlayerPresentationViewModel ObservePlayerPresentation() {
  return FG::RuntimeSelectors::SelectPlayerPresentation(
      // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
      FG::RuntimeSelectors::SelectState());
}

UInputAction *LoadInputAction(const FString &AssetPath) {
  return LoadObject<UInputAction>(nullptr, *AssetPath);
}
} // namespace

APlayerCharacterView::APlayerCharacterView()
    : MappingContext(nullptr), MouseMappingContext(nullptr), MoveAction(nullptr),
      LookAction(nullptr), MouseLookAction(nullptr), JumpAction(nullptr),
      MeshRelativeLocation(FVector::ZeroVector),
      MeshRelativeRotation(FRotator::ZeroRotator), CharacterForcedLodModel(0),
      CharacterMinLodModel(0), CharacterCullDistance(0.0f),
      bCharacterCastShadow(false), bCharacterComponentTickEnabled(false),
      bCharacterUpdateRateOptimizationsEnabled(false) {
  const FG::FPlayerPresentationViewModel Presentation =
      ObservePlayerPresentation();
  const ForbocAI::Game::Data::FViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectViewNames(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState());
  MeshRelativeLocation = Presentation.MeshRelativeLocation;
  MeshRelativeRotation = Presentation.MeshRelativeRotation;
  CharacterForcedLodModel = Presentation.SkeletalMeshForcedLodModel;
  CharacterMinLodModel = Presentation.SkeletalMeshMinLodModel;
  CharacterCullDistance = Presentation.MeshCullDistance;
  bCharacterCastShadow = Presentation.bMeshCastShadow;
  bCharacterComponentTickEnabled = Presentation.bMeshComponentTickEnabled;
  bCharacterUpdateRateOptimizationsEnabled =
      Presentation.bMeshUpdateRateOptimizationsEnabled;
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

  CameraBoom =
      CreateDefaultSubobject<USpringArmComponent>(
          FName(*ViewNames.PlayerCameraBoom));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = Presentation.FollowCameraArmLength;
  CameraBoom->bUsePawnControlRotation = true;

  FollowCamera =
      CreateDefaultSubobject<UCameraComponent>(
          FName(*ViewNames.PlayerFollowCamera));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
  FollowCamera->bUsePawnControlRotation = false;

  ConfigureTemplateCharacter();
  ConfigureEnhancedInput();
}

void APlayerCharacterView::BeginPlay() {
  Super::BeginPlay();

  APlayerController *PlayerController = Cast<APlayerController>(Controller);
  check(PlayerController);
  ULocalPlayer *LocalPlayer = PlayerController->GetLocalPlayer();
  check(LocalPlayer);
  UEnhancedInputLocalPlayerSubsystem *Subsystem =
      ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
          LocalPlayer);
  check(Subsystem);
  check(MappingContext);
  check(MouseMappingContext);
  Subsystem->AddMappingContext(MappingContext, 0);
  Subsystem->AddMappingContext(MouseMappingContext, 0);
}

void APlayerCharacterView::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  UEnhancedInputComponent *EnhancedInput =
      Cast<UEnhancedInputComponent>(PlayerInputComponent);
  check(EnhancedInput);
  check(JumpAction);
  check(MoveAction);
  check(LookAction);
  check(MouseLookAction);
  EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this,
                            &APlayerCharacterView::DoJumpStart);
  EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this,
                            &APlayerCharacterView::DoJumpEnd);
  EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                            &APlayerCharacterView::Move);
  EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this,
                            &APlayerCharacterView::Look);
  EnhancedInput->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
                            &APlayerCharacterView::Look);
}

void APlayerCharacterView::DoMove(float Right, float Forward) {
  check(Controller);
  const ForbocAI::Game::Data::FObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectObservationIds(
          // boundary-allow: RTK-VIEW-007 tick reads multiple domain selectors from one snapshot
          FG::RuntimeSelectors::SelectState());
  const ForbocAI::Game::Level::FPlayerMovementInputViewModel Model =
      FG::RuntimeSelectors::SelectPlayerMovementInput(
          {Ids.PlayerMovementInputObserved, Controller->GetControlRotation(),
           Right, Forward, true});
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

UCameraComponent *APlayerCharacterView::GetRuntimeFollowCamera() const {
  return FollowCamera;
}

USpringArmComponent *APlayerCharacterView::GetRuntimeCameraBoom() const {
  return CameraBoom;
}

void APlayerCharacterView::ConfigureTemplateCharacter() {
  USkeletalMesh *CharacterMesh =
      LoadObject<USkeletalMesh>(nullptr, *CharacterMeshPath);
  UClass *AnimClass =
      LoadClass<UAnimInstance>(nullptr, *CharacterAnimationBlueprintClassPath);
  check(CharacterMesh);
  check(AnimClass);
  GetMesh()->SetSkeletalMesh(CharacterMesh);
  GetMesh()->SetRelativeLocation(MeshRelativeLocation);
  GetMesh()->SetRelativeRotation(MeshRelativeRotation);
  ForbocAI::Game::Views::SkeletalLodClamp::Apply(
      GetMesh(), CharacterForcedLodModel, CharacterMinLodModel);
  GetMesh()->SetCullDistance(CharacterCullDistance);
  GetMesh()->SetCastShadow(bCharacterCastShadow);
  GetMesh()->SetComponentTickEnabled(bCharacterComponentTickEnabled);
  GetMesh()->bEnableUpdateRateOptimizations =
      bCharacterUpdateRateOptimizationsEnabled;
  GetMesh()->SetAnimInstanceClass(AnimClass);
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
  check(MoveAction);
  check(LookAction);
  check(MouseLookAction);
  check(JumpAction);
  check(MappingContext);
  check(MouseMappingContext);
}

void APlayerCharacterView::Move(const FInputActionValue &Value) {
  const FVector2D MovementVector = Value.Get<FVector2D>();
  DoMove(MovementVector.X, MovementVector.Y);
}

void APlayerCharacterView::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  DoLook(LookAxisVector.X, LookAxisVector.Y);
}
