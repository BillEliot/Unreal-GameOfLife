// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_Cell.generated.h"

UCLASS()
class GAMEOFLIFE_API AActor_Cell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_Cell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    class UStaticMeshComponent* pMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
