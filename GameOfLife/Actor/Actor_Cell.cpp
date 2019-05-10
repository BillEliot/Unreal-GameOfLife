// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor_Cell.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"


// Sets default values
AActor_Cell::AActor_Cell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    InitialLifeSpan = 5.f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> object_Cell(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> object_Material_Cell(TEXT("Material'/Game/M_DefaultCell.M_DefaultCell'"));
    pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    pMesh->SetupAttachment(RootComponent);
    pMesh->SetSimulatePhysics(true);
    if (object_Cell.Succeeded()) pMesh->SetStaticMesh(object_Cell.Object);
    if (object_Material_Cell.Succeeded()) pMesh->SetMaterial(0, object_Material_Cell.Object);
}

// Called when the game starts or when spawned
void AActor_Cell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActor_Cell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

