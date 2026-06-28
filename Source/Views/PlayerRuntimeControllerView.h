#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerRuntimeControllerView.generated.h"

class ATownspersonView;
class URuntimeChatWidget;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

UCLASS()
class DEMOPROJECT_API APlayerRuntimeControllerView
    : public APlayerController {
  GENERATED_BODY()

public:
  APlayerRuntimeControllerView();

protected:
  virtual void SetupInputComponent() override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "Level|Interaction")
  float InteractionDistance;

  UPROPERTY()
  URuntimeChatWidget *RuntimeConversationWidget;

  void InteractWithNearestTownsperson();
  void PresentConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);
};
