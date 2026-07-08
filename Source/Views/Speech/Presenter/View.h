// View boundary: keep this file equivalent to markup/html/jsx presentation.
// Put runtime decisions, data derivation, and business logic in Features using
// Redux/RTK skills: actions, slices, reducers, selectors, thunks/listeners,
// adapters, and ECS/domain systems. Views consume feature-prepared payloads.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "View.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class USpeechComponent;

/**
 * @brief Display-only actor that gives Blueprint assets a speech presentation
 * surface.
 *
 * User Story: As a Blueprint user, I need a source-controlled actor parent with
 * a mesh and USpeechComponent so speech and lip-sync events can be exercised
 * while runtime decisions stay in Features.
 */
UCLASS(Blueprintable)
class FORBOCAIDEMO_API ARuntimeSpeechPresenterView : public AActor {
  GENERATED_BODY()

public:
  ARuntimeSpeechPresenterView();

private:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Speech",
            meta = (AllowPrivateAccess = "true"))
  USceneComponent *SceneRoot;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Speech",
            meta = (AllowPrivateAccess = "true"))
  USkeletalMeshComponent *PresentationMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level|Speech",
            meta = (AllowPrivateAccess = "true"))
  USpeechComponent *SpeechComponent;
};
