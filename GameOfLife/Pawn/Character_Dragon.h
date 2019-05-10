// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character_Dragon.generated.h"

UCLASS()
class GAMEOFLIFE_API ACharacter_Dragon : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacter_Dragon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
        class USplineComponent* pSpline;
    class UAudioComponent* pAudio;

    float Time;
    // Timer
    FTimerHandle TimerHandle;
    void TimerFunction();

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    void ToggleDragon(const bool bDragonVisible);
};
