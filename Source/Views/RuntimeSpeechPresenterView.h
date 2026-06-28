#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeSpeechPresenterView.generated.h"

class USceneComponent;
class USkeletalMeshComponent;
class USpeechComponent;

/**
 * @brief Display-only actor that gives Blueprint assets a speech presentation
 * surface.
 *
 * User Story: As a Blueprint user, I need a source-controlled actor parent with
 * a mesh and USpeechComponent so speech and lip-sync events can be exercised
 * without adding runtime decision logic to view code.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API ARuntimeSpeechPresenterView : public AActor {
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
