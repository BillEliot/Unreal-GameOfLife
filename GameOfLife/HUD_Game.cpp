// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD_Game.h"
#include "UI/UserWidget_HUD.h"


void AHUD_Game::BeginPlay() {
    Super::BeginPlay();

    TSubclassOf<UUserWidget_HUD> class_HUD = LoadClass<UUserWidget_HUD>(this, TEXT("WidgetBlueprint'/Game/UI/UW_HUD.UW_HUD_C'"));
    if (class_HUD) {
        pHUD = CreateWidget<UUserWidget_HUD>(GetWorld(), class_HUD);
        pHUD->AddToViewport();
    }
}

