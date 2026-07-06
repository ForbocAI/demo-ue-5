// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#include "Views/RuntimeSpeechPresenterView.h"

#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Features/Systems/Speech/SpeechAdapters.h"
#include "Features/Systems/Runtime/RuntimeSelectors.h"
#include "Store.h"

namespace FG = ForbocAI::Game::Level;

ARuntimeSpeechPresenterView::ARuntimeSpeechPresenterView()
    : SceneRoot(CreateDefaultSubobject<USceneComponent>(
          FName(*FG::RuntimeSelectors::SelectRuntimeViewNames(
                     FG::Store::GetStore().getState())
                     .SceneRoot))),
      PresentationMesh(CreateDefaultSubobject<USkeletalMeshComponent>(
          FName(*FG::RuntimeSelectors::SelectRuntimeViewNames(
                     FG::Store::GetStore().getState())
                     .SpeechPresentationMesh))),
      SpeechComponent(CreateDefaultSubobject<USpeechComponent>(
          FName(*FG::RuntimeSelectors::SelectRuntimeViewNames(
                     FG::Store::GetStore().getState())
                     .SpeechComponent))) {
  RootComponent = SceneRoot;
  PresentationMesh->SetupAttachment(SceneRoot);

  const FG::FRuntimeState &State = FG::Store::GetStore().getState();
  const FG::FPlayerPresentationViewModel Presentation =
      FG::RuntimeSelectors::SelectPlayerPresentation(State);
  USkeletalMesh *Mesh =
      LoadObject<USkeletalMesh>(nullptr, *Presentation.MeshPath);
  check(Mesh);
  PresentationMesh->SetSkeletalMesh(Mesh);
}
