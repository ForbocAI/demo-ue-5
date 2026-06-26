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
  UPROPERTY(EditDefaultsOnly, Category = "Map|Interaction")
  float InteractionDistance;

  UPROPERTY()
  URuntimeChatWidget *ChatWidget;

  void InteractWithNearestTownsperson();
  ATalkableTownsperson *FindNearestTownsperson() const;
};
