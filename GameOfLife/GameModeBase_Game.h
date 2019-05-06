// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBase_Game.generated.h"


// View Mode
enum EViewMode {
    E_GOD,
    E_FREE
};

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API AGameModeBase_Game : public AGameModeBase
{
	GENERATED_BODY()
	
public:
    AGameModeBase_Game();

protected:
    void BeginPlay() override;

    EViewMode ViewMode;
    class APlayerController* pPlayerController;

public:
    FORCEINLINE EViewMode GetViewMode() const { return ViewMode; }
    FORCEINLINE class APlayerController* GetPlayerController() const { return pPlayerController; }
    void SetGameMode();
    void SetUIMode();
};
