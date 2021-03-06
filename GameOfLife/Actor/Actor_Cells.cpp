// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_Cells.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Actor/Actor_Cell.h"
#include "Engine/StaticMesh.h"
#include "Public/TimerManager.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AActor_Cells::AActor_Cells()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    EvoluteSpeed = 1.f;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    
    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Panel(TEXT("StaticMesh'/Engine/MapTemplates/SM_Template_Map_Floor.SM_Template_Map_Floor'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> object_Material_Panel(TEXT("Material'/Game/M_DefaultPanel.M_DefaultPanel'"));
    pPanel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Panel"));
    if (object_Panel.Succeeded()) pPanel->SetStaticMesh(object_Panel.Object);
    if (object_Material_Panel.Succeeded()) pPanel->SetMaterial(0, object_Material_Panel.Object);
    pPanel->SetWorldScale3D(FVector(5.f, 5.f, 1.f));
    pPanel->SetupAttachment(RootComponent);
    // Default panel size is FVector(1000.f, 1000.f, 1.f)
    PanelSize = pPanel->GetStaticMesh()->GetBoundingBox().GetSize();
    // Default scale is 5
    PanelSize.X *= 5;
    PanelSize.Y *= 5;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Cell(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> object_Material_Cell(TEXT("Material'/Game/M_DefaultCell.M_DefaultCell'"));
    pCells = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cells"));
    if (object_Cell.Succeeded()) pCells->SetStaticMesh(object_Cell.Object);
    if (object_Material_Cell.Succeeded()) pCells->SetMaterial(0, object_Material_Cell.Object);
    pCells->SetupAttachment(RootComponent);
    CellSize = pCells->GetStaticMesh()->GetBoundingBox().GetSize();
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

void AActor_Cells::ToogleWireframe(const bool bWireframeVisible) {
    if (bWireframeVisible) {
        UMaterialInterface* pM_Wireframe = LoadObject<UMaterialInterface>(this, TEXT("Material'/Game/M_WireframePanel.M_WireframePanel'"));
        if (pM_Wireframe) pPanel->SetMaterial(0, pM_Wireframe);
    }
    else {
        UMaterialInterface* pM_Default = LoadObject<UMaterialInterface>(this, TEXT("Material'/Game/M_DefaultPanel.M_DefaultPanel'"));
        if (pM_Default) pPanel->SetMaterial(0, pM_Default);
    }
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
Function:         UnsetCell()
Description:    Remove a created instanced cell
Calls:               APawn_View::UnsetCell()
Input:              ImpactPoint: Clicked position
Return:            NULL
*************************************************/
void AActor_Cells::UnsetCell(FVector ImpactPoint) {
    FVector CellLocation = CalcCellLocation(ImpactPoint);

    // Due to the characteristics of UInstancedStaticMeshComponent, we must enum it
    for (int32 nIndex = 0; nIndex < pCells->GetInstanceCount(); ++nIndex) {
        FTransform CellTransform;
        pCells->GetInstanceTransform(nIndex, CellTransform);
        if (CellTransform.GetLocation() == CellLocation) {
            pCells->RemoveInstance(nIndex);
            break;
        }
    }
}

/*************************************************
Function:         OneStep()
Description:    Take one step to evolute by TimerFunction
Calls:               AGameModeBase_Game::OneStep()
Input:              NULL
Return:            NULL
*************************************************/
void AActor_Cells::OneStep() {
    TimerFunction();
}

/*************************************************
Function:         Reset()
Description:    Reset the game
Calls:               AGameModeBase_Game::Reset()
Input:              NULL
Return:            NULL
*************************************************/
void AActor_Cells::Reset() {
    if (TimerHandle.IsValid()) GetWorldTimerManager().PauseTimer(TimerHandle);
    pCells->ClearInstances();
    Array_DyingCells.Empty();
    Map_PotentialCells.Empty();
}

/*************************************************
Function:         ChangeEvoluteSpeed()
Description:    Change the speed of evolution
Calls:               AGameModeBase_Game::ChangeEvoluteSpeed()
Input:              float: Evolution speed
Return:            NULL
*************************************************/
void AActor_Cells::ChangeEvoluteSpeed(const float Speed) {
    EvoluteSpeed = Speed;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AActor_Cells::TimerFunction, EvoluteSpeed, true);
}

/*************************************************
Function:         ChangePanelSize()
Description:    Change size of the panel
Calls:               AGameModeBase_Game::ChangePanelSize()
Input:              int32: New panel width scale
                       int32: New panel height scale
Return:            NULL
*************************************************/
void AActor_Cells::ChangePanelSize(const int32 nWidthScale, const int32 nHeightScale) {
    pPanel->SetWorldScale3D(FVector(nWidthScale, nHeightScale, 1.f));
    pPanel->SetWorldLocation(FVector(0,0,0));

    PanelSize.X = 1000.f * nWidthScale;
    PanelSize.Y = 1000.f * nHeightScale;
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
        if (TimerHandle.IsValid()) GetWorldTimerManager().UnPauseTimer(TimerHandle);
        else GetWorldTimerManager().SetTimer(TimerHandle, this, &AActor_Cells::TimerFunction, EvoluteSpeed, true, 0);
    }
    else {
        GetWorldTimerManager().PauseTimer(TimerHandle);
    }
}
void AActor_Cells::TimerFunction() {
    // Detect active cells and add potential cells
    for (int32 index = 0; index < pCells->GetInstanceCount(); ++index) {
        if (!IsOnPanel(index)) continue;

        NotifyPotentialCells(index);
        int32 nCells = GetAroundActiveCells(index);
        if (nCells < 2 || nCells > 3) Array_DyingCells.AddUnique(index);
    }
    // Update Cells' Next Generation
    for (int32 index = 0; index < Array_DyingCells.Num(); ++index) {
        pCells->RemoveInstance(Array_DyingCells[index]);
        // Update instanced cell's index
        for (int32 i = index+1; i < Array_DyingCells.Num(); ++i) { Array_DyingCells[i] -= 1; }
    }
    for (auto cell : Map_PotentialCells) {
        // Create new
        if (cell.Value == 3 && !CheckLocation(cell.Key)) pCells->AddInstance(FTransform(FQuat(FRotator(0, 0, 0)), FVector(cell.Key, 0)));
    }
    // Clear
    Array_DyingCells.Empty();
    Map_PotentialCells.Empty();

    //UE_LOG(LogTemp, Warning, TEXT("%d"), pCells->GetInstanceCount());
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
    GetWorld()->SweepMultiByChannel(HitResults, CellTransform.GetLocation(), CellTransform.GetLocation(), FQuat(FRotator(0,0,0)), ECollisionChannel::ECC_Visibility, FCollisionShape::MakeBox(FVector(CellSize.X, CellSize.Y, CellSize.Z)), CollisionQueryParams);

    // Ignore self
    return HitResults.Num() - 1;
}

/*************************************************
Function:         CheckLocation()
Description:    Check the location if has a cell
Calls:               this->TimerFunction()
Input:              FVector2D: Checked Location
Return:            Whether has
*************************************************/
bool AActor_Cells::CheckLocation(const FVector2D Location) {
    FHitResult HitResult;
    FCollisionQueryParams CollisionQueryParams;

    CollisionQueryParams.AddIgnoredComponent(pPanel);
    return GetWorld()->LineTraceSingleByChannel(HitResult, FVector(Location, 0), FVector(Location, 100.f), ECollisionChannel::ECC_Visibility, CollisionQueryParams);
}

/*************************************************
Function:         IsOnPanel()
Description:    Check if the cell is on the panel, if not, we will spawn a AActor_Cell to simulate physics(UInstancedStaticMesh doesn't support "Simulate Physics")
Calls:               this->TimerFunction()
Input:              int32: The instanced cell's index
Return:            If the cell is on the panel
*************************************************/
bool AActor_Cells::IsOnPanel(const int32 nIndex) {
    FTransform CellTransform;

    pCells->GetInstanceTransform(nIndex, CellTransform);

    if (CellTransform.GetLocation().X >= PanelSize.X / 2 || CellTransform.GetLocation().Y >= PanelSize.Y / 2 || CellTransform.GetLocation().X <= -PanelSize.X / 2 || CellTransform.GetLocation().Y <= -PanelSize.Y / 2) {
        // Will be remove in the next update
        Array_DyingCells.Add(nIndex);
        // Spawn to simulate physics
        GetWorld()->SpawnActor<AActor_Cell>(AActor_Cell::StaticClass(), CellTransform);

        return false;
    }

    return true;
}

/*************************************************
Function:         NotifyPotentialCells()
Description:    Notify cells around the cell that survival cell counts in 8 directions around them plus one
Calls:               this->TimerFunction()
Input:              int32: the instanced cell's index
Return:            The number of cells around the cell(8 directions)
*************************************************/
void AActor_Cells::NotifyPotentialCells(const int32 nIndex) {
    FTransform CellTransform;

    pCells->GetInstanceTransform(nIndex, CellTransform);
    FVector2D CellLocation(CellTransform.GetLocation());
    FVector2D Location(0, 0);
    
    // Up
    Location = FVector2D(CellLocation.X + CellSize.X, CellLocation.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // Down
    Location = FVector2D(CellLocation.X - CellSize.X, CellLocation.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // Left
    Location = FVector2D(CellLocation.X, CellLocation.Y - CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // Right
    Location = FVector2D(CellLocation.X, CellLocation.Y + CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // UpLeft
    Location = FVector2D(CellLocation.X + CellSize.X, CellLocation.Y - CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // UpRight
    Location = FVector2D(CellLocation.X + CellSize.X, CellLocation.Y + CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // DownLeft
    Location = FVector2D(CellLocation.X - CellSize.X, CellLocation.Y - CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
    // DownRight
    Location = FVector2D(CellLocation.X - CellSize.X, CellLocation.Y + CellSize.Y);
    if (Map_PotentialCells.Contains(Location)) Map_PotentialCells[Location] += 1;
    else Map_PotentialCells.Add(Location, 1);
}
