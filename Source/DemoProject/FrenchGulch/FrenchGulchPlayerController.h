#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FrenchGulchPlayerController.generated.h"

class ATalkableTownsperson;
class UFrenchGulchRuntimeChatWidget;

UCLASS()
class DEMOPROJECT_API AFrenchGulchPlayerController : public APlayerController {
  GENERATED_BODY()

public:
  AFrenchGulchPlayerController();

protected:
  virtual void SetupInputComponent() override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "FrenchGulch|Interaction")
  float InteractionDistance;

  UPROPERTY()
  UFrenchGulchRuntimeChatWidget *ChatWidget;

  void InteractWithNearestTownsperson();
  ATalkableTownsperson *FindNearestTownsperson() const;
};
