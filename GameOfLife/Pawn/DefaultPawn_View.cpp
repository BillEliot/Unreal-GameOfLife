// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultPawn_View.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameModeBase_Game.h"
#include "Engine/World.h"
#include "Actor/Actor_Cells.h"


ADefaultPawn_View::ADefaultPawn_View() {
    PrimaryActorTick.bCanEverTick = false;
    AutoReceiveInput = EAutoReceiveInput::Player0;

    pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    pCamera->SetupAttachment(RootComponent);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;

    speed = 800.f;
}

void ADefaultPawn_View::BeginPlay() {
    Super::BeginPlay();

    Cast<APlayerController>(GetController())->SetControlRotation(FRotator(-90.f, 0, 0));
    SetActorRotation(FRotator(-90.f,0,0));
    SetActorLocation(FVector(0,0,4500.f));
    pGameMode_Game = Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode());
}

void ADefaultPawn_View::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ADefaultPawn_View::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADefaultPawn_View::MoveRight);
    PlayerInputComponent->BindAxis("Scale", this, &ADefaultPawn_View::Scale);

    PlayerInputComponent->BindAction("SetCell", IE_Pressed, this, &ADefaultPawn_View::SetCell);
    PlayerInputComponent->BindAction("ToggleUI", IE_Pressed, this, &ADefaultPawn_View::ToggleUI);
    PlayerInputComponent->BindAction("GodView", IE_Pressed, this, &ADefaultPawn_View::GodView);
    PlayerInputComponent->BindAction("FreeView", IE_Pressed, this, &ADefaultPawn_View::FreeView);
}

void ADefaultPawn_View::MoveForward(const float axis) {
    if (axis != 0) {
        EViewMode ViewMode = pGameMode_Game->GetViewMode();
        switch (ViewMode) {
        case EViewMode::E_FREE:
            AddMovementInput(GetActorForwardVector(), axis);
            break;
        case EViewMode::E_GOD:
            AddActorLocalOffset(FVector(0, 0, axis * GetWorld()->DeltaTimeSeconds * speed));
            break;
        }
    }
}
void ADefaultPawn_View::MoveRight(const float axis) {
    if (axis != 0) {
        EViewMode ViewMode = pGameMode_Game->GetViewMode();
        switch (ViewMode) {
        case EViewMode::E_FREE:
            AddMovementInput(GetActorRightVector(), axis);
            break;
        case EViewMode::E_GOD:
            AddActorLocalOffset(FVector(0, axis * GetWorld()->DeltaTimeSeconds * speed, 0));
            break;
        }
    }
}
void ADefaultPawn_View::Scale(const float axis) {
    if (axis != 0) {
        EViewMode ViewMode = pGameMode_Game->GetViewMode();
        if (ViewMode == EViewMode::E_GOD) AddActorLocalOffset(FVector(axis * GetWorld()->DeltaTimeSeconds * 2000.f, 0, 0));
    }
}

void ADefaultPawn_View::SetCell() {
    APlayerController* pPlayerController = pGameMode_Game->GetPlayerController();
    FVector WorldPosition, WorldDirection;
    FHitResult HitResult;

    if (pPlayerController->bShowMouseCursor) {
        // Mouse Pos To World Pos
        pPlayerController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
        // Trace
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, WorldPosition+ WorldDirection * 10000.f, ECollisionChannel::ECC_Visibility);
        if (Cast<AActor_Cells>(HitResult.GetActor())) {
            if (HitResult.GetComponent() == Cast<UPrimitiveComponent>(Cast<AActor_Cells>(HitResult.GetActor())->GetPanel())) {
                Cast<AActor_Cells>(HitResult.GetActor())->SetCell(HitResult.ImpactPoint);
            }
            else {
                Cast<AActor_Cells>(HitResult.GetActor())->UnsetCell(HitResult.ImpactPoint);
            }
        }
    }
}

void ADefaultPawn_View::ToggleUI() {
    pGameMode_Game->ToggleUI();
}

void ADefaultPawn_View::ResetView() {
    Cast<APlayerController>(GetController())->SetControlRotation(FRotator(-90.f, 0, 0));
    SetActorRotation(FRotator(-90.f, 0, 0));
    SetActorLocation(FVector(0, 0, 4500.f));
}

void ADefaultPawn_View::GodView() {
    if (pGameMode_Game) pGameMode_Game->SetViewMode(EViewMode::E_GOD);
}
void ADefaultPawn_View::FreeView() {
    if (pGameMode_Game) pGameMode_Game->SetViewMode(EViewMode::E_FREE);
}
