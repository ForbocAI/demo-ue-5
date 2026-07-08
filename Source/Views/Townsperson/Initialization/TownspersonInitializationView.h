#pragma once

// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Townsperson/TownspersonView.h"

#include "Animation/AnimInstance.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "Core/frmt.hpp"
#include "Engine/SkeletalMesh.h"
#include "Features/Systems/SystemsSelectors.h"
#include "GameFramework/Pawn.h"
#include "Views/Skeletal/Lod/Clamp/ClampView.h"

namespace FG = ForbocAI::Game::Level;

namespace {

FG::FTownspersonViewDefaults ObserveTownspersonViewDefaults(
    const FG::FTownspersonViewDefaultsRequest &Request) {
  return FG::RuntimeSelectors::SelectTownspersonViewDefaults(Request);
}

FG::FTownspersonPresentationViewModel ObserveTownspersonPresentation() {
  return FG::RuntimeSelectors::SelectTownspersonPresentation();
}

FG::FBotInitialPatrolLocationPayload
ObserveTownspersonInitialPatrol(const TArray<FVector> &PatrolRoute,
                                int32 &PatrolIndex) {
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(PatrolRoute);
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation({PatrolRoute});
}

FG::FBotPatrolAdvancePayload
ObserveTownspersonPatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(Request);
}

FG::FTownspersonInteractionOverlapViewModel
ObserveInteractionOverlap(
    const FG::FTownspersonInteractionOverlapRequest &Request) {
  return FG::RuntimeSelectors::SelectTownspersonInteractionOverlap(Request);
}
} // namespace

ATownspersonView::ATownspersonView()
    : WalkSpeed(0.0f), PauseDuration(0.0f), PatrolIndex(0),
      PauseRemaining(0.0f), PatrolArrivalDistance(0.0f),
      bPlayerNearby(false), CurrentLod() {
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickInterval =
      FG::RuntimeSelectors::SelectBotSettings().PatrolTickIntervalSeconds;
  const FG::FTownspersonPresentationViewModel Presentation =
      ObserveTownspersonPresentation();
  const ForbocAI::Game::Data::FViewNameSettings &ViewNames =
      FG::RuntimeSelectors::SelectViewNames();
  WalkSpeed = Presentation.WalkSpeed;
  PauseDuration = Presentation.PauseDuration;
  PatrolArrivalDistance = Presentation.PatrolArrivalDistance;
  CharacterMeshPath = Presentation.MeshPath;
  CharacterAnimationBlueprintClassPath =
      Presentation.AnimationBlueprintClassPath;

  SceneRoot =
      CreateDefaultSubobject<USceneComponent>(FName(*ViewNames.SceneRoot));
  RootComponent = SceneRoot;

  CharacterMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.TownspersonCharacterMesh));
  CharacterMesh->SetupAttachment(SceneRoot);
  CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CharacterMesh->SetRelativeLocation(Presentation.MannequinOffset);
  CharacterMesh->SetRelativeRotation(Presentation.MannequinRotation);
  CharacterMesh->SetRelativeScale3D(Presentation.MannequinScale);
  CharacterMesh->SetVisibility(false);
  CharacterMesh->SetHiddenInGame(true);

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(
          FName(*ViewNames.TownspersonInteractionSphere));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(Presentation.InteractionRadius);
  InteractionSphere->SetCollisionProfileName(
      FName(*ViewNames.TownspersonOverlapProfile));

  NameText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonNameText));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameTextLocation);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameTextWorldSize);
  NameText->SetVisibility(false);
  NameText->SetHiddenInGame(true);

  PromptText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonPromptText));
  PromptText->SetupAttachment(SceneRoot);
  PromptText->SetRelativeLocation(Presentation.PromptTextLocation);
  PromptText->SetHorizontalAlignment(EHTA_Center);
  PromptText->SetWorldSize(Presentation.PromptTextWorldSize);
  PromptText->SetText(FText::FromString(Presentation.InteractionPrompt));
  PromptText->SetVisibility(false);

  DialogueText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.TownspersonDialogueText));
  DialogueText->SetupAttachment(SceneRoot);
  DialogueText->SetRelativeLocation(Presentation.DialogueTextLocation);
  DialogueText->SetHorizontalAlignment(EHTA_Center);
  DialogueText->SetWorldSize(Presentation.DialogueTextWorldSize);
  DialogueText->SetVisibility(false);

  ConfigureSampleCharacterAsset();

  const ForbocAI::Game::Level::FTownspersonViewDefaults Defaults =
      ObserveTownspersonViewDefaults({});
  TownspersonId = Defaults.Id;
  TownspersonName = Defaults.Name;
  TownspersonRole = Defaults.Role;
  Persona = Defaults.Persona;
  InteractionPrompt = Defaults.InteractionPrompt;
  DefaultPlayerLine = Defaults.DefaultPlayerLine;
  PinnedResponse = FString();
}
