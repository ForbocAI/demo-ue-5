#include "Views/PlayerCharacterView.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Features/Entities/Characters/Player/PlayerSelectors.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "InputTriggers.h"
#include "InputCoreTypes.h"

namespace FGP = ForbocAI::Demo::Level::PlayerSelectors;

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

APlayerCharacterView::APlayerCharacterView()
    : MappingContext(nullptr), MouseMappingContext(nullptr), MoveAction(nullptr),
      LookAction(nullptr), MouseLookAction(nullptr), JumpAction(nullptr) {
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
}

void APlayerCharacterView::DoMove(float Right, float Forward) {
  const ForbocAI::Demo::Level::FPlayerMovementInputViewModel Model =
      FGP::SelectMovementInput({Controller ? Controller->GetControlRotation()
                                           : FRotator::ZeroRotator,
                                Right, Forward, Controller != nullptr});
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
  if (USkeletalMesh *CharacterMesh = LoadObject<USkeletalMesh>(
          nullptr,
          TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple"))) {
    GetMesh()->SetSkeletalMesh(CharacterMesh);
    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -CapsuleHalfHeight));
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("ThirdPerson: required Manny skeletal mesh is missing."));
  }

  if (UClass *AnimClass = LoadClass<UAnimInstance>(
          nullptr,
          TEXT("/Game/Characters/Mannequins/Anims/Unarmed/ABP_Unarmed.ABP_Unarmed_C"))) {
    GetMesh()->SetAnimInstanceClass(AnimClass);
  } else {
    UE_LOG(LogTemp, Error,
           TEXT("ThirdPerson: required ABP_Unarmed animation blueprint is "
                "missing."));
  }
}

void APlayerCharacterView::ConfigureEnhancedInput() {
  MoveAction = InputAction(TEXT("/Game/Input/Actions/IA_Move.IA_Move"), this,
                           TEXT("IA_Move"), EInputActionValueType::Axis2D);
  LookAction = InputAction(TEXT("/Game/Input/Actions/IA_Look.IA_Look"), this,
                           TEXT("IA_Look"), EInputActionValueType::Axis2D);
  MouseLookAction =
      InputAction(TEXT("/Game/Input/Actions/IA_MouseLook.IA_MouseLook"), this,
                  TEXT("IA_MouseLook"), EInputActionValueType::Axis2D);
  JumpAction = InputAction(TEXT("/Game/Input/Actions/IA_Jump.IA_Jump"), this,
                           TEXT("IA_Jump"), EInputActionValueType::Boolean);

  MappingContext = LoadObject<UInputMappingContext>(
      nullptr, TEXT("/Game/Input/IMC_Default.IMC_Default"));
  MouseMappingContext = LoadObject<UInputMappingContext>(
      nullptr, TEXT("/Game/Input/IMC_MouseLook.IMC_MouseLook"));
  if (MappingContext && MouseMappingContext) {
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
  MapKey({MappingContext, MouseLookAction, EKeys::Mouse2D});
  MapKey({MappingContext, JumpAction, EKeys::SpaceBar});
  MapKey({MappingContext, JumpAction, EKeys::Gamepad_FaceButton_Bottom});
  MouseMappingContext = nullptr;
}

void APlayerCharacterView::Move(const FInputActionValue &Value) {
  const FVector2D MovementVector = Value.Get<FVector2D>();
  DoMove(MovementVector.X, MovementVector.Y);
}

void APlayerCharacterView::Look(const FInputActionValue &Value) {
  const FVector2D LookAxisVector = Value.Get<FVector2D>();
  DoLook(LookAxisVector.X, LookAxisVector.Y);
}
