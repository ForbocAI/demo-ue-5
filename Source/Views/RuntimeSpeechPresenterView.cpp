#include "Views/RuntimeSpeechPresenterView.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

namespace FG = ForbocAI::Demo::Level;

ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          TEXT("PresentationMesh"))),
      SpeechComponent(
          CreateDefaultSubobject<USpeechComponent>(TEXT("SpeechComponent"))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);

  const FG::FRuntimeState State = FG::Store::GetStore().getState();
  const FG::FPlayerPresentationViewModel Presentation =
      FG::RuntimeSelectors::SelectPlayerPresentation(State);
  if (USkeletalMesh *Mesh =
          LoadObject<USkeletalMesh>(nullptr, *Presentation.MeshPath)) {
    PresentationMesh->SetSkeletalMesh(Mesh);
  }
}
