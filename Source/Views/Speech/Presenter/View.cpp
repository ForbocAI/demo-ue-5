// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/Speech/Presenter/View.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Features/Systems/Speech/Selectors.h"
#include "Features/Systems/Selectors.h"

namespace FG = ForbocAI::Game::Level;

ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames(
                     FG::RuntimeSelectors::SelectState())
                     .SceneRoot))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames(
                     FG::RuntimeSelectors::SelectState())
                     .SpeechPresentationMesh))),
      SpeechComponent(CreateDefaultSubobject<USpeechComponent>(
          FName(*FG::RuntimeSelectors::SelectViewNames(
                     FG::RuntimeSelectors::SelectState())
                     .SpeechComponent))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);

  const FG::FRuntimeState &State = FG::RuntimeSelectors::SelectState();
  const FG::FPlayerPresentationViewModel Presentation =
      FG::RuntimeSelectors::SelectPlayerPresentation(State);
  USkeletalMesh *Mesh =
      LoadObject<USkeletalMesh>(nullptr, *Presentation.MeshPath);
  check(Mesh);
  PresentationMesh->SetSkeletalMesh(Mesh);
}
