// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigun.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "BaseWeapon.h"



void AMinigun::FireWeapon() {
    GetWorld()->GetTimerManager().SetTimer(minigunEvent, this, &AMinigun::CallSuperFire, 0.15, true, 0);
}


void AMinigun::CallSuperFire() {
    Super::FireWeapon();
    //if (!hasEnoughAmmo()) StopFire();
}


void AMinigun::StopFire() {
    GetWorldTimerManager().ClearTimer(minigunEvent);
    resetFlipbook();
}