// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "Pawn_View.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API APawn_View : public APawn
{
	GENERATED_BODY()
	
public:
    APawn_View();
	
protected:
    class UCameraComponent* pCamera;

    float speed;
    class AGameModeBase_Game* pGameMode_Game;

    void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void BeginPlay() override;

    void Turn(const float axis);
    void LookUp(const float axis);
    void MoveForward(const float axis);
    void MoveRight(const float axis);

    void SetCell();
};
