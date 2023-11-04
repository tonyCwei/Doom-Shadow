// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Shotgun.generated.h"

/**
 * 
 */
UCLASS()
class DOOM_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float FireRate = 1.5f;

	bool FireOnce = true;


public:

	void FireWeapon() override;
	
};
