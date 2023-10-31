// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	//native construct needed
	virtual void NativeConstruct() override;


private:
	//player reference
	class ADoomCharacter* playerCharacter;

	//Ammo Text Blocks
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* bulletNo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* maxBulletNo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* shellNo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* maxShellNo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* rocketNo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* maxRocketNo;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* cellNo;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* maxCellNo;

public:

	void InitlizeAmmo();

	void UpdateBullet();

	void UpdateShell();

	void UpdateRocket();

	void UpdateCell();


};
