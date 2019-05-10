// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_Game.h"
#include "Pawn/DefaultPawn_View.h"
#include "Pawn/Character_Dragon.h"
#include "Actor/Actor_Cells.h"
#include "HUD_Game.h"
#include "Engine/World.h"
#include "Public/EngineUtils.h"


AGameModeBase_Game::AGameModeBase_Game() {
    ViewMode = EViewMode::E_GOD;

    DefaultPawnClass = ADefaultPawn_View::StaticClass();
    HUDClass = AHUD_Game::StaticClass();
}

void AGameModeBase_Game::BeginPlay() {
    Super::BeginPlay();

    pPlayerController = GetWorld()->GetFirstPlayerController();
    SetUIMode();
    // Get Actor_Cells
    for (TActorIterator<AActor_Cells> iterator(GetWorld()); iterator; ++iterator) { pCells = *iterator; }
    // Get Dragon
    for (TActorIterator<ACharacter_Dragon> iterator(GetWorld()); iterator; ++iterator) { pDragon = *iterator; }
}

void AGameModeBase_Game::SetGameMode() {
    FInputModeGameOnly InputMode;
    pPlayerController->SetInputMode(InputMode);
    pPlayerController->bShowMouseCursor = false;
}
void AGameModeBase_Game::SetUIMode() {
    FInputModeGameAndUI InputMode;
    InputMode.SetHideCursorDuringCapture(false);
    pPlayerController->SetInputMode(InputMode);
    pPlayerController->bShowMouseCursor = true;
}

void AGameModeBase_Game::Start(const bool bStart) {
    if (pCells) pCells->Start(bStart);
}

void AGameModeBase_Game::Reset() {
    if (pCells) pCells->Reset();
}

void AGameModeBase_Game::OneStep() {
    if (pCells) pCells->OneStep();
}

void AGameModeBase_Game::ChangeEvoluteSpeed(const float Speed) {
    if (pCells) pCells->ChangeEvoluteSpeed(Speed);
}

void AGameModeBase_Game::ChangePanelSize(const int32 nWidthScale, const int32 nHeightScale) {
    if (pCells) pCells->ChangePanelSize(nWidthScale, nHeightScale);
}

void AGameModeBase_Game::SetViewMode(const EViewMode viewMode) {
    ViewMode = viewMode;
    if (ViewMode == EViewMode::E_FREE) {
        SetGameMode();
        Cast<ADefaultPawn_View>(pPlayerController->GetPawn())->bUseControllerRotationYaw = true;
        Cast<ADefaultPawn_View>(pPlayerController->GetPawn())->bUseControllerRotationPitch = true;
    }
    else {
        SetUIMode();
        Cast<ADefaultPawn_View>(pPlayerController->GetPawn())->bUseControllerRotationYaw = false;
        Cast<ADefaultPawn_View>(pPlayerController->GetPawn())->bUseControllerRotationPitch = false;
    }
}

void AGameModeBase_Game::ResetView() {
    Cast<ADefaultPawn_View>(pPlayerController->GetPawn())->ResetView();
}

void AGameModeBase_Game::ToggleUI() {
    Cast<AHUD_Game>(pPlayerController->GetHUD())->ToggleUI();
}

void AGameModeBase_Game::ToggleDragon(const bool bDragonVisible) {
    if (pDragon) pDragon->ToggleDragon(bDragonVisible);
}

void AGameModeBase_Game::ToggleWireframe(const bool bWireframeVisible) {
    if (pCells) pCells->ToogleWireframe(bWireframeVisible);
}
