// Fill out your copyright notice in the Description page of Project Settings.

#include "UserWidget_HUD.h"
#include "GameModeBase_Game.h"

UUserWidget_HUD::UUserWidget_HUD(const FObjectInitializer& objectInitializer) : Super(objectInitializer) {
    bStart = false;
    StartText = FText::FromString("START");
}

void UUserWidget_HUD::ChangeEvoluteSpeed(const float Value) {
    Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode())->ChangeEvoluteSpeed(FMath::Clamp<float>(1.f - Value, 0.1f, 1.f));
    if (!bStart) {
        StartText = FText::FromString("STOP");
        bStart = true;
    }
}

void UUserWidget_HUD::StartGame() {
    if (bStart) {
        Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode())->Start(false);
        StartText = FText::FromString("START");
        bStart = false;
    }
    else {
        Cast<AGameModeBase_Game>(GetWorld()->GetAuthGameMode())->Start(true);
        StartText = FText::FromString("STOP");
        bStart = true;
    }
}
