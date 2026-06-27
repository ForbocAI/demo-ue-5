#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RuntimePlayerController.generated.h"

class ATalkableTownsperson;
class URuntimeChatWidget;

UCLASS()
class DEMOPROJECT_API ARuntimePlayerController : public APlayerController {
  GENERATED_BODY()

public:
  ARuntimePlayerController();

protected:
  virtual void SetupInputComponent() override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "Level|Interaction")
  float InteractionDistance;

  UPROPERTY()
  URuntimeChatWidget *ChatWidget;

  void InteractWithNearestTownsperson();
  ATalkableTownsperson *FindNearestTownsperson() const;
  void ShowConversation(const FString &Name, const FString &SpeakerRole,
                        const FString &PlayerLine, const FString &Reply);
};
