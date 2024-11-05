// Fill out your copyright notice in the Description page of Project Settings.


#include "Harubus.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "Doom/DoomCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <Kismet/KismetMathLibrary.h>
#include "Doom/Projectile/BaseProjectile.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"


#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

AHarubus::AHarubus()
{

	AnimationFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AnimationFlipBookComponent"));
	AnimationFlipBookComponent->SetupAttachment(RootComponent);

	AnimationFlipBookComponent->SetLooping(false);
	
	ProjectileSpawn2 = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawn2"));
	ProjectileSpawn2->SetupAttachment(RootComponent);

	onetwentyTimeline = CreateDefaultSubobject<UTimelineComponent> (TEXT("OnetwentyTimeline"));

	dropAttackTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DropAttackTimeline"));

}

void AHarubus::BeginPlay()
{
	Super::BeginPlay();


	//120 Attack Timeline Binding
	if (onetwentyFloatCurve) {
		// Bind the update function
		FOnTimelineFloat onetwentyUpdateFunction;
		onetwentyUpdateFunction.BindUFunction(this, FName("onetwentyTimelineUpdate"));

		// Bind the finished function
		FOnTimelineEvent onetwentyFinishedFunction;
		onetwentyFinishedFunction.BindUFunction(this, FName("onetwentyTimelineFinished"));

		// Add functions to the timeline
		onetwentyTimeline->AddInterpFloat(onetwentyFloatCurve, onetwentyUpdateFunction);
		onetwentyTimeline->SetTimelineFinishedFunc(onetwentyFinishedFunction);

		// Set the timeline to loop or play once
		onetwentyTimeline->SetLooping(false);

	}

	//Drop Attack Timeline Binding
	if (dropAttackFloatCurve) {
		// Bind the update function
		FOnTimelineFloat dropAttackUpdateFunction;
		dropAttackUpdateFunction.BindUFunction(this, FName("dropAttackTimelineUpdate"));

		// Bind the finished function
		FOnTimelineEvent dropAttackFinishedFunction;
		dropAttackFinishedFunction.BindUFunction(this, FName("dropAttackTimelineFinished"));

		// Add functions to the timeline
		dropAttackTimeline->AddInterpFloat(dropAttackFloatCurve, dropAttackUpdateFunction);
		dropAttackTimeline->SetTimelineFinishedFunc(dropAttackFinishedFunction);

		// Set the timeline to loop or play once
		dropAttackTimeline->SetLooping(false);

	}


}

void AHarubus::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorldTimerManager().ClearTimer(onetwentyAttackTimer);
}

void AHarubus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHarubus::facePlayerYaw()
{
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), playerCharacter->GetActorLocation());
	this->SetActorRotation(FRotator(0, TargetRotation.Yaw, 0));
}










void AHarubus::ShootProjectle()
{
	

	GetCharacterMovement()->StopMovementImmediately();

	FVector spawnLocation1 = ProjectileSpawn->GetComponentLocation();
	FRotator spawnRotation1 = ProjectileSpawn->GetComponentRotation();
	


	FVector spawnLocation2 = ProjectileSpawn2->GetComponentLocation();
	FRotator spawnRotation2 = ProjectileSpawn2->GetComponentRotation();
	



	if (ProjectileClass) {
		ABaseProjectile* Projectile1 = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, spawnLocation1, spawnRotation1);
		if (Projectile1) {
			Projectile1->SetOwner(this);

		}

		ABaseProjectile* Projectile2 = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, spawnLocation2, spawnRotation2);
		if (Projectile2) {
			Projectile2->SetOwner(this);

		}
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Empty ProjectileClass"));
	}



}

void AHarubus::onetwentyAttack()
{

	isAttacking = true;
	attackingstate = RangedAttacking;

	GetWorldTimerManager().SetTimer(onetwentyAttackTimer, [&]()
		{
			ShootProjectle();
		}, 0.1, true, 0);


	FRotator currRotation = GetActorRotation();

	if (FMath::RandRange(0, 1) == 0) {
		onetwentyStart = FRotator(currRotation.Pitch, currRotation.Yaw - 60, currRotation.Roll);
		onetwentyEnd = FRotator(currRotation.Pitch, currRotation.Yaw + 60, currRotation.Roll);
	}
	else {
		onetwentyStart = FRotator(currRotation.Pitch, currRotation.Yaw + 60, currRotation.Roll);
		onetwentyEnd = FRotator(currRotation.Pitch, currRotation.Yaw - 60, currRotation.Roll);
	}

	onetwentyTimeline->PlayFromStart();
}



void AHarubus::onetwentyTimelineUpdate(float Value)
{
	
	FRotator newRoation = FMath::Lerp(onetwentyStart, onetwentyEnd, Value);
	this->SetActorRotation(newRoation);

}

void AHarubus::onetwentyTimelineFinished()
{
	GetWorldTimerManager().ClearTimer(onetwentyAttackTimer);
	isAttacking = false;
}


void AHarubus::startPreDropAttack()
{
	// isInvin off when finish drop attack
	isInvin = true;

	disableGravity();

	EnemyFlipBookComponent->SetVisibility(false);

	if (preDropFlipbook) {
		AnimationFlipBookComponent->SetFlipbook(preDropFlipbook);

		AnimationFlipBookComponent->SetRelativeLocation(FVector(0,0,40));
		AnimationFlipBookComponent->SetRelativeScale3D(FVector(9, 9, 9));

		AnimationFlipBookComponent->Play();
	}

	//Disappear after flame
	FTimerHandle disappearTimerHandle;
	GetWorldTimerManager().SetTimer(disappearTimerHandle, [&]()
		{
			UE_LOG(LogTemp, Display, TEXT("disappearTimerHandle"));
			this->SetActorLocation(GetActorLocation() + FVector(0, 0, 1000));
		}, AnimationFlipBookComponent->GetFlipbookLength(), false);


	//Determine drop location after dropAttackDelay
	FTimerHandle startDropTimerHandle;

	GetWorldTimerManager().SetTimer(startDropTimerHandle, [&]()
		{
			UE_LOG(LogTemp, Display, TEXT("startDropTimerHandle"));
			startDropAttack();
		}, dropAttackDelay, false);

}

void AHarubus::startDropAttack()
{
	shouldUpdateDirectionalSprite = false;

	setCollisionOverlapPawn();

	EnemyFlipBookComponent->SetVisibility(true);
	EnemyFlipBookComponent->SetFlipbook(directionalFlipbooks[0]);

	dropAttackEndLocation = playerCharacter->GetActorLocation();
	dropAttackStartLocation = dropAttackEndLocation + FVector(0, 0, 1000);

	this->SetActorLocation(dropAttackStartLocation);

	if (dropAttackIndicator) {
		GetWorld()->SpawnActor<AActor>(dropAttackIndicator, dropAttackEndLocation, FRotator(0,0,0));
	}
	
	//give player time to run away from the red circle
	FTimerHandle actualDropTimerHandle;

	GetWorldTimerManager().SetTimer(actualDropTimerHandle, [&]()
		{
			EnemyFlipBookComponent->SetRelativeRotation(FRotator(0, -90, 0));
			dropAttackTimeline->PlayFromStart();
		}, dropAttackIndicatorTime, false);
	

}

void AHarubus::dropAttackTimelineUpdate(float Value)
{
	facePlayerYaw();

	FVector newLocation = FMath::Lerp(dropAttackStartLocation, dropAttackEndLocation + FVector(0, 0, 150), Value);
	this->SetActorLocation(newLocation);

}

void AHarubus::dropAttackTimelineFinished()
{
	dropAttackSphereTrace();
	isInvin = false;
	shouldUpdateDirectionalSprite = true;
	enableGravity();

	if (dropAttackLandEffect) {
		FVector effectLocation = dropAttackEndLocation + FVector(0, 0, -100);

		GetWorld()->SpawnActor<AActor>(dropAttackLandEffect, effectLocation, FRotator(0, 0, 0));
	}


	
	FTimerHandle resetCollisionTimerHandle;

	GetWorldTimerManager().SetTimer(resetCollisionTimerHandle, [&]()
		{
			setCollisionBlockPawn();
		}, 0.2, false);
	


}

void AHarubus::disableGravity()
{
	GetCharacterMovement()->GravityScale = 0;
}

void AHarubus::enableGravity()
{
	GetCharacterMovement()->GravityScale = 1;
}

void AHarubus::setCollisionOverlapPawn()
{
	if (myCapsuleComponent) {
		myCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

void AHarubus::setCollisionBlockPawn()
{
	if (myCapsuleComponent) {
		myCapsuleComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	}
}



void AHarubus::dropAttackSphereTrace()
{
	TArray<FHitResult> HitResults;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = { UEngineTypes::ConvertToObjectType(ECC_Pawn) };
	TArray<AActor*> ActorsToIgnore = { Cast<AActor>(this) };

	bool hasHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		this->GetWorld(),
		dropAttackEndLocation,
		dropAttackEndLocation,
		dropAttackRange,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::Type::ForDuration,
		HitResults,
		true
	);


	if (hasHit) {
		for (FHitResult hitResult : HitResults) {
			AActor* HitActor = hitResult.GetActor();
			AController* MyInstigator = GetInstigatorController();
			auto DamageTypeClass = UDamageType::StaticClass();
			if (HitActor->ActorHasTag("Player")) {
				UGameplayStatics::ApplyDamage(HitActor, dropAttackDamage, MyInstigator, this, DamageTypeClass);

				FVector launchDirection;
				
				if (playerCharacter->GetActorLocation() != dropAttackEndLocation) {
					launchDirection = playerCharacter->GetActorLocation() - dropAttackEndLocation;
				}
				else {
					launchDirection = -playerCharacter->GetActorForwardVector();
				}
				
				
				launchDirection.Normalize();


				FVector launchVelocity = FVector(launchDirection.X * 10000, launchDirection.Y * 10000, 0);

				playerCharacter->LaunchCharacter(launchVelocity, false, false);
			}
		}

	}
}

void AHarubus::dropAttack()
{
	facePlayerYaw();
	startPreDropAttack();

}


//Drop Attack finish
// IsInvin = false, shoudl update driectional sprite = true, enable gravity,