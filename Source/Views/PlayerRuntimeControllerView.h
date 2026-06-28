#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Templates/SubclassOf.h"
#include "PlayerRuntimeControllerView.generated.h"

class ATownspersonView;
class URuntimeChatWidget;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

UCLASS(Blueprintable)
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

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeChatWidget> RuntimeConversationWidgetClass;

  UPROPERTY()
  URuntimeChatWidget *RuntimeConversationWidget;

  void InteractWithNearestTownsperson();
  void PresentConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);
};
