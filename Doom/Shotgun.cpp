// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

void AShotgun::FireWeapon() {
    if (FireOnce) {
        FireOnce = false;
        Super::FireWeapon();
        FTimerHandle TimerHandle;
	    GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	    {
	    FireOnce = true;
	    }, FireRate, false);  
    } 
}