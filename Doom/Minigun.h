// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Minigun.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API AMinigun : public ABaseWeapon
{
	GENERATED_BODY()

private:
	FTimerHandle minigunEvent;


public:

	void FireWeapon() override;

	void StopFire() override;

	virtual void CallFire(); 
	
};
