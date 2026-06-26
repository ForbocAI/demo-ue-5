#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PrototypeGameMode.generated.h"

class ARuntimeLevel;

UCLASS()
class DEMOPROJECT_API APrototypeGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  APrototypeGameMode();

  virtual void StartPlay() override;
  virtual void RestartPlayer(AController *NewPlayer) override;

private:
  UPROPERTY()
  ARuntimeLevel *RuntimeLevel;
};
