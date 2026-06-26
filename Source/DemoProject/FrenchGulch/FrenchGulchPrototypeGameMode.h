#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FrenchGulchPrototypeGameMode.generated.h"

class AFrenchGulchRuntimeLevel;

UCLASS()
class DEMOPROJECT_API AFrenchGulchPrototypeGameMode : public AGameModeBase {
  GENERATED_BODY()

public:
  AFrenchGulchPrototypeGameMode();

  virtual void StartPlay() override;
  virtual void RestartPlayer(AController *NewPlayer) override;

private:
  UPROPERTY()
  AFrenchGulchRuntimeLevel *RuntimeLevel;
};
