// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "BFG.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API ABFG : public ABaseWeapon
{
	GENERATED_BODY()

private:
	
	bool bFireOnce = true;

public:

	void FireWeapon() override;
	
	void ShootProjectle() override;
};
