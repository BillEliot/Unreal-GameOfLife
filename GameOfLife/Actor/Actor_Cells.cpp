// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_Cells.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Public/TimerManager.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AActor_Cells::AActor_Cells()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    nPanelWidthScale = 5;
    nPanelHeightScale = 5;
    nCellWidthScale = 1;
    nCellHeightScale = 1;
    EvoluteSpeed = 1.f;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Panel(TEXT("StaticMesh'/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> object_Material_Panel(TEXT("Material'/Game/M_DefaultPanel.M_DefaultPanel'"));
    pPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel"));
    if (object_Panel.Succeeded()) pPanel->SetStaticMesh(object_Panel.Object);
    if (object_Material_Panel.Succeeded()) pPanel->SetMaterial(0, object_Material_Panel.Object);
    pPanel->SetWorldScale3D(FVector(nPanelWidthScale, nPanelHeightScale, 1.f));
    pPanel->SetupAttachment(RootComponent);
    PanelSize = pPanel->GetStaticMesh()->GetBoundingBox().GetSize();
    PanelSize.X *= nPanelWidthScale;
    PanelSize.Y *= nPanelHeightScale;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Cell(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> object_Material_Cell(TEXT("Material'/Game/M_DefaultCell.M_DefaultCell'"));
    pCells = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cells"));
    if (object_Cell.Succeeded()) pCells->SetStaticMesh(object_Cell.Object);
    if (object_Material_Cell.Succeeded()) pCells->SetMaterial(0, object_Material_Cell.Object);
    pCells->SetupAttachment(RootComponent);
    CellSize = pCells->GetStaticMesh()->GetBoundingBox().GetSize();
    CellSize.X *= nCellWidthScale;
    CellSize.Y *= nCellHeightScale;
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

/*************************************************
Function:         CalcCellLocation()
Description:    Calculate a cell's location based on clicked position
Calls:               this->SetCell()
Input:              ImpactPoint: Clicked position
Return:            A new cell's location
*************************************************/
FVector AActor_Cells::CalcCellLocation(FVector ImpactPoint) {
    FVector CellLocation(0,0,0);

    // X
    if (ImpactPoint.X >= 0) CellLocation.X = int32(ImpactPoint.X / CellSize.X) * CellSize.X + (CellSize.X / 2);
    else CellLocation.X = int32(ImpactPoint.X / CellSize.X) * CellSize.X - (CellSize.X / 2);
    // Y
    if (ImpactPoint.Y >= 0) CellLocation.Y = int32(ImpactPoint.Y / CellSize.Y) * CellSize.Y + (CellSize.Y / 2);
    else CellLocation.Y = int32(ImpactPoint.Y / CellSize.Y) * CellSize.Y - (CellSize.Y / 2);
    
    return CellLocation;
}

/*************************************************
Function:         SetCell()
Description:    Set a new cell on the panel
Calls:               APawn_View::SetCell()
Input:              ImpactPoint: Clicked position
Return:            NULL
*************************************************/
void AActor_Cells::SetCell(FVector ImpactPoint) {
    FVector CellLocation = CalcCellLocation(ImpactPoint);

    pCells->AddInstance(FTransform(FQuat(FRotator(0,0,0)), CellLocation));
}

/*************************************************
Function:         OneStep()
Description:    Take one step to evolute by TimerFunction
Calls:               AGameModeBase_Game::OneStep()
Input:              NULL
Return:            NULL
*************************************************/
void AActor_Cells::OneStep() {
    // TimerFunction();
}

/*************************************************
Function:         Start()
Description:    Start/Stop to evolute by TimerFunction
Calls:               AGameModeBase_Game::Start()
Input:              bStart: Start or Stop
Return:            NULL
*************************************************/
void AActor_Cells::Start(const bool bStart) {
    if (bStart) {
        // Add Potential Cells Array
        for (int32 index = 0; pCells->GetInstanceCount(); ++index) {
            DetectAndAddToPotentialArray(index);
        }

        if (TimerHandle.IsValid()) GetWorldTimerManager().UnPauseTimer(TimerHandle);
        else GetWorldTimerManager().SetTimer(TimerHandle, this, &AActor_Cells::TimerFunction, EvoluteSpeed, false, 0);
    }
    else {
        GetWorldTimerManager().PauseTimer(TimerHandle);
    }
}
void AActor_Cells::TimerFunction() {
    // Detect Active Cells
    for (int32 index = 0; index < pCells->GetInstanceCount(); ++index) {
        int32 nCells = GetAroundActiveCells(index);
        if (nCells != 2 && nCells != 3) Array_DyingCells.AddUnique(index);
    }
    // Detect Potential Cells
    for (auto Location : Array_PotentialCells) {
        int32 nCells = GetAroundActiveCells(Location);
        if (nCells == 3) Array_WillActiveCells.AddUnique(Location);
    }
    // Clear Potential Cells Array
    Array_PotentialCells.Empty();
    // Update Cells' Next Generation
    for (auto index : Array_DyingCells) {
        pCells->RemoveInstance(index);
    }
    for (auto Location : Array_WillActiveCells) {
        // Add to Potential Cells Array
        DetectAndAddToPotentialArray(Location);
        // Create new
        pCells->AddInstance(FTransform(FQuat(FRotator(0,0,0)), FVector(Location, 0)));
    }
    // Clear Array
    Array_DyingCells.Empty();
    Array_WillActiveCells.Empty();
}

/*************************************************
Function:         GetAroundActiveCells()
Description:    Get the number of cells around the cell(8 directions)
Calls:               this->TimerFunction()
Input:              int32: the instanced cell's index
Return:            The number of cells around the cell(8 directions)
*************************************************/
int32 AActor_Cells::GetAroundActiveCells(const int32 nIndex) {
    FTransform CellTransform;
    TArray<FHitResult> HitResults;
    
    // Get transform
    pCells->GetInstanceTransform(nIndex, CellTransform);
    // Get around cells
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredComponent(pPanel);
    GetWorld()->SweepMultiByChannel(HitResults, CellTransform.GetLocation(), CellTransform.GetLocation(), FQuat(FRotator(0,0,0)), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeBox(FVector(CellSize.X / 2 * 3, CellSize.Y / 2 * 3, CellSize.Z / 2)), CollisionQueryParams);

    // Ignore self
    return HitResults.Num() - 1;
}
int32 AActor_Cells::GetAroundActiveCells(const FVector2D Location) {
    TArray<FHitResult> HitResults;

    // Get around cells
    FCollisionQueryParams CollisionQueryParams;
    CollisionQueryParams.AddIgnoredComponent(pPanel);
    GetWorld()->SweepMultiByChannel(HitResults, FVector(Location, 0), FVector(Location, 0), FQuat(FRotator(0, 0, 0)), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeBox(FVector(CellSize.X / 2 * 3, CellSize.Y / 2 * 3, CellSize.Z / 2)), CollisionQueryParams);

    // Ignore self
    return HitResults.Num() - 1;
}

/*************************************************
Function:         DetectAndAddToPotentialArray()
Description:    Search around 8 directions to add inactivated cells
Calls:               
Input:              FVector: The cell's location
Return:            NULL
*************************************************/
void AActor_Cells::DetectAndAddToPotentialArray(const FVector2D Location) {
    FHitResult HitResult;
    bool bHit = false;
    // Down
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y));
    // Up
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y));
    // Left
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X, Location.Y - CellSize.Y));
    // Right
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X, Location.Y + CellSize.Y));
    // DownLeft
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y - CellSize.Y));
    // DownRight
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y + CellSize.Y));
    // UpLeft
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y - CellSize.Y));
    // UpRight
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y + CellSize.Y));
}
void AActor_Cells::DetectAndAddToPotentialArray(const int32 nIndex) {
    FHitResult HitResult;
    FTransform CellTransform;
    bool bHit = false;

    pCells->GetInstanceTransform(nIndex, CellTransform);
    FVector2D Location(CellTransform.GetLocation().X, CellTransform.GetLocation().Y);
    // Down
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y));
    // Up
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y));
    // Left
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X, Location.Y - CellSize.Y));
    // Right
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X, Location.Y + CellSize.Y));
    // DownLeft
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y - CellSize.Y));
    // DownRight
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X - CellSize.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X - CellSize.X, Location.Y + CellSize.Y));
    // UpLeft
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y - CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y - CellSize.Y));
    // UpRight
    bHit = GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location.X + CellSize.X, Location.Y + CellSize.Y, 0), ECollisionChannel::ECC_Visibility);
    if (!bHit) Array_PotentialCells.AddUnique(FVector2D(Location.X + CellSize.X, Location.Y + CellSize.Y));
}
