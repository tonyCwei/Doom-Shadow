// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "Doom/DoomCharacter.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UPlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();
    playerCharacter = Cast<ADoomCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
    InitlizeAmmo();
}

void UPlayerHUD::InitlizeAmmo() {
    UpdateBullet();
    maxBulletNo->SetText(FText::AsNumber(playerCharacter->getMaxBullet()));

    UpdateShell();
    maxShellNo->SetText(FText::AsNumber(playerCharacter->getMaxShell()));

    UpdateRocket();
    maxRocketNo->SetText(FText::AsNumber(playerCharacter->getMaxRocket()));

    UpdateCell();
    maxCellNo->SetText(FText::AsNumber(playerCharacter->getMaxCell()));
}

void UPlayerHUD::UpdateBullet() {
    bulletNo->SetText(FText::AsNumber(playerCharacter->getBullet()));
}

void UPlayerHUD::UpdateShell() {
    shellNo->SetText(FText::AsNumber(playerCharacter->getShell()));
}

void UPlayerHUD::UpdateRocket() {
    rocketNo->SetText(FText::AsNumber(playerCharacter->getRocket()));
}

void UPlayerHUD::UpdateCell() {
    cellNo->SetText(FText::AsNumber(playerCharacter->getCell()));
}


