// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUD_Game.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API AHUD_Game : public AHUD
{
	GENERATED_BODY()
	
	
protected:
    void BeginPlay() override;

    class UUserWidget_HUD* pHUD;
};
