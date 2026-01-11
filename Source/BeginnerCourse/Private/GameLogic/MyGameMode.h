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
};
