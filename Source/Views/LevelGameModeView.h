#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LevelGameModeView.generated.h"

class ARuntimeLevelView;

UCLASS(Blueprintable)
class DEMOPROJECT_API ALevelGameModeView : public AGameModeBase {
  GENERATED_BODY()

public:
  ALevelGameModeView();

  virtual void StartPlay() override;
  virtual void RestartPlayer(AController *NewPlayer) override;

private:
  UPROPERTY()
  ARuntimeLevelView *RuntimeLevelView;
};
