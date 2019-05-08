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
    bool bStart;
    UPROPERTY(BlueprintReadonly)
        FText StartText;

    UFUNCTION(BlueprintCallable)
        void ChangeEvoluteSpeed(const float Value);
    UFUNCTION(BlueprintCallable)
        void StartGame();
};
