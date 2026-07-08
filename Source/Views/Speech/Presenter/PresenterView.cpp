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

ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames().SceneRoot))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames()
                     .SpeechPresentationMesh))),
      SpeechComponent(CreateDefaultSubobject<USpeechComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames()
                     .SpeechComponent))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);
  const FG::FPresentationViewModel Presentation =
      FG::RuntimeSelectors::SelectPlayerPresentation();
  USkeletalMesh *Mesh =
      LoadObject<USkeletalMesh>(nullptr, *Presentation.MeshPath);
  check(Mesh);
  PresentationMesh->SetSkeletalMesh(Mesh);
}
