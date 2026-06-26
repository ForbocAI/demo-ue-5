#include "Features/Player/ThirdPersonCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputTriggers.h"
#include "InputCoreTypes.h"

namespace {
constexpr float CapsuleRadius = 42.0f;
constexpr float CapsuleHalfHeight = 96.0f;
constexpr float FollowCameraArmLength = 400.0f;
constexpr float RotationRateYaw = 500.0f;
constexpr float JumpZVelocity = 500.0f;
constexpr float AirControl = 0.35f;
constexpr float MaxWalkSpeed = 500.0f;
constexpr float MinAnalogWalkSpeed = 20.0f;
constexpr float BrakingDecelerationWalking = 2000.0f;
constexpr float BrakingDecelerationFalling = 1500.0f;

UInputAction *InputAction(const TCHAR *AssetPath, UObject *Outer,
                          const FName Name, EInputActionValueType Type) {
  if (UInputAction *Loaded = LoadObject<UInputAction>(nullptr, AssetPath)) {
    return Loaded;
  }
  UInputAction *Created = NewObject<UInputAction>(Outer, Name);
  Created->ValueType = Type;
  return Created;
}

UInputModifierNegate *NegateY(UObject *Outer) {
  UInputModifierNegate *Modifier = NewObject<UInputModifierNegate>(Outer);
  Modifier->bX = false;
  Modifier->bY = true;
  Modifier->bZ = false;
  return Modifier;
}

UInputModifierNegate *NegateX(UObject *Outer) {
  UInputModifierNegate *Modifier = NewObject<UInputModifierNegate>(Outer);
  Modifier->bX = true;
  Modifier->bY = false;
  Modifier->bZ = false;
  return Modifier;
}

UInputModifierSwizzleAxis *SwizzleYXZ(UObject *Outer) {
  UInputModifierSwizzleAxis *Modifier =
      NewObject<UInputModifierSwizzleAxis>(Outer);
  Modifier->Order = EInputAxisSwizzle::YXZ;
  return Modifier;
}

struct FInputKeyMap {
  UInputMappingContext *Context;
  const UInputAction *Action;
  FKey Key;
};

struct FInputKeyMapWithModifiers {
  FInputKeyMap Map;
  TArray<UInputModifier *> Modifiers;
};

void MapKey(const FInputKeyMap &Map) {
  Map.Context->MapKey(Map.Action, Map.Key);
}

void MapKeyWithModifiers(const FInputKeyMapWithModifiers &Map) {
  FEnhancedActionKeyMapping &Mapping =
      Map.Map.Context->MapKey(Map.Map.Action, Map.Map.Key);
  for (UInputModifier *Modifier : Map.Modifiers) {
    Mapping.Modifiers.Add(Modifier);
  }
}
} // namespace

AThirdPersonCharacter::AThirdPersonCharacter()
    : MappingContext(nullptr), MoveAction(nullptr), LookAction(nullptr),
      JumpAction(nullptr) {
  GetCapsuleComponent()->InitCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;

  UCharacterMovementComponent *Movement = GetCharacterMovement();
  Movement->bOrientRotationToMovement = true;
  Movement->RotationRate = FRotator(0.0f, RotationRateYaw, 0.0f);
  Movement->JumpZVelocity = JumpZVelocity;
  Movement->AirControl = AirControl;
  Movement->MaxWalkSpeed = MaxWalkSpeed;
  Movement->MinAnalogWalkSpeed = MinAnalogWalkSpeed;
  Movement->BrakingDecelerationWalking = BrakingDecelerationWalking;
  Movement->BrakingDecelerationFalling = BrakingDecelerationFalling;

  CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
  CameraBoom->SetupAttachment(RootComponent);
  CameraBoom->TargetArmLength = FollowCameraArmLength;
  CameraBoom->bUsePawnControlRotation = true;

  FollowCamera =
      CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
  FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
  FollowCamera->bUsePawnControlRotation = false;

  ConfigureTemplateCharacter();
  ConfigureEnhancedInput();
}

void AThirdPersonCharacter::BeginPlay() {
  Super::BeginPlay();

  if (APlayerController *PlayerController = Cast<APlayerController>(Controller)) {
    if (ULocalPlayer *LocalPlayer = PlayerController->GetLocalPlayer()) {
      if (UEnhancedInputLocalPlayerSubsystem *Subsystem =
              ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
                  LocalPlayer)) {
        Subsystem->AddMappingContext(MappingContext, 0);
      }
    }
  }
}

void AThirdPersonCharacter::SetupPlayerInputComponent(
    UInputComponent *PlayerInputComponent) {
  if (UEnhancedInputComponent *EnhancedInput =
          Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this,
                              &AThirdPersonCharacter::DoJumpStart);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this,
                              &AThirdPersonCharacter::DoJumpEnd);
    EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this,
                              &AThirdPersonCharacter::Move);
    EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this,
                              &AThirdPersonCharacter::Look);
  }
}

void AThirdPersonCharacter::DoMove(float Right, float Forward) {
  if (!Controller) {
    return;
  }

  const FRotator Rotation = Controller->GetControlRotation();
  const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
  const FVector ForwardDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
  const FVector RightDirection =
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

  AddMovementInput(ForwardDirection, Forward);
  AddMovementInput(RightDirection, Right);
}

void AThirdPersonCharacter::DoLook(float Yaw, float Pitch) {
  AddControllerYawInput(Yaw);
  AddControllerPitchInput(Pitch);
}

void AThirdPersonCharacter::DoJumpStart() { Jump(); }

void AThirdPersonCharacter::DoJumpEnd() { StopJumping(); }

void AThirdPersonCharacter::ConfigureTemplateCharacter() {
  if (USkeletalMesh *CharacterMesh = LoadObject<USkeletalMesh>(
          nullptr, TEXT("/Game/Character/Mesh/SK_Mannequin.SK_Mannequin"))) {
    GetMesh()->SetSkeletalMesh(CharacterMesh);
    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleHalfHeight));
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
  }

  if (UClass *AnimClass = LoadClass<UAnimInstance>(
          nullptr,
          TEXT("/Game/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"))) {
    GetMesh()->SetAnimInstanceClass(AnimClass);
  }
}

void AThirdPersonCharacter::ConfigureEnhancedInput() {
  MoveAction = InputAction(TEXT("/Game/Input/Actions/IA_Move.IA_Move"), this,
                           TEXT("IA_Move"), EInputActionValueType::Axis2D);
  LookAction = InputAction(TEXT("/Game/Input/Actions/IA_Look.IA_Look"), this,
                           TEXT("IA_Look"), EInputActionValueType::Axis2D);
  JumpAction = InputAction(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"), this,
                           TEXT("IA_Jump"), EInputActionValueType::Boolean);

  MappingContext = LoadObject<UInputMappingContext>(
      nullptr, TEXT("/Game/Input/IMC_MapTP.IMC_MapTP"));
  if (MappingContext) {
    return;
  }

  MappingContext = NewObject<UInputMappingContext>(this, TEXT("IMC_RuntimeTP"));
  MapKeyWithModifiers(
      {{MappingContext, MoveAction, EKeys::W}, {SwizzleYXZ(MappingContext)}});
  MapKeyWithModifiers({{MappingContext, MoveAction, EKeys::S},
                       {SwizzleYXZ(MappingContext), NegateY(MappingContext)}});
  MapKey({MappingContext, MoveAction, EKeys::D});
  MapKeyWithModifiers(
      {{MappingContext, MoveAction, EKeys::A}, {NegateX(MappingContext)}});
  MapKey({MappingContext, MoveAction, EKeys::Gamepad_Left2D});
  MapKey({MappingContext, LookAction, EKeys::Mouse2D});
  MapKey({MappingContext, LookAction, EKeys::Gamepad_Right2D});
  MapKey({MappingContext, JumpAction, EKeys::SpaceBar});
  MapKey({MappingContext, JumpAction, EKeys::Gamepad_FaceButton_Bottom});
}

void AThirdPersonCharacter::Move(const FInputActionValue &Value) {
  const FVector2D MovementVector = Value.Get<FVector2D>();
  DoMove(MovementVector.X, MovementVector.Y);
}

void AThirdPersonCharacter::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  DoLook(LookAxisVector.X, LookAxisVector.Y);
}
