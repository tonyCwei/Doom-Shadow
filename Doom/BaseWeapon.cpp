// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "PaperFlipbookComponent.h"
#include "DoomCharacter.h"
#include "UI/PlayerHUD.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PaperFlipbook.h"
#include "GameFramework/Character.h"

// Sets default values
ABaseWeapon::ABaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComponent;


	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	ArrowComponent->SetupAttachment(RootComponent);

	WeaponFlipBookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WeaponFlipBook"));
	WeaponFlipBookComponent->SetupAttachment(RootComponent);
	WeaponFlipBookComponent->SetRelativeRotation(FRotator(0.f,90.f,0.f));
	WeaponFlipBookComponent->SetWorldScale3D(FVector(0.07,0.07,0.07));

	LineTraceComponent = CreateDefaultSubobject<USceneComponent>(TEXT("LineTrace"));
	LineTraceComponent->SetupAttachment(RootComponent);
	LineTraceComponent->SetRelativeLocation(FVector(2.f,0.f,0.f));

	
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	IdleFlipbook = WeaponFlipBookComponent->GetFlipbook();
	playerCharacter = Cast<ADoomCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ABaseWeapon::FireWeapon(){
	//UE_LOG(LogTemp, Warning, TEXT("BaseWeapon Fire"));

	if (!hasEnoughAmmo()) return;
	//Parameters for line trace for objects
	FVector lineTraceLocation = LineTraceComponent->GetComponentLocation();
    FVector lineTraceForward = UKismetMathLibrary::GetForwardVector(LineTraceComponent->GetComponentRotation());
	FVector lineTraceEnd = lineTraceForward * lineTraceDistance + lineTraceLocation;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UEngineTypes::ConvertToObjectType(ECC_WorldStatic), 
														 UEngineTypes::ConvertToObjectType(ECC_WorldDynamic), 	
														 UEngineTypes::ConvertToObjectType(ECC_Pawn)};
	
	TArray<AActor*> ActorsToIgnore = {Cast<AActor>(this), UGameplayStatics::GetPlayerCharacter(this,0)};
	FHitResult HitResult;													 
	
	//Line Trace
	bool hasHit = UKismetSystemLibrary::LineTraceSingleForObjects(this->GetWorld(), 
													lineTraceLocation, 
													lineTraceEnd, 
													ObjectTypes, 
													false, 
													ActorsToIgnore,
													EDrawDebugTrace::Type::ForDuration, 
													HitResult, 
													true);
	
	//Apply DMG
	if (hasHit) {
		AActor* HitActor = HitResult.GetActor();
		AActor* myOwner = GetOwner();
		AController* MyOwnerInstigator = GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(HitActor, weaponDamage, MyOwnerInstigator, this, DamageTypeClass);
	}

	//Decrease Ammo
	decreaseAmmo();
	
	//Animation
	PlayFireAnimation();
}

//For Weapons shoot projectle instead of line trace
void ABaseWeapon::ShootProjectle() {
	if (!hasEnoughAmmo()) return;
	
	//UE_LOG(LogTemp, Display, TEXT("Shoot projectle"));
	
	//Decrease Ammo
	decreaseAmmo();
	
	//Animation
	PlayFireAnimation();
}


void ABaseWeapon::resetFlipbook() {
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
	   WeaponFlipBookComponent->SetFlipbook(IdleFlipbook);
	}, WeaponFlipBookComponent->GetFlipbookLength(), false);
}

void ABaseWeapon::PlayFireAnimation() {
	WeaponFlipBookComponent->SetFlipbook(ShootingFlipbook);
}

//for hold fire weapon only
void ABaseWeapon::StopFire() {
	
}

bool ABaseWeapon::hasEnoughAmmo() {
	switch (ammoType)
	{
	case Bullet:
		return playerCharacter->getBullet() > 0;	
	
	case Shell:
		return playerCharacter->getShell() > 0;
		
	case Rocket:
		return playerCharacter->getRocket() > 0;
	
	case Cell:
		return playerCharacter->getCell() > 0;
	
	default:
		return false;
	}
}

void ABaseWeapon::decreaseAmmo() {
	switch (ammoType)
	{
	case Bullet:
		 playerCharacter->decreaseBullet();	
		 playerCharacter->getPlayerHUD()->UpdateBullet();
		 break;
	
	case Shell:
		 playerCharacter->decreaseShell();
		 playerCharacter->getPlayerHUD()->UpdateShell();
		 break;
		
	case Rocket:
		 playerCharacter->decreaseRocket();
		 playerCharacter->getPlayerHUD()->UpdateRocket();
		 break;
	
	case Cell:
		 playerCharacter->decreaseCell();
		 playerCharacter->getPlayerHUD()->UpdateCell();
		 break;
	
	default:
		return;
	}
}