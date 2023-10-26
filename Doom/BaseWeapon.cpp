// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "PaperFlipbookComponent.h"
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
	
}

// Called every frame
void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// float ABaseWeapon::GetShootingFlipbookLength() const {
// 	return ShootingFlipbook->GetTotalDuration();
// }

void ABaseWeapon::FireWeapon(){
	UE_LOG(LogTemp, Warning, TEXT("BaseWeapon Fire"));

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
	
	if (hasHit) {
		AActor* HitActor = HitResult.GetActor();
		AActor* myOwner = GetOwner();
		AController* MyOwnerInstigator = GetInstigatorController();
		auto DamageTypeClass = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(HitActor, weaponDamage, MyOwnerInstigator, this, DamageTypeClass);
	}

	PlayFireAnimation();
}

void ABaseWeapon::PlayFireAnimation() {
	
	WeaponFlipBookComponent->SetFlipbook(ShootingFlipbook);
	
	//Set Flipbook back to Idle
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
	   WeaponFlipBookComponent->SetFlipbook(IdleFlipbook);
	}, WeaponFlipBookComponent->GetFlipbookLength(), false);

}