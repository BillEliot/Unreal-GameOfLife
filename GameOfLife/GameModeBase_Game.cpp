// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_Game.h"
#include "Pawn/Pawn_View.h"
#include "Actor/Actor_Cells.h"
#include "HUD_Game.h"
#include "Engine/World.h"
#include "Public/EngineUtils.h"


AGameModeBase_Game::AGameModeBase_Game() {
    ViewMode = EViewMode::E_GOD;

    DefaultPawnClass = APawn_View::StaticClass();
    HUDClass = AHUD_Game::StaticClass();
}

void AGameModeBase_Game::BeginPlay() {
    Super::BeginPlay();

    pPlayerController = GetWorld()->GetFirstPlayerController();
    SetUIMode();
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
    if (pCells) {
        pCells->Start(bStart);
    }
}

void AGameModeBase_Game::OneStep() {
    if (pCells) {
        pCells->OneStep();
    }
}
