// Fill out your copyright notice in the Description page of Project Settings.

#include "GameModeBase_Game.h"
#include "Pawn/Pawn_View.h"
#include "HUD_Game.h"
#include "Engine/World.h"


AGameModeBase_Game::AGameModeBase_Game() {
    ViewMode = EViewMode::E_GOD;

    DefaultPawnClass = APawn_View::StaticClass();
    HUDClass = AHUD_Game::StaticClass();
}

void AGameModeBase_Game::BeginPlay() {
    Super::BeginPlay();

    pPlayerController = GetWorld()->GetFirstPlayerController();
    SetUIMode();
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
