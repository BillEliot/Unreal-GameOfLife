// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DefaultPawn.h"
#include "DefaultPawn_View.generated.h"

/**
 * 
 */
UCLASS()
class GAMEOFLIFE_API ADefaultPawn_View : public ADefaultPawn
{
	GENERATED_BODY()
	
public:
    ADefaultPawn_View();
	
protected:
    class UCameraComponent* pCamera;
    class USpringArmComponent* pSpringArm;

    float speed;
    class AGameModeBase_Game* pGameMode_Game;

    void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    void BeginPlay() override;

    void MoveForward(const float axis);
    void MoveRight(const float axis);
    void Scale(const float axis);

    void SetCell();
};
