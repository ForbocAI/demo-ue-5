// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Speech/Presenter/PresenterView.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Features/Systems/Speech/SpeechSelectors.h"
#include "Features/Systems/SystemsSelectors.h"

namespace FG = ForbocAI::Game::Level;

/** User Story: As a views speech presenter consumer, I need to invoke aruntime speech presenter view through a stable signature so the views speech presenter workflow remains explicit and composable. @fn ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView() */
ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames().Scene.Root))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames()
                     .Speech.PresentationMesh))),
      SpeechComponent(CreateDefaultSubobject<USpeechComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames()
                     .Speech.Component))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);
  const FG::FPresentationViewModel Presentation =
      FG::RuntimeSelectors::SelectPlayerPresentation();
  USkeletalMesh *Mesh =
      LoadObject<USkeletalMesh>(nullptr,
                                *Presentation.MeshAssets.MeshPath);
  check(Mesh);
  PresentationMesh->SetSkeletalMesh(Mesh);
}
