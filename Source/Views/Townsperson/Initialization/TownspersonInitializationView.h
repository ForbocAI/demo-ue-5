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

/** User Story: As a views townsperson initialization consumer, I need to invoke observe townsperson view defaults through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn FG::FTownspersonViewDefaults ObserveTownspersonViewDefaults( const FG::FTownspersonViewDefaultsRequest &Request) */
FG::FTownspersonViewDefaults ObserveTownspersonViewDefaults(
    const FG::FTownspersonViewDefaultsRequest &Request) {
  return FG::RuntimeSelectors::SelectTownspersonViewDefaults(Request);
}

/** User Story: As a views townsperson initialization consumer, I need to invoke observe townsperson presentation through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn FG::FTownspersonPresentationViewModel ObserveTownspersonPresentation() */
FG::FTownspersonPresentationViewModel ObserveTownspersonPresentation() {
  return FG::RuntimeSelectors::SelectTownspersonPresentation();
}

/** User Story: As a views townsperson initialization consumer, I need to invoke observe townsperson initial patrol through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn FG::FBotInitialPatrolLocationPayload ObserveTownspersonInitialPatrol(const TArray<FVector> &PatrolRoute, int32 &PatrolIndex) */
FG::FBotInitialPatrolLocationPayload
ObserveTownspersonInitialPatrol(const TArray<FVector> &PatrolRoute,
                                int32 &PatrolIndex) {
  PatrolIndex = FG::RuntimeSelectors::SelectBotInitialPatrolIndex(PatrolRoute);
  return FG::RuntimeSelectors::SelectBotInitialPatrolLocation({PatrolRoute});
}

/** User Story: As a views townsperson initialization consumer, I need to invoke observe townsperson patrol advance through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn FG::FBotPatrolAdvancePayload ObserveTownspersonPatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) */
FG::FBotPatrolAdvancePayload
ObserveTownspersonPatrolAdvance(const FG::FBotPatrolAdvanceRequest &Request) {
  return FG::RuntimeSelectors::SelectBotPatrolAdvance(Request);
}

/** User Story: As a views townsperson initialization consumer, I need to invoke observe interaction overlap through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn FG::FTownspersonInteractionOverlapViewModel ObserveInteractionOverlap( const FG::FTownspersonInteractionOverlapRequest &Request) */
FG::FTownspersonInteractionOverlapViewModel
ObserveInteractionOverlap(
    const FG::FTownspersonInteractionOverlapRequest &Request) {
  return FG::RuntimeSelectors::SelectTownspersonInteractionOverlap(Request);
}
} // namespace

/** User Story: As a views townsperson initialization consumer, I need to invoke atownsperson view through a stable signature so the views townsperson initialization workflow remains explicit and composable. @fn ATownspersonView::ATownspersonView() */
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
  WalkSpeed = Presentation.Movement.WalkSpeed;
  PauseDuration = Presentation.Movement.PauseDuration;
  PatrolArrivalDistance = Presentation.Movement.PatrolArrivalDistance;
  CharacterMeshPath = Presentation.Character.MeshPath;
  CharacterAnimationBlueprintClassPath =
      Presentation.Character.AnimationBlueprintClassPath;

  SceneRoot =
      CreateDefaultSubobject<USceneComponent>(FName(*ViewNames.Scene.Root));
  RootComponent = SceneRoot;

  CharacterMesh =
      CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*ViewNames.Townsperson.CharacterMesh));
  CharacterMesh->SetupAttachment(SceneRoot);
  CharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  CharacterMesh->SetRelativeLocation(Presentation.Character.MannequinOffset);
  CharacterMesh->SetRelativeRotation(Presentation.Character.MannequinRotation);
  CharacterMesh->SetRelativeScale3D(Presentation.Character.MannequinScale);
  CharacterMesh->SetVisibility(false);
  CharacterMesh->SetHiddenInGame(true);

  InteractionSphere =
      CreateDefaultSubobject<USphereComponent>(
          FName(*ViewNames.Townsperson.InteractionSphere));
  InteractionSphere->SetupAttachment(SceneRoot);
  InteractionSphere->SetSphereRadius(Presentation.Interaction.Radius);
  InteractionSphere->SetCollisionProfileName(
      FName(*ViewNames.Townsperson.OverlapProfile));

  NameText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.Townsperson.NameText));
  NameText->SetupAttachment(SceneRoot);
  NameText->SetRelativeLocation(Presentation.NameText.Location);
  NameText->SetHorizontalAlignment(EHTA_Center);
  NameText->SetWorldSize(Presentation.NameText.WorldSize);
  NameText->SetVisibility(false);
  NameText->SetHiddenInGame(true);

  PromptText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.Townsperson.PromptText));
  PromptText->SetupAttachment(SceneRoot);
  PromptText->SetRelativeLocation(Presentation.PromptText.Location);
  PromptText->SetHorizontalAlignment(EHTA_Center);
  PromptText->SetWorldSize(Presentation.PromptText.WorldSize);
  PromptText->SetText(
      FText::FromString(Presentation.Interaction.InteractionPrompt));
  PromptText->SetVisibility(false);

  DialogueText =
      CreateDefaultSubobject<UTextRenderComponent>(
          FName(*ViewNames.Townsperson.DialogueText));
  DialogueText->SetupAttachment(SceneRoot);
  DialogueText->SetRelativeLocation(Presentation.DialogueText.Location);
  DialogueText->SetHorizontalAlignment(EHTA_Center);
  DialogueText->SetWorldSize(Presentation.DialogueText.WorldSize);
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
