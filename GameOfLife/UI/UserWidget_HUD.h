// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserWidget_HUD.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API UUserWidget_HUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UUserWidget_HUD(const FObjectInitializer& objectInitializer);
	
protected:
    void NativeConstruct() override;

    bool bStart;
    bool bDragonVisible;
    bool bWireframeVisible;
    UPROPERTY(BlueprintReadonly)
        FText StartText;
    UPROPERTY(BlueprintReadonly)
        FText DragonText;
    UPROPERTY(BlueprintReadonly)
        FText WireframeText;

    class AGameModeBase_Game* pGameModeBase_Game;

    UFUNCTION(BlueprintCallable)
        void ChangeEvoluteSpeed(const float Value);
    UFUNCTION(BlueprintCallable)
        void ChangePanelSize(const float ValueX, const float ValueY);
    UFUNCTION(BlueprintCallable)
        void StartGame();
    UFUNCTION(BlueprintCallable)
        void ToggleDragon();
    UFUNCTION(BlueprintCallable)
        void ToggleWireframe();
};
