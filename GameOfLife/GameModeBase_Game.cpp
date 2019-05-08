// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_Game.h"
#include "Pawn/DefaultPawn_View.h"
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
    for (TActorIterator<AActor_Cells> iterator(GetWorld()); iterator; ++iterator) {
        pCells = *iterator;
    }
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

void AGameModeBase_Game::SetViewMode(const EViewMode viewMode) {
    ViewMode = viewMode;
    if (ViewMode == EViewMode::E_FREE) SetGameMode();
}
