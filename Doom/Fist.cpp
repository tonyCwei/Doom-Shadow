// Fill out your copyright notice in the Description page of Project Settings.


#include "Fist.h"
// #include "PaperFlipbookComponent.h"
// #include "PaperFlipbook.h"
// #include "Kismet/KismetSystemLibrary.h"
// #include "Kismet/GameplayStatics.h"
// #include "Kismet/KismetMathLibrary.h"
// #include "GameFramework/Character.h"

void AFist::FireWeapon() {
   Super::FireWeapon();
   resetFlipbook();
}

// void AFist::Punch() {
//     FVector lineTraceLocation = LineTraceComponent->GetComponentLocation();
//     FVector lineTraceForward = UKismetMathLibrary::GetForwardVector(LineTraceComponent->GetComponentRotation());
// 	FVector lineTraceEnd = lineTraceForward * lineTraceDistance + lineTraceLocation;
// 	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_WorldStatic), 
// 														 UEngineTypes::ConvertToObjectType(ECC_WorldDynamic), 	
// 														 UEngineTypes::ConvertToObjectType(ECC_Pawn)};
	
// 	TArray<AActor*> ActorsToIgnore = {Cast<AActor>(this), UGameplayStatics::GetPlayerCharacter(this,0)};
// 	FHitResult HitResult;													 
	
// 	//Line Trace
// 	bool hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(this->GetWorld(), 
// 													lineTraceLocation, 
// 													lineTraceEnd, 
// 													ObjectTypes, 
// 													false, 
// 													ActorsToIgnore,
// 													EDrawDebugTrace::Type::ForDuration, 
// 													HitResult, 
// 													true);
	
// 	//Apply DMG
// 	if (hasHit) {
// 		AActor* HitActor = HitResult.GetActor();
// 		AActor* myOwner = GetOwner();
// 		AController* MyOwnerInstigator = GetInstigatorController();
// 		auto DamageTypeClass = UDamageType::StaticClass();
// 		UGameplayStatics::ApplyDamage(HitActor, weaponDamage, MyOwnerInstigator, this, DamageTypeClass);
// 	}

//     PlayFireAnimation();

//     FTimerHandle MeleeTimerHandle;
// 	GetWorld()->GetTimerManager().SetTimer(MeleeTimerHandle, [&]()
// 	{
// 	    WeaponFlipBookComponent->SetFlipbook(IdleFlipbook);	   
// 	}, WeaponFlipBookComponent->GetFlipbookLength(), false);
   
// }