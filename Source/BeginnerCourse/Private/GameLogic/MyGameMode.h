#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "MyGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);

UCLASS()
class AMyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameMode();

	UPROPERTY(BlueprintAssignable)
	FOnGameOver OnGameOver;

	void TriggerGameOver() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void GameOverHandler();
};
