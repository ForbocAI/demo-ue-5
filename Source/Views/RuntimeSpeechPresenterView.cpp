#include "Views/RuntimeSpeechPresenterView.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "UObject/ConstructorHelpers.h"

ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          TEXT("PresentationMesh"))),
      SpeechComponent(
          CreateDefaultSubobject<USpeechComponent>(TEXT("SpeechComponent"))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);

  static ConstructorHelpers::FObjectFinder<USkeletalMesh> DefaultMesh(
      TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple."
           "SKM_Manny_Simple"));
  if (DefaultMesh.Succeeded()) {
    PresentationMesh->SetSkeletalMesh(DefaultMesh.Object);
  }
}
