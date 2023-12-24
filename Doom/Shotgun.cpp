// Fill out your copyright notice in the Description page of Project Settings.


#include "Shotgun.h"

void AShotgun::FireWeapon() {
    if (bFireOnce) {
        bFireOnce = false;
        Super::FireWeapon();
        resetFlipbook();

        //control fire rate
        FTimerHandle ShotgunTimerHandle;
	    GetWorld()->GetTimerManager().SetTimer(ShotgunTimerHandle, [&]()
	    {
	    bFireOnce = true;
	    }, fireRate, false);  
    } 
}