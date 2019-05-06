// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_Cells.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AActor_Cells::AActor_Cells()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    nPanelWidth = 10;
    nPanelHeight = 10;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Panel(TEXT("StaticMesh'/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor'"));
    pPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel"));
    if (object_Panel.Succeeded()) pPanel->SetStaticMesh(object_Panel.Object);
    pPanel->SetWorldScale3D(FVector(nPanelWidth, nPanelHeight, 1.f));
    pPanel->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Cell(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    pCells = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cells"));
    if (object_Cell.Succeeded()) pCells->SetStaticMesh(object_Cell.Object);
    pCells->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AActor_Cells::BeginPlay()
{
	Super::BeginPlay();
	
    SetActorLocation(FVector(0, 0, 0));
}

// Called every frame
void AActor_Cells::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AActor_Cells::SetCell(FVector ImpactPoint) {
    int32 nX = FMath::CeilToInt(ImpactPoint.X);
    int32 nY = FMath::CeilToInt(ImpactPoint.Y);

    UE_LOG(LogTemp, Warning, TEXT("%d - %d"), nX, nY);

    pCells->AddInstance(FTransform(FQuat(FRotator()), FVector(nX, nY, 0)));
}
