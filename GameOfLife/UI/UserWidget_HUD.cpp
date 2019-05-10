// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_HUD.h"
#include "GameModeBase_Game.h"
#include "Kismet/KismetSystemLibrary.h"

UUserWidget_HUD::UUserWidget_HUD(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
    bStart = false;
    bDragonVisible = true;
    bWireframeVisible = false;
    StartText = FText::FromString("START");
    DragonText = FText::FromString("Hide Dragon");
    WireframeText = FText::FromString("Show Wireframe");
}

// BeginPlay
void UUserWidget_HUD::NativeConstruct() {
    Super::NativeConstruct();

    pGameModeBase_Game = Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode());
}

void UUserWidget_HUD::ChangeEvoluteSpeed(const float Value) {
    Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode())->ChangeEvoluteSpeed(FMath::Clamp<float>(1.f - Value, 0.1f, 1.f));
    if (!bStart) {
        StartText = FText::FromString("STOP");
        bStart = true;
    }
}

void UUserWidget_HUD::ChangePanelSize(const float ValueX, const float ValueY) {
    // Slider range : 0.1 ~ 1
    // Scale range : 5 ~ 50
    int32 nPanelWidthScale = FMath::Clamp<int32>(ValueX * 50, 5, 50);
    int32 nPanelHeightScale = FMath::Clamp<int32>(ValueY * 50, 5, 50);

    if (pGameModeBase_Game) pGameModeBase_Game->ChangePanelSize(nPanelWidthScale, nPanelHeightScale);
}

void UUserWidget_HUD::StartGame() {
    if (bStart) {
        if (pGameModeBase_Game) pGameModeBase_Game->Start(false);
        StartText = FText::FromString("START");
        bStart = false;
    }
    else {
        if (pGameModeBase_Game) pGameModeBase_Game->Start(true);
        StartText = FText::FromString("STOP");
        bStart = true;
    }
}

void UUserWidget_HUD::ToggleDragon() {
    if (bDragonVisible) {
        DragonText = FText::FromString("Show Dragon");
        if (pGameModeBase_Game) pGameModeBase_Game->ToggleDragon(false);
        bDragonVisible = false;
    }
    else {
        DragonText = FText::FromString("Hide Dragon");
        if (pGameModeBase_Game) pGameModeBase_Game->ToggleDragon(true);
        bDragonVisible = true;
    }
}

void UUserWidget_HUD::ToggleWireframe() {
    if (bWireframeVisible) {
        WireframeText = FText::FromString("Hide Wireframe");
        bWireframeVisible = false;
        if (pGameModeBase_Game) pGameModeBase_Game->ToggleWireframe(false);
    }
    else {
        WireframeText = FText::FromString("Show Wireframe");
        bWireframeVisible = true;
        if (pGameModeBase_Game) pGameModeBase_Game->ToggleWireframe(true);
    }
    
}
