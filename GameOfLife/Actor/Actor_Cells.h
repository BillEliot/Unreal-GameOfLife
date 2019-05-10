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

    FVector PanelSize;
    FVector CellSize;

    float EvoluteSpeed;

    TArray<int32> Array_DyingCells;
    TMap<FVector2D, int32> Map_PotentialCells;

    FVector CalcCellLocation(FVector ImpactPoint);
    int32 GetAroundActiveCells(const int32 nIndex);
    bool CheckLocation(const FVector2D Location);
    bool IsOnPanel(const int32 nIndex);
    void NotifyPotentialCells(const int32 nIndex);

public:	
    FORCEINLINE class UStaticMeshComponent* GetPanel() const { return pPanel; }
    FORCEINLINE class UInstancedStaticMeshComponent* GetCells() const { return pCells; }
    void SetCell(FVector ImpactPoint);
    void UnsetCell(FVector ImpactPoint);
    void Start(const bool bStart);
    void Reset();
    void OneStep();
    void ChangeEvoluteSpeed(const float Speed);
    void ChangePanelSize(const int32 nWidthScale, const int32 nHeightScale);
    void ToogleWireframe(const bool bWireframeVisible);
};
