// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketLauncher.h"

void ARocketLauncher::FireWeapon() {
    if (bFireOnce) {
        bFireOnce = false;
        ShootProjectle();
        resetFlipbook();

        //control fire rate
        FTimerHandle RocketLauncherTimerHandle;
	    GetWorld()->GetTimerManager().SetTimer(RocketLauncherTimerHandle, [&]()
	    {
	    bFireOnce = true;
	    }, fireRate, false);  
    } 
}