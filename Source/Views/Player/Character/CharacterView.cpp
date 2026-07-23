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
/** User Story: As a views player character consumer, I need to invoke observe player presentation through a stable signature so the views player character workflow remains explicit and composable. @fn FG::FPresentationViewModel ObservePlayerPresentation() */
FG::FPresentationViewModel ObservePlayerPresentation() {
  return FG::RuntimeSelectors::SelectPlayerPresentation();
}

/** User Story: As a views player character consumer, I need to invoke load input action through a stable signature so the views player character workflow remains explicit and composable. @fn UInputAction *LoadInputAction(const FString &AssetPath) */
UInputAction *LoadInputAction(const FString &AssetPath) {
  return LoadObject<UInputAction>(nullptr, *AssetPath);
}
} // namespace

/** User Story: As a views player character consumer, I need to invoke aplayer character view through a stable signature so the views player character workflow remains explicit and composable. @fn APlayerCharacterView::APlayerCharacterView() */
APlayerCharacterView::APlayerCharacterView()
    : MappingContext(nullptr), MouseMappingContext(nullptr), MoveAction(nullptr),
      LookAction(nullptr), MouseLookAction(nullptr), JumpAction(nullptr),
      MeshRelativeLocation(FVector::ZeroVector),
      MeshRelativeRotation(FRotator::ZeroRotator), CharacterForcedLodModel(0),
      CharacterMinLodModel(0), CharacterCullDistance(0.0f),
      bCharacterCastShadow(false), bCharacterComponentTickEnabled(false),
      bCharacterUpdateRateOptimizationsEnabled(false) {
  const FG::FPresentationViewModel Presentation =
      ObservePlayerPresentation();
  const ForbocAI::Game::Data::FViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectViewNames();
  MeshRelativeLocation = Presentation.MeshTransform.MeshRelativeLocation;
  MeshRelativeRotation = Presentation.MeshTransform.MeshRelativeRotation;
  CharacterForcedLodModel =
      Presentation.MeshLod.SkeletalMeshForcedLodModel;
  CharacterMinLodModel = Presentation.MeshLod.SkeletalMeshMinLodModel;
  CharacterCullDistance = Presentation.MeshLod.MeshCullDistance;
  bCharacterCastShadow = Presentation.MeshBehavior.bMeshCastShadow;
  bCharacterComponentTickEnabled =
      Presentation.MeshBehavior.bMeshComponentTickEnabled;
  bCharacterUpdateRateOptimizationsEnabled =
      Presentation.MeshBehavior.bMeshUpdateRateOptimizationsEnabled;
  CharacterMeshPath = Presentation.MeshAssets.MeshPath;
  CharacterAnimationBlueprintClassPath =
      Presentation.MeshAssets.AnimationBlueprintClassPath;
  MoveActionPath = Presentation.InputActions.MoveActionPath;
  LookActionPath = Presentation.InputActions.LookActionPath;
  MouseLookActionPath = Presentation.InputActions.MouseLookActionPath;
  JumpActionPath = Presentation.InputActions.JumpActionPath;
  DefaultMappingContextPath =
      Presentation.InputMappings.DefaultMappingContextPath;
  MouseMappingContextPath =
      Presentation.InputMappings.MouseMappingContextPath;
  GetCapsuleComponent()->InitCapsuleSize(
      Presentation.Capsule.CapsuleRadius,
      Presentation.Capsule.CapsuleHalfHeight);

  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  UCharacterMovementComponent *Movement = GetCharacterMovement();
  Movement->bOrientRotationToMovement = true;
  Movement->RotationRate =
      FRotator(0.0f, Presentation.Movement.RotationRateYaw, 0.0f);
  Movement->JumpZVelocity = Presentation.Movement.JumpZVelocity;
  Movement->AirControl = Presentation.Movement.AirControl;
  Movement->MaxWalkSpeed = Presentation.Movement.MaxWalkSpeed;
  Movement->MinAnalogWalkSpeed =
      Presentation.Movement.MinAnalogWalkSpeed;
  Movement->BrakingDecelerationWalking =
      Presentation.Braking.BrakingDecelerationWalking;
  Movement->BrakingDecelerationFalling =
      Presentation.Braking.BrakingDecelerationFalling;

  CameraBoom =
      CreateDefaultSubobject<USpringArmComponent>(
          FName(*ViewNames.Player.CameraBoom));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = Presentation.Camera.FollowCameraArmLength;
  CameraBoom->bUsePawnControlRotation = true;

  FollowCamera =
      CreateDefaultSubobject<UCameraComponent>(
          FName(*ViewNames.Player.FollowCamera));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
  FollowCamera->bUsePawnControlRotation = false;

  ConfigureTemplateCharacter();
  ConfigureEnhancedInput();
}

/** User Story: As a views player character consumer, I need to invoke begin play through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::BeginPlay() */
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

/** User Story: As a views player character consumer, I need to invoke setup player input component through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::SetupPlayerInputComponent( UInputComponent *PlayerInputComponent) */
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

/** User Story: As a views player character consumer, I need to invoke do move through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::DoMove(float Right, float Forward) */
void APlayerCharacterView::DoMove(float Right, float Forward) {
  check(Controller);
  const ForbocAI::Game::Data::FObservationIdSettings &Ids =
      FG::RuntimeSelectors::SelectObservationIds();
  const ForbocAI::Game::Level::FMovementInputViewModel Model =
      FG::RuntimeSelectors::SelectPlayerMovementInput(
          {Ids.PlayerMovementInputObserved, Controller->GetControlRotation(),
           Right, Forward, true});
  Model.bShouldMove
      ? (AddMovementInput(Model.ForwardDirection, Model.ForwardScale),
         AddMovementInput(Model.RightDirection, Model.RightScale), void())
      : void();
}

/** User Story: As a views player character consumer, I need to invoke do look through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::DoLook(float Yaw, float Pitch) */
void APlayerCharacterView::DoLook(float Yaw, float Pitch) {
  AddControllerYawInput(Yaw);
  AddControllerPitchInput(Pitch);
}

/** User Story: As a views player character consumer, I need to invoke do jump start through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::DoJumpStart() */
void APlayerCharacterView::DoJumpStart() { Jump(); }

/** User Story: As a views player character consumer, I need to invoke do jump end through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::DoJumpEnd() */
void APlayerCharacterView::DoJumpEnd() { StopJumping(); }

/** User Story: As a views player character consumer, I need to invoke get runtime follow camera through a stable signature so the views player character workflow remains explicit and composable. @fn UCameraComponent *APlayerCharacterView::GetRuntimeFollowCamera() const */
UCameraComponent *APlayerCharacterView::GetRuntimeFollowCamera() const {
  return FollowCamera;
}

/** User Story: As a views player character consumer, I need to invoke get runtime camera boom through a stable signature so the views player character workflow remains explicit and composable. @fn USpringArmComponent *APlayerCharacterView::GetRuntimeCameraBoom() const */
USpringArmComponent *APlayerCharacterView::GetRuntimeCameraBoom() const {
  return CameraBoom;
}

/** User Story: As a views player character consumer, I need to invoke configure template character through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::ConfigureTemplateCharacter() */
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

/** User Story: As a views player character consumer, I need to invoke configure enhanced input through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::ConfigureEnhancedInput() */
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

/** User Story: As a views player character consumer, I need to invoke move through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::Move(const FInputActionValue &Value) */
void APlayerCharacterView::Move(const FInputActionValue &Value) {
  const FVector2D MovementVector = Value.Get<FVector2D>();
  DoMove(MovementVector.X, MovementVector.Y);
}

/** User Story: As a views player character consumer, I need to invoke look through a stable signature so the views player character workflow remains explicit and composable. @fn void APlayerCharacterView::Look(const FInputActionValue &Value) */
void APlayerCharacterView::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  DoLook(LookAxisVector.X, LookAxisVector.Y);
}
