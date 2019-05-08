// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBase_Game.generated.h"


// View Mode
UENUM(BlueprintType)
enum class EViewMode : uint8 {
    E_GOD UMETA(DisplayName = "God"),
    E_FREE UMETA(DisplayName = "Free")
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

    class AActor_Cells* pCells;
    class APlayerController* pPlayerController;

public:
    UFUNCTION(BlueprintCallable)
        void Start(const bool bStart);
    UFUNCTION(BlueprintCallable)
        void Reset();
    UFUNCTION(BlueprintCallable)
        void OneStep();
    UFUNCTION(BlueprintCallable)
        void SetViewMode(const EViewMode viewMode);

    void ChangeEvoluteSpeed(const float Speed);

    FORCEINLINE EViewMode GetViewMode() const { return ViewMode; }
    FORCEINLINE class APlayerController* GetPlayerController() const { return pPlayerController; }
    void SetGameMode();
    void SetUIMode();
};
