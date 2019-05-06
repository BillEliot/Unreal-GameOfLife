// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawn_View.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameModeBase_Game.h"
#include "Engine/World.h"
#include "Actor/Actor_Cells.h"


APawn_View::APawn_View() {
    PrimaryActorTick.bCanEverTick = false;

    speed = 800.f;

    pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    RootComponent = pCamera;
}

void APawn_View::BeginPlay() {
    Super::BeginPlay();

    SetActorRotation(FQuat(FRotator(-90.f, 0, 0)));
    SetActorLocation(FVector(0,0,8000.f));
    pGameMode_Game = Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode());
}

void APawn_View::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Turn", this, &APawn_View::Turn);
    PlayerInputComponent->BindAxis("LookUp", this, &APawn_View::LookUp);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawn_View::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &APawn_View::MoveRight);

    PlayerInputComponent->BindAction("SetCell", EInputEvent::IE_Pressed, this, &APawn_View::SetCell);
}

void APawn_View::Turn(const float axis) {
    if (axis != 0 && pGameMode_Game->GetViewMode() == EViewMode::E_FREE) {
        AddControllerYawInput(axis);
    }
}
void APawn_View::LookUp(const float axis) {
    if (axis != 0 && pGameMode_Game->GetViewMode() == EViewMode::E_FREE) {
        AddControllerPitchInput(axis);
    }
}
void APawn_View::MoveForward(const float axis) {
    if (axis != 0) {
        EViewMode ViewMode = pGameMode_Game->GetViewMode();
        switch (ViewMode) {
        case EViewMode::E_FREE:
            AddActorLocalOffset(axis * GetWorld()->DeltaTimeSeconds * speed * GetActorForwardVector());
            break;
        case EViewMode::E_GOD:
            AddActorLocalOffset(FVector(axis * GetWorld()->DeltaTimeSeconds * speed, 0, 0));
            break;
        }
    }
}
void APawn_View::MoveRight(const float axis) {
    if (axis != 0) {
        EViewMode ViewMode = pGameMode_Game->GetViewMode();
        switch (ViewMode) {
        case EViewMode::E_FREE:
            AddActorLocalOffset(axis * GetWorld()->DeltaTimeSeconds * speed * GetActorRightVector());
            break;
        case EViewMode::E_GOD:
            AddActorLocalOffset(FVector(0, axis * GetWorld()->DeltaTimeSeconds * speed, 0));
            break;
        }
    }
}

void APawn_View::SetCell() {
    APlayerController* pPlayerController = pGameMode_Game->GetPlayerController();
    FVector WorldPosition, WorldDirection;
    FHitResult HitResult;

    if (pPlayerController->bShowMouseCursor) {
        // Mouse Pos To World Pos
        pPlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
        // Trace
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition+FVector(0,0,-1.f)*10000.f, ECollisionChannel::ECC_Visibility);
        if (HitResult.GetComponent() == Cast<UPrimitiveComponent>(Cast<AActor_Cells>(HitResult.GetActor())->GetPanel())) {
            Cast<AActor_Cells>(HitResult.GetActor())->SetCell(HitResult.ImpactPoint);
        }
    }
}
