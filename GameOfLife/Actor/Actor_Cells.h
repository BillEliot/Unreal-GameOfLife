// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actor_Cells.generated.h"

UCLASS()
class GAMEOFLIFE_API AActor_Cells : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_Cells();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    class UStaticMeshComponent* pPanel;
    class UInstancedStaticMeshComponent* pCells;

    uint32 nPanelWidth;
    uint32 nPanelHeight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    FORCEINLINE class UStaticMeshComponent* GetPanel() const { return pPanel; }
    void SetCell(FVector ImpactPoint);
};
