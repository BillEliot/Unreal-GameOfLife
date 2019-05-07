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
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    class UStaticMeshComponent* pPanel;
    class UInstancedStaticMeshComponent* pCells;

    // Timer
    FTimerHandle TimerHandle;
    void TimerFunction();

    uint32 nPanelWidthScale;
    uint32 nPanelHeightScale;
    uint32 nCellWidthScale;
    uint32 nCellHeightScale;

    FVector PanelSize;
    FVector CellSize;

    float EvoluteSpeed;

    TArray<int32> Array_DyingCells;
    TArray<FVector2D> Array_WillActiveCells;
    TArray<FVector2D> Array_PotentialCells;
    TArray<FVector2D> Array_ActiveCells;

    FVector CalcCellLocation(FVector ImpactPoint);
    int32 GetAroundActiveCells(const int32 nIndex);
    int32 GetAroundActiveCells(const FVector2D Location);
    void DetectAndAddToPotentialArray(const int32 nIndex);
    void DetectAndAddToPotentialArray(const FVector2D Location);

public:	
    FORCEINLINE class UStaticMeshComponent* GetPanel() const { return pPanel; }
    void SetCell(FVector ImpactPoint);
    void Start(const bool bStart);
    void OneStep();
};
