#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Templates/SubclassOf.h"
#include "PlayerRuntimeControllerView.generated.h"

class ATownspersonView;
class URuntimeChatWidget;
class URuntimeStatsWidget;

namespace ForbocAI::Demo::UI {
struct FRuntimeConversationViewModel;
}

/**
 * @brief Player-controller boundary that converts UE input into RTK events.
 *
 * Architecture: This view observes Unreal actors/input, dispatches action
 * creators or thunks, and reads selector output. Target selection, dialogue
 * request composition, UI payload composition, and state transitions belong to
 * reducers/thunks in the single runtime store, not to this controller.
 *
 * User story: As a player, pressing the interaction key starts a one-way
 * action -> reducer/thunk -> selector -> view flow for the nearest NPC.
 */
UCLASS(Blueprintable)
class DEMOPROJECT_API APlayerRuntimeControllerView
    : public APlayerController {
  GENERATED_BODY()

public:
  APlayerRuntimeControllerView();

protected:
  virtual void BeginPlay() override;
  virtual void SetupInputComponent() override;

private:
  UPROPERTY(EditDefaultsOnly, Category = "Level|Interaction")
  float InteractionDistance;

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeChatWidget> RuntimeConversationWidgetClass;

  UPROPERTY()
  URuntimeChatWidget *RuntimeConversationWidget;

  UPROPERTY(EditDefaultsOnly, Category = "Level|UI")
  TSubclassOf<URuntimeStatsWidget> RuntimeStatsWidgetClass;

  UPROPERTY()
  URuntimeStatsWidget *RuntimeStatsWidget;

  /**
   * @brief Observes nearby townsperson actors and dispatches the interaction
   * candidate action/thunk workflow.
   */
  void InteractWithNearestTownsperson();

  /**
   * @brief Applies reducer-owned conversation display data to the chat widget.
   */
  void PresentConversationViewModel(
      const ForbocAI::Demo::UI::FRuntimeConversationViewModel &Conversation);

  /**
   * @brief Ensures the runtime stats overlay is visible in the viewport.
   */
  void PresentRuntimeStatsWidget();
};
